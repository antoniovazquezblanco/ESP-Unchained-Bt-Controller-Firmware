/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel on the C5.
 *
 * Scanning normally hops the three primary channels (37/38/39). The controller
 * keeps a forced-channel byte in its link-layer environment (ble_ll_env_p + 0x38):
 * 0 means hop, a channel number (37/38/39) holds the radio there. The scan path
 * reads it each window, so a single write pins or releases scanning with no
 * restart. This is exactly what the controller's own SET_SCAN_CHAN QA command
 * does; we reuse the mechanism rather than hooking the channel chooser (which the
 * controller only calls from within its own object, so a linker --wrap cannot
 * reach it).
 */
#include "scan_pin.h"

#include <stddef.h>

#include "esp32c5_bt_rom.h"

/* The pinned channel (37/38/39), or SCAN_PIN_OFF. Mirrors the controller byte so
 * scan_pin_get works before any poke and across controller resets. */
static volatile uint8_t s_channel;

_Bool scan_pin_set(uint8_t channel)
{
    if (channel != SCAN_PIN_OFF &&
        (channel < LLD_ADV_CHANNEL_37 || channel > LLD_ADV_CHANNEL_39)) {
        return 0;
    }
    if (ble_ll_env_p == NULL) {
        return 0; /* controller not initialised yet */
    }
    /* The byte is the channel number itself (37/38/39) or 0 to hop. */
    *((volatile uint8_t *)ble_ll_env_p + BLE_LL_ENV_SCAN_CHAN_OFFSET) = channel;
    s_channel = channel;
    return 1;
}

uint8_t scan_pin_get(void)
{
    return s_channel;
}
