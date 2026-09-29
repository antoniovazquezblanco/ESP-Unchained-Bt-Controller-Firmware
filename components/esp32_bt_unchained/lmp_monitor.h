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

#include "bt_em_buf.h"
#include "hci_desc_tabs.h"

/*
 * Monitor flags, the SET_TRAFFIC_MONITOR parameter byte. Only LMP_TX is
 * implemented; the rest are reserved so the wire flag values stay stable as the
 * capture surface grows, and are rejected until they have a hook behind them.
 */
#define LMP_MONITOR_LMP_TX 0x01 /* outgoing BR/EDR LMP PDUs */
#define LMP_MONITOR_LMP_RX 0x02 /* incoming BR/EDR LMP PDUs (reserved) */
#define LMP_MONITOR_LL_TX 0x04  /* outgoing BLE LL PDUs (reserved)     */
#define LMP_MONITOR_LL_RX 0x08  /* incoming BLE LL PDUs (reserved)     */

/* Everything a hook exists for; a request outside this is UNSUPPORTED_FEATURE. */
#define LMP_MONITOR_SUPPORTED (LMP_MONITOR_LMP_TX)

/*
 * Capture event layout, carried in the 0xFF vendor event after code and length.
 *
 *   [0] subcode = LMP_MONITOR_EVT_SUBCODE, namespaces this under the 0xFF event
 *   [1] direction: 0 = TX (controller -> peer), 1 = RX
 *   [2] link id
 *   [3..6] controller clock at capture (little-endian uint32, 312.5 us ticks)
 *   [7] PDU length
 *   [8..] the raw LMP/LL PDU bytes
 */
#define LMP_MONITOR_EVT_SUBCODE 0x01
#define LMP_MONITOR_DIR_TX 0x00
#define LMP_MONITOR_DIR_RX 0x01

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
 * Command descriptor for our vendor event code (0xFF), so the ROM packs the
 * capture event with our status/params intact instead of erroring on a missing
 * descriptor -- the event-side twin of vsc_cmd_desc(). NULL for other codes.
 */
hci_evt_desc_t *lmp_monitor_evt_desc(uint8_t evt_code);

#endif /* LMP_MONITOR_H */
