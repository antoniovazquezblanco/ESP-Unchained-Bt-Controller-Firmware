/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel on the C3.
 */

#ifndef SCAN_PIN_H
#define SCAN_PIN_H

#include <stdint.h>

/* Disable pinning: scanning hops all three primary channels again. */
#define SCAN_PIN_OFF 0

/* The three BLE primary advertising channels. */
#define LLD_ADV_CHANNEL_37 37
#define LLD_ADV_CHANNEL_38 38
#define LLD_ADV_CHANNEL_39 39

/*
 * Pin scanning to primary channel `channel` (37, 38 or 39), or SCAN_PIN_OFF to
 * restore the default three-channel hop. Returns false for a channel outside
 * {0, 37, 38, 39}. Takes effect from the next scan window via the scan-sched hook.
 */
_Bool scan_pin_set(uint8_t channel);

/* The currently pinned channel, or SCAN_PIN_OFF when hopping. */
uint8_t scan_pin_get(void);

/*
 * Re-apply the pin for scan activity `scan_idx`. esp32c3_bt_unchained wires this
 * into the lld_scan_sched slot, which the controller runs to set up each scan
 * window; it holds the sub-env's channel index at the pinned value so the window
 * stays on one channel instead of advancing 37/38/39. A no-op when not pinned.
 */
void scan_pin_on_scan_sched(uint32_t scan_idx);

#endif /* SCAN_PIN_H */
