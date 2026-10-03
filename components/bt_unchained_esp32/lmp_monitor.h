/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor: reports controller LMP/LL PDUs to the host as
 * the shared 0xFF capture event (capture_event.h), driven by SET_TRAFFIC_MONITOR.
 */

#ifndef LMP_MONITOR_H
#define LMP_MONITOR_H

#include <stdint.h>

#include "capture_event.h"
#include "rom/bt_em_buf.h"
#include "rom/hci_desc_tabs.h"

/* Capture sources this target can report (the SET_TRAFFIC_MONITOR mask the handler
 * accepts). The classic ESP32 is the only dual-mode target, so it alone implements
 * the BR/EDR LMP sources in addition to the BLE LL ones. */
#define TRAFFIC_MONITOR_SUPPORTED \
    (TRAFFIC_MONITOR_LMP_TX | TRAFFIC_MONITOR_LMP_RX | TRAFFIC_MONITOR_LL_TX | TRAFFIC_MONITOR_LL_RX)

/*
 * TRAFFIC_MONITOR_LL_RX captures every incoming LL PDU (data and control, LLCP);
 * TRAFFIC_MONITOR_LL_TX captures outgoing LL *data* only. Outgoing LL control is
 * queued straight to the baseband by llc_llcp_send, which is not one of the
 * lld_pdu push slots we can hook, so it does not pass our TX tap -- the peer's
 * replies still show on LL_RX.
 */

/** Enable the capture sources named by flags (TRAFFIC_MONITOR_*); 0 disables all. */
void lmp_monitor_set(uint8_t flags);

/** The currently enabled capture sources. */
uint8_t lmp_monitor_get(void);

/*
 * The outgoing-LMP tap. bt_unchained_esp32 wires it into the ld_acl_lmp_tx slot;
 * it captures the PDU when LMP_TX is enabled and is a no-op otherwise. Called
 * before the ROM handler runs, so the buffer is still intact.
 */
void lmp_monitor_on_lmp_tx(uint32_t link_id, const bt_em_lmp_buf_elt_t *buf_elt);

/*
 * The incoming-LMP tap. bt_unchained_esp32 wires it into the lmp_unpack slot,
 * the one point every received LMP PDU passes through. Pass the wire bytes and
 * their on-air length from the opcode's lmp_desc_tab entry -- NOT lmp_unpack's
 * *len, which comes back padded; it captures the PDU when LMP_RX is enabled and
 * is a no-op otherwise.
 */
void lmp_monitor_on_lmp_rx(const uint8_t *pdu, uint8_t pdu_len);

/*
 * The outgoing-LL tap. bt_unchained_esp32 wires it into the lld_pdu_data_tx_push
 * slot; pass the TX descriptor the ROM is about to queue. It captures the PDU
 * when LL_TX is enabled and is a no-op otherwise. Called before the ROM handler
 * runs, so the descriptor still describes this PDU.
 */
struct em_desc_node;
void lmp_monitor_on_ll_tx(const struct em_desc_node *tx_desc);

/*
 * The incoming-LL tap. bt_unchained_esp32 wires it into the lld_pdu_rx_handler
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
