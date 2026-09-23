/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * A generic bridge between the controller virtual HCI interface and
 * a byte stream interface represented as a write and read function pair.
 */

#include "vhci_bridge.h"

#include "esp_bt.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"
#include "freertos/task.h"

#include "hci.h"

static const char *TAG = "VHCI";

/* Largest payload reassembled from the host. An HCI command tops out at 255 and
 * LE ACL data is far smaller, so this is headroom, not a real limit. A length
 * beyond it means the stream is out of sync rather than genuinely big. */
#define HCI_MAX_PAYLOAD 1024
#define HCI_MAX_HDR 4
#define HCI_MAX_FRAME (1 + HCI_MAX_HDR + HCI_MAX_PAYLOAD)

#define TX_STREAM_BYTES 4096

static const vhci_bridge_io_t *s_io; /* the transport we were handed */
static StreamBufferHandle_t s_tx;    /* controller -> transport */
static SemaphoreHandle_t s_can_send; /* controller is ready for a packet */

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
    if (xStreamBufferSpacesAvailable(s_tx) < len)
    {
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

static void vhci_bridge_tx_task(void *arg)
{
    uint8_t buf[256];
    for (;;)
    {
        size_t n = xStreamBufferReceive(s_tx, buf, sizeof(buf), portMAX_DELAY);
        /* Blocking write: if the host is not reading, the back-pressure lands
         * here, on a stalled task, not in the controller. */
        for (size_t off = 0; off < n;)
        {
            int w = s_io->write(buf + off, n - off);
            if (w <= 0)
            {
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
    switch (type)
    {
    case HCI_PKT_CMD:
        return HCI_CMD_HDR_SIZE;
    case HCI_PKT_ACL:
        return HCI_ACL_HDR_SIZE;
    case HCI_PKT_SCO:
        return HCI_SCO_HDR_SIZE;
    case HCI_PKT_ISO:
        return HCI_ISO_HDR_SIZE;
    default:
        return 0;
    }
}

static size_t h4_payload_len(uint8_t type, const uint8_t *hdr)
{
    switch (type)
    {
    case HCI_PKT_CMD:
        return hdr[2];
    case HCI_PKT_SCO:
        return hdr[2];
    case HCI_PKT_ACL:
        return (size_t)hdr[2] | ((size_t)hdr[3] << 8);
    case HCI_PKT_ISO:
        return ((size_t)hdr[2] | ((size_t)hdr[3] << 8)) & HCI_ISO_LEN_MASK;
    default:
        return 0;
    }
}

static void send_to_controller(uint8_t *frame, size_t len)
{
    /* The controller takes one packet at a time and calls back when it can take
     * the next. */
    while (!esp_vhci_host_check_send_available())
    {
        if (xSemaphoreTake(s_can_send, pdMS_TO_TICKS(1000)) != pdTRUE)
        {
            ESP_LOGW(TAG, "controller still not accepting packets");
        }
    }
    esp_vhci_host_send_packet(frame, (uint16_t)len);
}

static void vhci_bridge_rx_task(void *arg)
{
    static uint8_t frame[HCI_MAX_FRAME];
    uint8_t chunk[128];
    size_t have = 0, hdr_len = 0, payload_len = 0;
    enum
    {
        WANT_TYPE,
        WANT_HDR,
        WANT_PAYLOAD
    } state = WANT_TYPE;

    for (;;)
    {
        int n = s_io->read(chunk, sizeof(chunk));
        if (n <= 0)
        {
            continue;
        }

        for (int i = 0; i < n; i++)
        {
            uint8_t b = chunk[i];

            switch (state)
            {
            case WANT_TYPE:
                /* Anything that is not a packet-type indicator is noise from a
                 * terminal probing the port; stay here until the stream makes
                 * sense again. */
                hdr_len = h4_header_len(b);
                if (hdr_len == 0)
                {
                    continue;
                }
                frame[0] = b;
                have = 1;
                state = WANT_HDR;
                break;

            case WANT_HDR:
                frame[have++] = b;
                if (have < 1 + hdr_len)
                {
                    break;
                }
                payload_len = h4_payload_len(frame[0], frame + 1);
                if (payload_len > HCI_MAX_PAYLOAD)
                {
                    ESP_LOGW(TAG, "type 0x%02x claims %u bytes, resyncing",
                             frame[0], (unsigned)payload_len);
                    state = WANT_TYPE;
                    have = 0;
                }
                else if (payload_len == 0)
                {
                    send_to_controller(frame, have);
                    state = WANT_TYPE;
                    have = 0;
                }
                else
                {
                    state = WANT_PAYLOAD;
                }
                break;

            case WANT_PAYLOAD:
                frame[have++] = b;
                if (have == 1 + hdr_len + payload_len)
                {
                    send_to_controller(frame, have);
                    state = WANT_TYPE;
                    have = 0;
                }
                break;
            }
        }
    }
}

esp_err_t vhci_bridge_start(const vhci_bridge_io_t *io)
{
    ESP_RETURN_ON_FALSE(io && io->read && io->write, ESP_ERR_INVALID_ARG, TAG, "io");
    s_io = io;

    s_tx = xStreamBufferCreate(TX_STREAM_BYTES, 1);
    ESP_RETURN_ON_FALSE(s_tx, ESP_ERR_NO_MEM, TAG, "tx stream buffer");

    s_can_send = xSemaphoreCreateBinary();
    ESP_RETURN_ON_FALSE(s_can_send, ESP_ERR_NO_MEM, TAG, "send semaphore");

    ESP_RETURN_ON_ERROR(esp_vhci_host_register_callback(&s_vhci_cb), TAG, "vhci callback");

    ESP_RETURN_ON_FALSE(xTaskCreate(vhci_bridge_tx_task, "hci_tx", 3072, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "tx task");
    ESP_RETURN_ON_FALSE(xTaskCreate(vhci_bridge_rx_task, "hci_rx", 4096, NULL, 12, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "rx task");

    ESP_LOGI(TAG, "HCI H4 bridged to %s", io->name);
    return ESP_OK;
}
