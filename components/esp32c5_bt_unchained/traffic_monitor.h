/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C5: reports controller LL PDUs to
 * the host as the shared 0xFF capture event (capture_event.h), driven by
 * SET_TRAFFIC_MONITOR.
 */

#ifndef TRAFFIC_MONITOR_H
#define TRAFFIC_MONITOR_H

#include <stdint.h>

#include "capture_event.h"

/* Capture sources this target can report (the SET_TRAFFIC_MONITOR mask the handler
 * accepts). The C5 is BLE-only, so only the LL bits apply. */
#define TRAFFIC_MONITOR_SUPPORTED (TRAFFIC_MONITOR_LL_TX | TRAFFIC_MONITOR_LL_RX)

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
