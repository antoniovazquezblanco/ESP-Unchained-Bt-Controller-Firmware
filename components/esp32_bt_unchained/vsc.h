/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the classic ESP32. The opcodes and their
 * contracts are shared across targets (unchained_vsc.h); this header adds only the
 * ESP32 dispatch glue. We own the whole vendor group: our opcodes are handled here
 * and every other vendor opcode answers "Unknown HCI Command".
 */

#ifndef VSC_H
#define VSC_H

#include <stdint.h>

#include "esp32_bt_rom.h"
#include "unchained_vsc.h"

/*
 * The command descriptor the ROM needs to pack a Command Complete for one of our
 * opcodes; NULL for anything we do not implement.
 */
hci_cmd_desc_t *vsc_cmd_desc(uint16_t opcode);

/* Handle one vendor-group command. Mirrors the ROM hci_cmd_received signature. */
void vsc_cmd_received(uint16_t opcode, uint8_t length, uint8_t *payload);

#endif /* VSC_H */
