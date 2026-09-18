/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over the chip's native USB-Serial/JTAG transport.
 * This is the chip's own serial peripheral, not a USB Bluetooth device class,
 * so the board enumerates as a serial port and needs no external adapter.
 */

#include "hci_usb_serial_jtag.h"

#include "sdkconfig.h"
#include "driver/usb_serial_jtag.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"

#include "vhci_bridge.h"

/*
 * HCI owns the USB-Serial/JTAG endpoint.
 * Anything else writing there causes faults and desynchronises the stream.
 * Check that the configuration is correct and nothing else writes to this
 * USB Serial/Jtag endpoint.
 */
#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
#error "HCI is bridged to USB-Serial/JTAG but the primary console is on it too. Set the console elsewhere, or use other HCI transport instead."
#endif
#if CONFIG_ESP_CONSOLE_SECONDARY_USB_SERIAL_JTAG
#error "HCI is bridged to USB-Serial/JTAG but the secondary console is on it too. Set CONFIG_ESP_CONSOLE_SECONDARY_NONE, or use other HCI transport instead."
#endif

static const char *TAG = "HCIUSJ";

#define USB_RX_BUF      1024
#define USB_TX_BUF      2048

static int hci_usb_serial_jtag_read(uint8_t *buf, size_t len)
{
    /* Returns as soon as anything is buffered, which is the bridge contract. */
    return usb_serial_jtag_read_bytes(buf, len, portMAX_DELAY);
}

static int hci_usb_serial_jtag_write(const uint8_t *buf, size_t len)
{
    return usb_serial_jtag_write_bytes(buf, len, portMAX_DELAY);
}

static const vhci_bridge_io_t s_io = {
    .name  = "USB-Serial/JTAG",
    .read  = hci_usb_serial_jtag_read,
    .write = hci_usb_serial_jtag_write,
};

esp_err_t hci_usb_serial_jtag_init(void)
{
    usb_serial_jtag_driver_config_t usb_cfg = {
        .tx_buffer_size = USB_TX_BUF,
        .rx_buffer_size = USB_RX_BUF,
    };
    ESP_RETURN_ON_ERROR(usb_serial_jtag_driver_install(&usb_cfg), TAG, "usb driver");

    return vhci_bridge_start(&s_io);
}
