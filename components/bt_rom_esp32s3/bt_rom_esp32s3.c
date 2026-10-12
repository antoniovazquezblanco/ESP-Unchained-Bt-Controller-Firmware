/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-S3 BLE controller ROM glue.
 */

#include <stdint.h>

#include "bt_rom_esp32s3.h"

bool bt_rom_esp32s3_selftest(void)
{
    /* r_hci_cmd_received is an absolute symbol from esp32s3.rom.bt_funcs.ld; its
     * address is the ROM entry point. Confirm it linked into the ROM window. */
    uintptr_t addr = (uintptr_t)&r_hci_cmd_received;
    return addr >= 0x40000000u && addr < 0x40100000u;
}
