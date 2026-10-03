/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C3. The opcodes and their
 * contracts are shared across targets (unchained_vsc.h); this header adds only the
 * C3 dispatch glue. We do NOT own the whole vendor group here: only our own opcodes
 * are intercepted; every other opcode chains through to the controller unchanged.
 */

#ifndef VSC_H
#define VSC_H

#include <stdint.h>

#include "esp32c3_bt_rom.h"
#include "unchained_vsc.h"

/* True when opcode is one of ours (so the hci_cmd_received hook routes it here
 * instead of chaining the controller). */
_Bool vsc_owns(uint16_t opcode);

/*
 * The command descriptor the controller needs to pack a Command Complete for one
 * of our opcodes; NULL for anything we do not implement. Without it r_hci_build_cc_evt
 * finds no descriptor and overwrites our status byte with 0x01 "Unknown HCI Command".
 */
esp32c3_hci_cmd_desc_t *vsc_cmd_desc(uint16_t opcode);

/* Handle one of our vendor commands: do the work and send the Command Complete.
 * Mirrors the controller hci_cmd_received signature. */
void vsc_cmd_received(uint16_t opcode, uint16_t length, const uint8_t *payload);

#endif /* VSC_H */
