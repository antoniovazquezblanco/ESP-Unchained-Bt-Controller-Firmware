/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over the chip's native USB-OTG as a standard USB Bluetooth device (BTH
 * class, Wireless Controller 0xE0/01/01). The board enumerates as an ordinary
 * Bluetooth dongle: Linux btusb binds it as an hciX adapter with no H4 attach.
 *
 * Unlike the H4 transports there is no framing on the wire -- the packet type is
 * the endpoint. TinyUSB hands us HCI commands (control endpoint) and host ACL
 * (bulk OUT) through the tud_bt_* callbacks; we send events (interrupt IN) and
 * controller ACL (bulk IN) back with tud_bt_event_send / tud_bt_acl_data_send.
 * The VHCI side is the controller's H4-framed interface, so this only strips and
 * re-adds the one-byte packet type as it crosses between the two.
 */

#include "hci_usb_bth.h"

#include <string.h>

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "tinyusb.h"
#include "tinyusb_default_config.h"
#include "tusb.h"

#include "hci.h"
#include "vhci_bridge.h"

static const char *TAG = "HCIBTH";

/* Largest controller->host packet we re-emit: an HCI event (<= 257) or an ACL
 * PDU; sized to the bridge's reassembly cap so a copy never clips. */
#define BTH_MAX_PKT 1028

/* Endpoint addresses: interrupt IN for events, bulk IN/OUT for ACL data. The SCO
 * interface's isochronous endpoints are derived by TUD_BTH_DESCRIPTOR as the next
 * addresses (0x83 IN / 0x03 OUT); unused, since the controller is BLE-only. */
#define BTH_EP_EVENT 0x81
#define BTH_EP_ACL_IN 0x82
#define BTH_EP_ACL_OUT 0x02

/* String descriptor indices. */
#define BTH_STR_MANUFACTURER 1
#define BTH_STR_PRODUCT 2
#define BTH_STR_SERIAL 3
#define BTH_STR_INTERFACE 4

/* Device descriptor. The device class is the Wireless/Bluetooth triple so a
 * generic host driver (Linux btusb) binds it without a per-device match. */
static const tusb_desc_device_t s_device_desc = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUD_BT_APP_CLASS,
    .bDeviceSubClass = TUD_BT_APP_SUBCLASS,
    .bDeviceProtocol = TUD_BT_PROTOCOL_PRIMARY_CONTROLLER,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A,  /* Espressif */
    .idProduct = 0x4200, /* project-specific product id */
    .bcdDevice = 0x0101,
    .iManufacturer = BTH_STR_MANUFACTURER,
    .iProduct = BTH_STR_PRODUCT,
    .iSerialNumber = BTH_STR_SERIAL,
    .bNumConfigurations = 1,
};

/*
 * Configuration descriptor: the primary BTH interface (interface 0: event +
 * two ACL endpoints) plus the voice/SCO interface (interface 1) the USB BT spec
 * defines, with its six standard alternate settings (isochronous packet sizes
 * 0/9/17/25/33/49). The controller is BLE-only so SCO is never used, but
 * Microsoft's bthusb.sys will not start the radio without the interface.
 *
 * This is laid out from TinyUSB's interface sub-macros rather than the top-level
 * TUD_BTH_DESCRIPTOR because that one prepends an Interface Association Descriptor
 * a real Bluetooth dongle does not have, which Microsoft's bthusb.sys rejects.
 * CFG_TUD_BTH_ISO_ALT_COUNT (6) must match the number of isochronous sizes.
 */
#define BTH_PRI_ITF_LEN (9 + 7 + 7 + 7)
#define BTH_ISO_ITF_LEN (9 + 7 + 7)
#define BTH_CONFIG_TOTAL_LEN \
    (TUD_CONFIG_DESC_LEN + BTH_PRI_ITF_LEN + CFG_TUD_BTH_ISO_ALT_COUNT * BTH_ISO_ITF_LEN)

