/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 Bluetooth controller (BR/EDR + BLE) ROM glue.
 */

#include <stddef.h>

#include "bt_rom_esp32.h"

bool bt_rom_esp32_selftest(void)
{
    return r_ip_funcs_p != NULL;
}
