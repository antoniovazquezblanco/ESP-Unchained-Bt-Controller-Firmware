/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set.
 */

#ifndef VSC_H
#define VSC_H

#include <stdint.h>

#include "esp32_bt_rom.h"
#include "hci.h"

/*
 * Our vendor-specific commands, numbered from the base of the vendor group up.
 * We own the whole group: these are handled here, every other vendor opcode
 * answers "Unknown HCI Command". Each replies with a standard Command Complete
 * whose first return byte is status.
 */

/*
 * INFO: identify this build.
 *
 * in:  nothing
 * out: status, then firmware name, firmware version and board name, each a
 *      uint8 length followed by that many bytes.
 */
#define UNCHAINED_VS_INFO_OCF 0x000
#define UNCHAINED_VS_INFO_OPCODE HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_INFO_OCF) /* 0xFC00 */

/*
 * SUPPORTED_CMDS: report which vendor commands this build implements.
 *
 * in:  nothing
 * out: status, then a little-endian uint64 bitfield: bit N is set when the
 *      command with OCF N is implemented, so INFO is bit 0. Commands past OCF
 *      63 are not representable and are reported as absent.
 */
#define UNCHAINED_VS_SUPPORTED_CMDS_OCF 0x001
#define UNCHAINED_VS_SUPPORTED_CMDS_OPCODE HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SUPPORTED_CMDS_OCF) /* 0xFC01 */

/*
 * SET_BDADDR: override the controller public address.
 *
 * in:  6-byte BD_ADDR
 * out: status; INVALID_PARAMS if fewer than 6 bytes were given.
 */
#define UNCHAINED_VS_SET_BDADDR_OCF 0x002
#define UNCHAINED_VS_SET_BDADDR_OPCODE HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_BDADDR_OCF) /* 0xFC02 */

/*
 * The command descriptor the ROM needs to pack a Command Complete for one of our
 * opcodes; NULL for anything we do not implement.
 */
hci_cmd_desc_t *vsc_cmd_desc(uint16_t opcode);

/* Handle one vendor-group command. Mirrors the ROM hci_cmd_received signature. */
void vsc_cmd_received(uint16_t opcode, uint8_t length, uint8_t *payload);

#endif /* VSC_H */
