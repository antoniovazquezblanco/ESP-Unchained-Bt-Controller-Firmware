/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C3: reports controller LL PDUs to
 * the host as the shared 0xFF capture event (capture_event.h), driven by
 * SET_TRAFFIC_MONITOR.
 */

#ifndef TRAFFIC_MONITOR_H
#define TRAFFIC_MONITOR_H

#include <stdint.h>

#include "capture_event.h"
#include "bt_rom_esp32c3.h"

/* Capture sources this target can report (the SET_TRAFFIC_MONITOR mask the handler
 * accepts). The C3 is BLE-only, so only the LL bits apply. */
#define TRAFFIC_MONITOR_SUPPORTED (TRAFFIC_MONITOR_LL_TX | TRAFFIC_MONITOR_LL_RX)

/** Enable the capture sources named by flags (TRAFFIC_MONITOR_*); 0 disables all. */
void traffic_monitor_set(uint8_t flags);

/** The currently enabled capture sources. */
uint8_t traffic_monitor_get(void);

/*
 * The incoming-LL tap. bt_unchained_esp32c3 wires it into the
 * lld_con_rx_llcp_check slot, called once per received connection PDU with its
 * LLID and length. It reads the PDU from exchange memory and emits a capture
 * event when LL_RX is enabled; a no-op otherwise.
 */
void traffic_monitor_on_ll_rx(uint32_t link_id, uint32_t llid, uint16_t length);

/*
 * The outgoing-LL tap. bt_unchained_esp32c3 wires it into the lld_con_data_tx
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
