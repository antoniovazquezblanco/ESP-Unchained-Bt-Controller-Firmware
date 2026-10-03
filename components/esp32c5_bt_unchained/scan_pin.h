/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel on the C5.
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
 * {0, 37, 38, 39}, or if the controller is not yet initialised. Takes effect from
 * the next scan window; the controller holds the setting until it is changed.
 */
_Bool scan_pin_set(uint8_t channel);

/* The currently pinned channel, or SCAN_PIN_OFF when hopping. */
uint8_t scan_pin_get(void);

#endif /* SCAN_PIN_H */
