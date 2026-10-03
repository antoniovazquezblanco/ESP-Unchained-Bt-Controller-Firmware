/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel.
 */

#ifndef SCAN_PIN_H
#define SCAN_PIN_H

#include <stdint.h>

/* Disable pinning: scanning hops all three primary channels again. */
#define SCAN_PIN_OFF 0

/*
 * Pin scanning to primary channel `channel` (37, 38 or 39), or SCAN_PIN_OFF to
 * restore the default three-channel hop. Narrows the radio's primary-channel
 * bitmap, so it takes effect on the running scan and, via the scan-start hook,
 * on every scan started afterwards. Returns false for a channel outside
 * {0, 37, 38, 39}.
 */
_Bool scan_pin_set(uint8_t channel);

/* The currently pinned channel, or SCAN_PIN_OFF when hopping. */
uint8_t scan_pin_get(void);

/*
 * Re-apply the pin. bt_unchained_esp32 wires this into the lld_scan_start slot,
 * after the ROM has set the scan up with the full channel map; a no-op when not
 * pinned.
 */
void scan_pin_on_scan_start(void);

#endif /* SCAN_PIN_H */
