/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-S3 BLE controller model: the aggregate header.
 *
 * The S3 ships a large slice of the RivieraWaves BLE stack in mask ROM, reached
 * through writable function-pointer tables and RAM env structs (exported as
 * absolute symbols by esp32s3.rom.ld), and it works through exchange memory. The
 * S3 runs the same controller build as the C3 (ESP-IDF maps its BT sources to
 * esp32c3), so this model mirrors bt_rom_esp32c3 at the S3 ROM addresses; only the
 * symbol addresses differ. This component models the slices the unchained layer
 * builds on, split like the classic bt_rom_esp32:
 *
 *   hal/  the memory-mapped hardware -- exchange memory (em.h), gathered by
 *         hal/hal.h; no core radio registers modelled yet.
 *   rom/  the ROM software -- the dispatch tables (ip_funcs.h), the HCI surface
 *         (hci.h), the link-layer environments (lld.h), and the sdk-config
 *         (sdk_config.h), gathered by rom/rom.h.
 *
 * Nothing here changes controller behaviour, it only describes.
 */

#ifndef BT_ROM_ESP32S3_H
#define BT_ROM_ESP32S3_H

#include <stdbool.h>

#include "hal/hal.h"
#include "rom/rom.h"

/**
 * Self-test: confirm the ROM BT symbols resolved into the S3 ROM address window.
 */
bool bt_rom_esp32s3_selftest(void);

#endif /* BT_ROM_ESP32S3_H */
