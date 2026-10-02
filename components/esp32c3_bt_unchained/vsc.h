/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C3.
 */

#ifndef VSC_H
#define VSC_H

#include <stdint.h>

#include "esp32c3_bt_rom.h"

/*
 * Our vendor-specific commands. The opcodes match the classic ESP32 and the C5
 * build so one host tool drives every target. We do NOT own the whole vendor
 * group here: the C3 also enables the stock Espressif vendor commands, so only
 * our own opcodes are intercepted and the rest pass through to the controller.
 * Each replies with a Command Complete whose first return byte is status.
 *
 * INFO            0xFC00  in: nothing; out: fw name, fw version, board name,
 *                         each a uint8 length followed by that many bytes.
 * SUPPORTED_CMDS  0xFC01  in: nothing; out: little-endian uint64, bit N set when
 *                         the command with OCF N is implemented.
 * SET_BDADDR      0xFC02  in: 6-byte BD_ADDR; out: nothing.
 */
#define UNCHAINED_VS_INFO_OCF 0x000           /* 0xFC00 */
#define UNCHAINED_VS_SUPPORTED_CMDS_OCF 0x001 /* 0xFC01 */
#define UNCHAINED_VS_SET_BDADDR_OCF 0x002     /* 0xFC02 */

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
