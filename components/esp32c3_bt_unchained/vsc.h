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
 * group here: only our own opcodes are intercepted; every other opcode chains
 * through to the controller unchanged. Each replies with a Command Complete whose
 * first return byte is status.
 *
 * INFO            0xFC00  in: nothing; out: fw name, fw version, board name,
 *                         each a uint8 length followed by that many bytes.
 * SUPPORTED_CMDS  0xFC01  in: nothing; out: little-endian uint64, bit N set when
 *                         the command with OCF N is implemented.
 * SET_BDADDR      0xFC02  in: 6-byte BD_ADDR; out: nothing.
 * SET_TRAFFIC_MONITOR 0xFC03  in: 1-byte flag bitmask (TRAFFIC_MONITOR_* in
 *                         traffic_monitor.h), 0 disables; out: nothing.
 * SET_SCAN_CHANNEL 0xFC04  in: 1-byte channel (37/38/39 to pin, 0 to restore the
 *                         three-channel hop); out: nothing.
 */
#define UNCHAINED_VS_INFO_OCF 0x000                /* 0xFC00 */
#define UNCHAINED_VS_SUPPORTED_CMDS_OCF 0x001      /* 0xFC01 */
#define UNCHAINED_VS_SET_BDADDR_OCF 0x002          /* 0xFC02 */
#define UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF 0x003 /* 0xFC03 */
#define UNCHAINED_VS_SET_SCAN_CHANNEL_OCF 0x004    /* 0xFC04 */

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