static const uint8_t s_fs_config_desc[] = {
    TUD_CONFIG_DESCRIPTOR(1, 2, 0, BTH_CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_BTH_PRI_ITF(0, BTH_STR_INTERFACE, BTH_EP_EVENT, CFG_TUD_BTH_EVENT_EPSIZE, 1,
                    BTH_EP_ACL_IN, BTH_EP_ACL_OUT, CFG_TUD_BTH_DATA_EPSIZE)
        TUD_BTH_ISO_ITFS(1, BTH_EP_ACL_IN + 1, BTH_EP_ACL_OUT + 1, 0, 9, 17, 25, 33, 49)};

static const char *s_strings[] = {
    (const char[]){0x09, 0x04}, /* 0: LANGID en-US */
    "Espressif",                /* 1: manufacturer */
    "ESP Unchained BT",         /* 2: product */
    "unchained",                /* 3: serial */
    "Bluetooth HCI",            /* 4: interface */
};

/* ---- controller -> host ------------------------------------------------- */

/* How long a packet waits for the previous IN transfer before we give up on it. */
#define BTH_TX_TIMEOUT pdMS_TO_TICKS(1000)

/* Taken while an IN transfer reads txbuf; given back by the sent callbacks. */
static SemaphoreHandle_t s_tx_idle;

/* Deliver one controller->host packet to its endpoint. The controller only ever
 * sends events and ACL upward. TinyUSB reads the buffer until the transfer's sent
 * callback, so wait for that before reusing txbuf; give up after a timeout (a bus
 * reset never completes it), so a stalled host cannot wedge the bridge task.
 *
 * The caller's buffer points one byte into the reassembled frame (the H4 type
 * byte was stripped), so it is unaligned. The USB-OTG (dwc2) reads the TX FIFO
 * source word-aligned and would otherwise send from the aligned byte before it,
 * so copy into a word-aligned buffer first. Safe as a single static: bth_send
 * runs only from the one bridge tx task. */
static int bth_send(uint8_t type, const uint8_t *data, size_t len)
{
    static uint8_t __attribute__((aligned(4))) txbuf[BTH_MAX_PKT];
    if (type != HCI_PKT_EVT && type != HCI_PKT_ACL) {
        return (int)len; /* nothing else is expected upward */
    }
    if (len > sizeof(txbuf)) {
        ESP_LOGW(TAG, "packet too large (%u bytes), dropped", (unsigned)len);
        return 0;
    }
    xSemaphoreTake(s_tx_idle, BTH_TX_TIMEOUT);
    memcpy(txbuf, data, len);

    bool sent = tud_mounted() && ((type == HCI_PKT_EVT) ? tud_bt_event_send(txbuf, (uint16_t)len)
                                                        : tud_bt_acl_data_send(txbuf, (uint16_t)len));
    if (!sent) {
        xSemaphoreGive(s_tx_idle);
        ESP_LOGW(TAG, "host not draining, dropped a %u byte packet", (unsigned)len);
        return 0;
    }
    return (int)len;
}

void tud_bt_event_sent_cb(uint16_t sent_bytes)
{
    xSemaphoreGive(s_tx_idle);
}

void tud_bt_acl_data_sent_cb(uint16_t sent_bytes)
{
    xSemaphoreGive(s_tx_idle);
}

static const vhci_bridge_pkt_io_t s_pkt_io = {
    .name = "USB Bluetooth (BTH)",
    .send = bth_send,
};

/* ---- host -> controller (TinyUSB callbacks) ----------------------------- */

void tud_bt_hci_cmd_cb(void *hci_cmd, size_t cmd_len)
{
    vhci_bridge_pkt_input(HCI_PKT_CMD, (const uint8_t *)hci_cmd, cmd_len);
}

void tud_bt_acl_data_received_cb(void *acl_data, uint16_t data_len)
{
    vhci_bridge_pkt_input(HCI_PKT_ACL, (const uint8_t *)acl_data, data_len);
}

esp_err_t hci_usb_bth_init(void)
{
    tinyusb_config_t tusb_cfg = TINYUSB_CONFIG_FULL_SPEED(NULL, NULL);
    tusb_cfg.descriptor.device = &s_device_desc;
    tusb_cfg.descriptor.string = s_strings;
    tusb_cfg.descriptor.string_count = sizeof(s_strings) / sizeof(s_strings[0]);
    tusb_cfg.descriptor.full_speed_config = s_fs_config_desc;

    s_tx_idle = xSemaphoreCreateBinary();
    ESP_RETURN_ON_FALSE(s_tx_idle != NULL, ESP_ERR_NO_MEM, TAG, "tx semaphore");
    xSemaphoreGive(s_tx_idle);

    ESP_RETURN_ON_ERROR(tinyusb_driver_install(&tusb_cfg), TAG, "tinyusb install");

    return vhci_bridge_start_pkt(&s_pkt_io);
}
