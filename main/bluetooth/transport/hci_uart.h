/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over a hardware UART transport.
 * This requires an external UART to USB on a board to connect to a PC.
 */

#ifndef HCI_UART_H
#define HCI_UART_H

#include "esp_err.h"

/*
 * Transport initialization.
 * Must be called after esp_bt_controller_init().
 */
esp_err_t hci_uart_init(void);

#endif /* HCI_UART_H */
