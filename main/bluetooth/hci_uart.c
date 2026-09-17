/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI (H4) over a hardware UART.
 *
 * The UART counterpart of hci_usb.c, for chips without a native USB-Serial/JTAG
 * (e.g. the classic ESP32, reached through its onboard USB-UART bridge on
 * UART0). It bridges the controller's VHCI interface to a UART: bytes in are
 * reassembled into HCI packets and handed to the controller, packets out are
 * streamed back. The result is an ordinary H4 controller over that UART.
 */
#include "hci_uart.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_HCI_OVER_UART

#include "driver/uart.h"
#include "esp_bt.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"
#include "freertos/task.h"

/* HCI owns this UART. A console on the same UART would interleave its text with
 * HCI packets and desynchronise the H4 stream, so it is a build error rather
 * than a device that enumerates fine and then misbehaves. Put the console on a
 * different UART, or set CONFIG_ESP_CONSOLE_NONE. */
#if CONFIG_ESP_CONSOLE_UART && (CONFIG_ESP_CONSOLE_UART_NUM == CONFIG_UNCHAINED_HCI_UART_NUM)
#error "HCI is bridged to a UART that also carries the console. Set the console elsewhere (CONFIG_ESP_CONSOLE_NONE), or use a different UART for HCI."
#endif

static const char *TAG = "HCIUART";

/* H4 packet-type indicators. */
#define H4_CMD  0x01
#define H4_ACL  0x02
#define H4_SCO  0x03
#define H4_ISO  0x05

/* Largest payload reassembled from the host. An HCI command tops out at 255 and
 * LE ACL data is far smaller, so this is headroom, not a real limit. A length
 * beyond it means the stream is out of sync rather than genuinely big. */
#define H4_MAX_PAYLOAD  1024
#define H4_MAX_HDR      4
#define H4_MAX_FRAME    (1 + H4_MAX_HDR + H4_MAX_PAYLOAD)

#define HCI_UART_PORT   (CONFIG_UNCHAINED_HCI_UART_NUM)
#define UART_RX_BUF     1024
#define TX_STREAM_BYTES 4096

static StreamBufferHandle_t s_tx;        /* controller -> UART */
static SemaphoreHandle_t    s_can_send;  /* controller is ready for a packet */

/* ---- controller -> host ------------------------------------------------- */

static void notify_host_send_available(void)
{
    xSemaphoreGive(s_can_send);
}

static int notify_host_recv(uint8_t *data, uint16_t len)
{
    /* Called in controller context, so it must not block. A partial write would
     * desynchronise the H4 stream for good, so a packet that does not fit whole
     * is dropped whole and the stream stays well-formed. */
    if (xStreamBufferSpacesAvailable(s_tx) < len) {
        ESP_LOGW(TAG, "host not draining, dropped a %u byte packet", (unsigned)len);
        return 0;
    }
    xStreamBufferSend(s_tx, data, len, 0);
    return 0;
}

static const esp_vhci_host_callback_t s_vhci_cb = {
    .notify_host_send_available = notify_host_send_available,
    .notify_host_recv = notify_host_recv,
};

static void hci_uart_tx_task(void *arg)
{
    uint8_t buf[256];
    for (;;) {
        size_t n = xStreamBufferReceive(s_tx, buf, sizeof(buf), portMAX_DELAY);
        /* Blocking write (no TX ring buffer): if the host is not reading, the
         * back-pressure lands here, on a stalled task, not in the controller. */
        for (size_t off = 0; off < n; ) {
            int w = uart_write_bytes(HCI_UART_PORT, (const char *)buf + off, n - off);
            if (w <= 0) {
                break;
            }
            off += (size_t)w;
        }
    }
}

/* ---- host -> controller ------------------------------------------------- */

/* Bytes of header that follow the packet-type indicator, or 0 for a type the
 * host has no business sending. */
static size_t h4_header_len(uint8_t type)
{
    switch (type) {
    case H4_CMD: return 3;   /* opcode(2) + plen(1)            */
    case H4_ACL: return 4;   /* handle(2) + len(2, LE)         */
    case H4_SCO: return 3;   /* handle(2) + len(1)             */
    case H4_ISO: return 4;   /* handle(2) + len(2, LE, 14 bit) */
    default:     return 0;
    }
}

static size_t h4_payload_len(uint8_t type, const uint8_t *hdr)
{
    switch (type) {
    case H4_CMD: return hdr[2];
    case H4_SCO: return hdr[2];
    case H4_ACL: return (size_t)hdr[2] | ((size_t)hdr[3] << 8);
    case H4_ISO: return ((size_t)hdr[2] | ((size_t)hdr[3] << 8)) & 0x3FFF;
    default:     return 0;
    }
}

