/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * The PDU-injection wire protocol, shared by every target.
 *
 * INJECT_PDU (0xFC05) transmits one attacker-crafted link-layer PDU on an active
 * connection the controller owns. It is the write twin of the traffic monitor:
 * the LL PDU is encoded exactly as the 0xFF capture event carries it
 * (capture_event.h), so a captured PDU can be replayed, tweaked or malformed.
 * One host tool drives every target; each target's pdu_inject.c maps this onto
 * its controller (RivieraWaves ROM on the ESP32/C3/S3, NimBLE on the C5).
 *
 * Parameter layout (the 0xFC05 command parameters):
 *
 *   [0]      kind (PDU_INJECT_KIND_*)
 *   [1..2]   link id / connection handle, little-endian. On the C5 this is the
 *            HCI connection handle; on the ROM targets it is the controller link
 *            id (low byte), which the host learns from a TX capture event. 0xFF
 *            RX captures carry no link id, so source it from the TX side.
 *   [3..]    the PDU (kind-specific, below)
 *
 * KIND_LL -- the PDU mirrors the capture event's LL body:
 *   [3]      LLID: 1 data continuation, 2 data start, 3 control (LLCP)
 *   [4]      length
 *   [5..]    payload (for control, the LLCP opcode first)
 *
 * KIND_LMP -- the PDU is the raw LMP bytes, opcode first (classic ESP32 only).
 */

#ifndef PDU_INJECT_H
#define PDU_INJECT_H

/* Injection kind (parameter byte 0). */
#define PDU_INJECT_KIND_LMP 0x01 /* BR/EDR LMP (classic ESP32 only) */
#define PDU_INJECT_KIND_LL 0x02  /* BLE LL (data or LLCP)           */

/* LL PDU LLID (KIND_LL, PDU byte 0), the on-air Link Layer ID. */
#define PDU_INJECT_LLID_DATA_CONT 0x01  /* data PDU, continuation fragment */
#define PDU_INJECT_LLID_DATA_START 0x02 /* data PDU, start / complete      */
#define PDU_INJECT_LLID_CONTROL 0x03    /* control PDU (LLCP)              */

/* Fixed offsets into the command parameters. */
#define PDU_INJECT_OFF_KIND 0
#define PDU_INJECT_OFF_HANDLE 1 /* little-endian uint16 */
#define PDU_INJECT_OFF_PDU 3
#define PDU_INJECT_HDR_LEN 3 /* kind + handle, before the PDU */

/* LL PDU sub-header (within the PDU, KIND_LL). */
#define PDU_INJECT_LL_OFF_LLID 0
#define PDU_INJECT_LL_OFF_LENGTH 1
#define PDU_INJECT_LL_OFF_PAYLOAD 2
#define PDU_INJECT_LL_HDR_LEN 2 /* llid + length, before the payload */

#endif /* PDU_INJECT_H */
