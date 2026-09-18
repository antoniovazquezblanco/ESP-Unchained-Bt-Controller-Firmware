/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth controller bring-up.
 */
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_err.h"

/*
 * Bring up the Bluetooth controller and its HCI transport.
 * Failures are logged here and ESP_OK means the controller is enabled and HCI is reachable.
 */
esp_err_t bluetooth_init(void);

#endif /* BLUETOOTH_H */
