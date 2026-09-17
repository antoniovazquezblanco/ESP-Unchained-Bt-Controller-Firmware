/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI (H4) over a hardware UART.
 */
#ifndef HCI_UART_H
#define HCI_UART_H

#include "esp_err.h"

/*
 * Bridge the controller's VHCI interface to a hardware UART, so the board is a
 * plain H4 HCI controller over that UART. For chips without native
 * USB-Serial/JTAG (e.g. the classic ESP32, reached through its onboard USB-UART
 * bridge on UART0). Call once, after the controller has been enabled. A no-op
 * unless CONFIG_UNCHAINED_HCI_OVER_UART.
 */
esp_err_t hci_uart_init(void);

#endif /* HCI_UART_H */
