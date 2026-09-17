/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI (H4) over the chip's native USB-Serial/JTAG.
 *
 * The controller offers two HCI transports (ESP-IDF BT_LE_HCI_INTERFACE): a
 * hardware UART, or VHCI -- an in-RAM interface the application pumps itself.
 * A board whose only socket is the chip's USB-Serial/JTAG cannot use the UART
 * without an external adapter, so this bridges VHCI to that endpoint: bytes in
 * are reassembled into HCI packets and handed to the controller, packets out
 * are streamed back. The result is an ordinary H4 controller over USB.
 */
#include "hci_usb.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG

#include "driver/usb_serial_jtag.h"
#include "esp_bt.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"
#include "freertos/task.h"

/* HCI owns the USB-Serial/JTAG endpoint. Anything else writing there
 * interleaves its bytes with HCI packets and desynchronises the H4 stream, so
 * a console on the same endpoint is a build error rather than a device that
 * enumerates fine and then misbehaves under load. A board that wants its
 * console back on USB should drive HCI over the controller's own UART
 * (CONFIG_BT_LE_HCI_INTERFACE_USE_UART), which disables this bridge. */
#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
#error "HCI is bridged to USB-Serial/JTAG but the primary console is on it too. Set the console elsewhere, or use the controller's HCI UART instead."
#endif
#if CONFIG_ESP_CONSOLE_SECONDARY_USB_SERIAL_JTAG
#error "HCI is bridged to USB-Serial/JTAG but the secondary console is on it too. Set CONFIG_ESP_CONSOLE_SECONDARY_NONE, or use the controller's HCI UART instead."
#endif

static const char *TAG = "HCIUSB";

/* H4 packet-type indicators. */
#define H4_CMD  0x01
#define H4_ACL  0x02
#define H4_SCO  0x03
#define H4_ISO  0x05

/* Largest payload reassembled from the host. An HCI command tops out at 255
 * and LE ACL data is far smaller, so this is headroom, not a real limit. A
 * length beyond it means the stream is out of sync rather than genuinely big. */
#define H4_MAX_PAYLOAD  1024
#define H4_MAX_HDR      4
#define H4_MAX_FRAME    (1 + H4_MAX_HDR + H4_MAX_PAYLOAD)

#define USB_RX_BUF      1024
#define USB_TX_BUF      2048
#define TX_STREAM_BYTES 4096

static StreamBufferHandle_t s_tx;        /* controller -> USB */
static SemaphoreHandle_t    s_can_send;  /* controller is ready for a packet */

/* ---- controller -> host ------------------------------------------------- */

static void notify_host_send_available(void)
{
    xSemaphoreGive(s_can_send);
}

static int notify_host_recv(uint8_t *data, uint16_t len)
{
    /* Called in controller context, so it must not block. A partial write
     * would desynchronise the H4 stream for good, so a packet that does not
     * fit whole is dropped whole and the stream stays well-formed. */
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

static void hci_usb_tx_task(void *arg)
{
    uint8_t buf[256];
    for (;;) {
        size_t n = xStreamBufferReceive(s_tx, buf, sizeof(buf), portMAX_DELAY);
        /* Blocking: if no host is reading, back-pressure belongs here, where it
         * costs a stalled task, rather than in the controller callback. */
        for (size_t off = 0; off < n; ) {
            int w = usb_serial_jtag_write_bytes(buf + off, n - off, portMAX_DELAY);
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
    /* The controller takes one packet at a time and calls back when it can
     * take the next. */
    while (!esp_vhci_host_check_send_available()) {
        if (xSemaphoreTake(s_can_send, pdMS_TO_TICKS(1000)) != pdTRUE) {
            ESP_LOGW(TAG, "controller still not accepting packets");
        }
    }
    esp_vhci_host_send_packet(frame, (uint16_t)len);
}

static void hci_usb_rx_task(void *arg)
{
    static uint8_t frame[H4_MAX_FRAME];
    uint8_t chunk[128];
    size_t have = 0, hdr_len = 0, payload_len = 0;
    enum { WANT_TYPE, WANT_HDR, WANT_PAYLOAD } state = WANT_TYPE;

    for (;;) {
        int n = usb_serial_jtag_read_bytes(chunk, sizeof(chunk), portMAX_DELAY);
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

esp_err_t hci_usb_init(void)
{
    s_tx = xStreamBufferCreate(TX_STREAM_BYTES, 1);
    ESP_RETURN_ON_FALSE(s_tx, ESP_ERR_NO_MEM, TAG, "tx stream buffer");

    s_can_send = xSemaphoreCreateBinary();
    ESP_RETURN_ON_FALSE(s_can_send, ESP_ERR_NO_MEM, TAG, "send semaphore");

    usb_serial_jtag_driver_config_t usb_cfg = {
        .tx_buffer_size = USB_TX_BUF,
        .rx_buffer_size = USB_RX_BUF,
    };
    ESP_RETURN_ON_ERROR(usb_serial_jtag_driver_install(&usb_cfg), TAG, "usb driver");

    ESP_RETURN_ON_ERROR(esp_vhci_host_register_callback(&s_vhci_cb), TAG, "vhci callback");

    ESP_RETURN_ON_FALSE(xTaskCreate(hci_usb_tx_task, "hci_tx", 3072, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "tx task");
    ESP_RETURN_ON_FALSE(xTaskCreate(hci_usb_rx_task, "hci_rx", 4096, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "rx task");

    ESP_LOGI(TAG, "HCI H4 bridged to USB-Serial/JTAG");
    return ESP_OK;
}

#else /* !CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG */

esp_err_t hci_usb_init(void) { return ESP_OK; }

#endif
