/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over the chip's native USB-Serial/JTAG transport.
 * This is the chip's own serial peripheral, not a USB Bluetooth device class,
 * so the board enumerates as a serial port and needs no external adapter.
 */

#ifndef HCI_USB_SERIAL_JTAG_H
#define HCI_USB_SERIAL_JTAG_H

#include "esp_err.h"

/*
 * Transport initialization.
 * Must be called after esp_bt_controller_init().
 */
esp_err_t hci_usb_serial_jtag_init(void);

#endif /* HCI_USB_SERIAL_JTAG_H */
