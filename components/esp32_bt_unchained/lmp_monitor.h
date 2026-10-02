/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor: reports controller LMP/LL PDUs to the host as
 * vendor-specific HCI events, driven by the SET_TRAFFIC_MONITOR command.
 */

#ifndef LMP_MONITOR_H
#define LMP_MONITOR_H

#include <stdint.h>

#include "rom/bt_em_buf.h"
#include "rom/hci_desc_tabs.h"

/*
 * Monitor flags, the SET_TRAFFIC_MONITOR parameter byte. BR/EDR LMP and BLE LL,
 * each direction, all have a hook behind them.
 */
#define LMP_MONITOR_LMP_TX 0x01 /* outgoing BR/EDR LMP PDUs            */
#define LMP_MONITOR_LMP_RX 0x02 /* incoming BR/EDR LMP PDUs            */
#define LMP_MONITOR_LL_TX 0x04  /* outgoing BLE LL data PDUs          */
#define LMP_MONITOR_LL_RX 0x08  /* incoming BLE LL PDUs               */

/*
 * LL_RX captures every incoming LL PDU (data and control); LL_TX captures
 * outgoing LL *data* only. Outgoing LL control (LLCP) is queued straight to
 * the baseband by llc_llcp_send, which is not one of the lld_pdu push slots we
 * can hook, so it does not pass our TX tap -- the peer's replies still show on
 * LL_RX.
 */
/* Everything a hook exists for; a request outside this is UNSUPPORTED_FEATURE. */
#define LMP_MONITOR_SUPPORTED (LMP_MONITOR_LMP_TX | LMP_MONITOR_LMP_RX | LMP_MONITOR_LL_TX | LMP_MONITOR_LL_RX)

/*
 * Capture event layout, carried in the 0xFF vendor event after code and length.
 *
 *   [0] subcode = LMP_MONITOR_EVT_SUBCODE, namespaces this under the 0xFF event
 *   [1] direction: 0 = TX (controller -> peer), 1 = RX
 *   [2] link id, or LMP_MONITOR_LINK_ID_UNKNOWN when the tap cannot supply one
 *   [3..6] controller clock at capture (little-endian uint32, 312.5 us ticks)
 *   [7] PDU length
 *   [8..] the raw LMP/LL PDU bytes
 */
#define LMP_MONITOR_EVT_SUBCODE 0x01
#define LMP_MONITOR_DIR_TX 0x00
#define LMP_MONITOR_DIR_RX 0x01

/* The RX tap (lmp_unpack) carries no link id in its arguments, so RX captures
 * report this sentinel instead of a real link. */
#define LMP_MONITOR_LINK_ID_UNKNOWN 0xFF

/** Enable the capture sources named by flags (LMP_MONITOR_*); 0 disables all. */
void lmp_monitor_set(uint8_t flags);

/** The currently enabled capture sources. */
uint8_t lmp_monitor_get(void);

/*
 * The outgoing-LMP tap. esp32_bt_unchained wires it into the ld_acl_lmp_tx slot;
 * it captures the PDU when LMP_TX is enabled and is a no-op otherwise. Called
 * before the ROM handler runs, so the buffer is still intact.
 */
void lmp_monitor_on_lmp_tx(uint32_t link_id, const bt_em_lmp_buf_elt_t *buf_elt);

/*
 * The incoming-LMP tap. esp32_bt_unchained wires it into the lmp_unpack slot,
 * the one point every received LMP PDU passes through. Pass the wire bytes and
 * their on-air length from the opcode's lmp_desc_tab entry -- NOT lmp_unpack's
 * *len, which comes back padded; it captures the PDU when LMP_RX is enabled and
 * is a no-op otherwise.
 */
void lmp_monitor_on_lmp_rx(const uint8_t *pdu, uint8_t pdu_len);

/*
 * The outgoing-LL tap. esp32_bt_unchained wires it into the lld_pdu_data_tx_push
 * slot; pass the TX descriptor the ROM is about to queue. It captures the PDU
 * when LL_TX is enabled and is a no-op otherwise. Called before the ROM handler
 * runs, so the descriptor still describes this PDU.
 */
struct em_desc_node;
void lmp_monitor_on_ll_tx(const struct em_desc_node *tx_desc);

/*
 * The incoming-LL tap. esp32_bt_unchained wires it into the lld_pdu_rx_handler
 * slot, which drains the BLE RX ring. Pass the PDU count the handler was given;
 * it reads the descriptor ring before the ROM frees the buffers, captures each
 * PDU when LL_RX is enabled, and is a no-op otherwise.
 */
void lmp_monitor_on_ll_rx(uint8_t nb_rx);

/*
 * Command descriptor for our vendor event code (0xFF), so the ROM packs the
 * capture event with our status/params intact instead of erroring on a missing
 * descriptor -- the event-side twin of vsc_cmd_desc(). NULL for other codes.
 */
hci_evt_desc_t *lmp_monitor_evt_desc(uint8_t evt_code);

#endif /* LMP_MONITOR_H */
