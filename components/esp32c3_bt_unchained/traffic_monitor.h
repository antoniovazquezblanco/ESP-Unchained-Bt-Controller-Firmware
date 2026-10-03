/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C3: reports controller LL PDUs to
 * the host as vendor-specific HCI events, driven by SET_TRAFFIC_MONITOR.
 */

#ifndef TRAFFIC_MONITOR_H
#define TRAFFIC_MONITOR_H

#include <stdint.h>

#include "esp32c3_bt_rom.h"

/*
 * Monitor flags, the SET_TRAFFIC_MONITOR parameter byte. The C3 is BLE-only, so
 * the BR/EDR LMP bits (0x01/0x02) never apply; the bit values match the classic
 * ESP32 so one host tool drives every target.
 */
#define TRAFFIC_MONITOR_LL_TX 0x04 /* outgoing BLE LL PDUs (data + control) */
#define TRAFFIC_MONITOR_LL_RX 0x08 /* incoming BLE LL PDUs                   */

/* Everything a hook exists for; a request outside this is UNSUPPORTED_FEATURE. */
#define TRAFFIC_MONITOR_SUPPORTED (TRAFFIC_MONITOR_LL_TX | TRAFFIC_MONITOR_LL_RX)

/*
 * Capture event layout, carried in the 0xFF vendor event after code and length
 * (identical to the classic ESP32 LMP/LL monitor):
 *
 *   [0] subcode = TRAFFIC_MONITOR_EVT_SUBCODE
 *   [1] direction: 0 = TX, 1 = RX
 *   [2] link id
 *   [3..6] controller clock at capture (little-endian uint32)
 *   [7] PDU length
 *   [8..] the raw LL PDU: a reconstructed 2-byte header (LLID, length) then payload
 */
#define TRAFFIC_MONITOR_EVT_SUBCODE 0x01
#define TRAFFIC_MONITOR_DIR_TX 0x00
#define TRAFFIC_MONITOR_DIR_RX 0x01

/** Enable the capture sources named by flags (TRAFFIC_MONITOR_*); 0 disables all. */
void traffic_monitor_set(uint8_t flags);

/** The currently enabled capture sources. */
uint8_t traffic_monitor_get(void);

/*
 * The incoming-LL tap. esp32c3_bt_unchained wires it into the
 * lld_con_rx_llcp_check slot, called once per received connection PDU with its
 * LLID and length. It reads the PDU from exchange memory and emits a capture
 * event when LL_RX is enabled; a no-op otherwise.
 */
void traffic_monitor_on_ll_rx(uint32_t link_id, uint32_t llid, uint16_t length);

/*
 * The outgoing-LL tap. esp32c3_bt_unchained wires it into the lld_con_data_tx
 * slot (llid 2, data) and the lld_con_llcp_tx slot (llid 3, control), each called
 * as the PDU is queued. It reads the PDU from exchange memory and emits a capture
 * event when LL_TX is enabled; a no-op otherwise.
 */
void traffic_monitor_on_ll_tx(uint32_t link_id, const lld_tx_elem_t *tx_elem, uint8_t llid);

/*
 * Event descriptor for our vendor event code (0xFF), so r_hci_build_evt packs the
 * capture event instead of dropping it on a missing descriptor. NULL for other
 * codes.
 */
esp32c3_hci_evt_desc_t *traffic_monitor_evt_desc(uint8_t evt_code);

#endif /* TRAFFIC_MONITOR_H */
