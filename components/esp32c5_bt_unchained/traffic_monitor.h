/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C5: reports controller LL PDUs to
 * the host as vendor-specific HCI events, driven by SET_TRAFFIC_MONITOR.
 */

#ifndef TRAFFIC_MONITOR_H
#define TRAFFIC_MONITOR_H

#include <stdint.h>

/*
 * Monitor flags, the SET_TRAFFIC_MONITOR parameter byte. The C5 is BLE-only, so
 * the BR/EDR LMP bits (0x01/0x02) never apply; the bit values match the classic
 * ESP32 and the C3 so one host tool drives every target.
 */
#define TRAFFIC_MONITOR_LL_TX 0x04 /* outgoing BLE LL PDUs (data + control) */
#define TRAFFIC_MONITOR_LL_RX 0x08 /* incoming BLE LL PDUs (data + control) */

/* Everything a hook exists for; a request outside this is UNSUPPORTED_FEATURE. */
#define TRAFFIC_MONITOR_SUPPORTED (TRAFFIC_MONITOR_LL_TX | TRAFFIC_MONITOR_LL_RX)

/*
 * Capture event layout, carried in the 0xFF vendor event after code and length
 * (identical to the classic ESP32 and C3 monitors, so one host parser serves all):
 *
 *   [0] subcode = TRAFFIC_MONITOR_EVT_SUBCODE
 *   [1] direction: 0 = TX, 1 = RX
 *   [2] link id (connection handle, or LINK_ID_UNKNOWN when the tap has none)
 *   [3..6] controller link-layer tick at capture (little-endian uint32)
 *   [7] PDU length
 *   [8..] the raw LL PDU: a reconstructed 2-byte header (LLID, length) then payload
 */
#define TRAFFIC_MONITOR_EVT_SUBCODE 0x01
#define TRAFFIC_MONITOR_DIR_TX 0x00
#define TRAFFIC_MONITOR_DIR_RX 0x01

/* The RX tap identifies the PDU but not its connection handle, so RX captures
 * report this sentinel instead of a real link. */
#define TRAFFIC_MONITOR_LINK_ID_UNKNOWN 0xFF

/** Enable the capture sources named by flags (TRAFFIC_MONITOR_*); 0 disables all. */
void traffic_monitor_set(uint8_t flags);

/** The currently enabled capture sources. */
uint8_t traffic_monitor_get(void);

/*
 * The taps themselves are the linker-wrapped controller functions
 * __wrap_r_ble_ll_conn_rx_data_pdu (RX) and __wrap_r_ble_lll_conn_append_tx_buffer
 * (TX) in traffic_monitor.c; the CMakeLists --wrap options route the controller's
 * own calls through them. There is nothing for esp32c5_bt_unchained to wire up.
 */

#endif /* TRAFFIC_MONITOR_H */
