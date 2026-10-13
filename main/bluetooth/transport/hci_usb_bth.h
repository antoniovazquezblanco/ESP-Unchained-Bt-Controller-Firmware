/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over the chip's USB-OTG as a standard USB Bluetooth device (BTH class).
 */

#ifndef HCI_USB_BTH_H
#define HCI_USB_BTH_H

#include "esp_err.h"

esp_err_t hci_usb_bth_init(void);

#endif /* HCI_USB_BTH_H */
