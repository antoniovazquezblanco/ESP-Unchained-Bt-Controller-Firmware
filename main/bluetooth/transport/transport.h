/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI transport setup.
 */

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "esp_err.h"

/*
 * Transport initialization.
 * Must be called after esp_bt_controller_init().
 */
esp_err_t transport_init(void);

#endif /* TRANSPORT_H */
