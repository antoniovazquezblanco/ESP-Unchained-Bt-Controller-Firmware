/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 classic (BR/EDR) Bluetooth controller ROM glue.
 */

#include <stddef.h>

#include "esp32_bt_rom.h"

bool esp32_bt_rom_selftest(void)
{
    return r_ip_funcs_p != NULL;
}
