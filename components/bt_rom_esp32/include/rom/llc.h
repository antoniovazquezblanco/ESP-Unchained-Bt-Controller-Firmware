/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 BLE link controller (LLC) per-connection environment pointers.
 */

#ifndef ROM_LLC_H
#define ROM_LLC_H

#include <stdint.h>

/* Maximum simultaneous BLE connections (llc_env is a 10-pointer array, 0x28 B). */
#define BT_LE_CONN_MAX 10

/* Largest raw LLCP PDU r_llc_llcp_tester_send stages (its heap buffer is 26). */
#define LLC_LLCP_PDU_MAX 26

/*
 * Inject one raw LLCP (control) PDU: the ROM copies exactly len bytes from pdu
 * into a heap buffer, tags the TX element raw (length in the element, no opcode
 * pack), and pushes it on the connection's LLCP TX list, so precisely those bytes
 * go on air as an LLID-3 control PDU. This is the controller's own DTM path for
 * sending a test LLCP, so arbitrary opcodes and malformed PDUs go out verbatim --
 * unlike the per-opcode senders, which re-pack from a struct. (The data-send path
 * forces a data LLID, so this is the route for control injection.) It does not
 * validate the link -- the caller must ensure llc_env[conhdl] is active. A stable
 * esp32.rom.ld symbol.
 */
void r_llc_llcp_tester_send(uint16_t conhdl, uint8_t len, const uint8_t *pdu);

/*
 * Per-connection BLE link-controller environments (llc_env, absolute ROM-data
 * symbol esp32.rom.ld). Indexed by the connection handle; an inactive slot is
 * NULL. The LL data/control send path (lld_pdu_data_send) indexes this by handle
 * with no bounds check, so an injector validates the target here (slot non-NULL,
 * handle in range) before submitting a PDU.
 */
extern void *llc_env[BT_LE_CONN_MAX];

#endif /* ROM_LLC_H */
