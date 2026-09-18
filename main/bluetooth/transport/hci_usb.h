/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI (H4) over the chip's native USB-Serial/JTAG.
 */
#ifndef HCI_USB_H
#define HCI_USB_H

#include "esp_err.h"

/*
 * Bridge the controller's VHCI interface to the USB-Serial/JTAG endpoint, so
 * the board is a plain H4 HCI controller over its USB socket. Call once, after
 * the controller has been enabled.
 *
 * Internal to transport/: compiled only when this bridge is the selected
 * transport, and reached through bluetooth_transport_init().
 */
esp_err_t hci_usb_init(void);

#endif /* HCI_USB_H */
