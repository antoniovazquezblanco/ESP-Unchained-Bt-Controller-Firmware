/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 BLE controller model: the aggregate header.
 *
 * The C3 ships a large slice of the RivieraWaves BLE stack in mask ROM, reached
 * through writable function-pointer tables and RAM env structs (exported as
 * absolute symbols by esp32c3.rom.ld), and it works through exchange memory. This
 * component models the slices the unchained layer builds on, split like the
 * classic bt_rom_esp32:
 *
 *   hal/  the memory-mapped hardware -- exchange memory (em.h), gathered by
 *         hal/hal.h; no core radio registers modelled yet.
 *   rom/  the ROM software -- the dispatch tables (ip_funcs.h), the HCI surface
 *         (hci.h), the link-layer environments (lld.h), and the sdk-config
 *         (sdk_config.h), gathered by rom/rom.h.
 *
 * Nothing here changes controller behaviour, it only describes.
 */

#ifndef BT_ROM_ESP32C3_H
#define BT_ROM_ESP32C3_H

#include <stdbool.h>

#include "hal/hal.h"
#include "rom/rom.h"

/**
 * Self-test: confirm the ROM BT symbols resolved into the C3 ROM address window.
 */
bool bt_rom_esp32c3_selftest(void);

#endif /* BT_ROM_ESP32C3_H */