static void send_to_controller(uint8_t *frame, size_t len)
{
    /* The controller takes one packet at a time and calls back when it can take
     * the next. */
    while (!esp_vhci_host_check_send_available()) {
        if (xSemaphoreTake(s_can_send, pdMS_TO_TICKS(1000)) != pdTRUE) {
            ESP_LOGW(TAG, "controller still not accepting packets");
        }
    }
    esp_vhci_host_send_packet(frame, (uint16_t)len);
}

static void hci_uart_rx_task(void *arg)
{
    static uint8_t frame[H4_MAX_FRAME];
    uint8_t chunk[128];
    size_t have = 0, hdr_len = 0, payload_len = 0;
    enum { WANT_TYPE, WANT_HDR, WANT_PAYLOAD } state = WANT_TYPE;

    for (;;) {
        /* Block for the first byte, then drain whatever else is already buffered
         * so a packet is handed over with minimal latency. */
        int n = uart_read_bytes(HCI_UART_PORT, chunk, 1, portMAX_DELAY);
        if (n <= 0) {
            continue;
        }
        int more = uart_read_bytes(HCI_UART_PORT, chunk + n, sizeof(chunk) - n, 0);
        if (more > 0) {
            n += more;
        }

        for (int i = 0; i < n; i++) {
            uint8_t b = chunk[i];

            switch (state) {
            case WANT_TYPE:
                /* Anything that is not a packet-type indicator is noise from a
                 * terminal probing the port; stay here until the stream makes
                 * sense again. */
                hdr_len = h4_header_len(b);
                if (hdr_len == 0) {
                    continue;
                }
                frame[0] = b;
                have = 1;
                state = WANT_HDR;
                break;

            case WANT_HDR:
                frame[have++] = b;
                if (have < 1 + hdr_len) {
                    break;
                }
                payload_len = h4_payload_len(frame[0], frame + 1);
                if (payload_len > H4_MAX_PAYLOAD) {
                    ESP_LOGW(TAG, "type 0x%02x claims %u bytes, resyncing",
                             frame[0], (unsigned)payload_len);
                    state = WANT_TYPE;
                    have = 0;
                } else if (payload_len == 0) {
                    send_to_controller(frame, have);
                    state = WANT_TYPE;
                    have = 0;
                } else {
                    state = WANT_PAYLOAD;
                }
                break;

            case WANT_PAYLOAD:
                frame[have++] = b;
                if (have == 1 + hdr_len + payload_len) {
                    send_to_controller(frame, have);
                    state = WANT_TYPE;
                    have = 0;
                }
                break;
            }
        }
    }
}

esp_err_t hci_uart_init(void)
{
    s_tx = xStreamBufferCreate(TX_STREAM_BYTES, 1);
    ESP_RETURN_ON_FALSE(s_tx, ESP_ERR_NO_MEM, TAG, "tx stream buffer");

    s_can_send = xSemaphoreCreateBinary();
    ESP_RETURN_ON_FALSE(s_can_send, ESP_ERR_NO_MEM, TAG, "send semaphore");

    uart_config_t uart_cfg = {
        .baud_rate  = CONFIG_UNCHAINED_HCI_UART_BAUD,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_RETURN_ON_ERROR(uart_driver_install(HCI_UART_PORT, UART_RX_BUF, 0, 0, NULL, 0),
                        TAG, "uart driver");
    ESP_RETURN_ON_ERROR(uart_param_config(HCI_UART_PORT, &uart_cfg), TAG, "uart config");
    ESP_RETURN_ON_ERROR(uart_set_pin(HCI_UART_PORT, CONFIG_UNCHAINED_HCI_UART_TX_GPIO,
                                     CONFIG_UNCHAINED_HCI_UART_RX_GPIO,
                                     UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE), TAG, "uart pins");

    ESP_RETURN_ON_ERROR(esp_vhci_host_register_callback(&s_vhci_cb), TAG, "vhci callback");

    ESP_RETURN_ON_FALSE(xTaskCreate(hci_uart_tx_task, "hci_tx", 3072, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "tx task");
    ESP_RETURN_ON_FALSE(xTaskCreate(hci_uart_rx_task, "hci_rx", 4096, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "rx task");

    ESP_LOGI(TAG, "HCI H4 bridged to UART%d @ %d baud",
             HCI_UART_PORT, CONFIG_UNCHAINED_HCI_UART_BAUD);
    return ESP_OK;
}

#else /* !CONFIG_UNCHAINED_HCI_OVER_UART */

esp_err_t hci_uart_init(void) { return ESP_OK; }

#endif
