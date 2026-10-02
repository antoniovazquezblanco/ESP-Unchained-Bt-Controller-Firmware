/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel.
 *
 * Scanning normally hops the three primary advertising channels. The adv/scan
 * control structure's HOPCNTL register drives that: clearing its frequency-hop
 * enable (fh_en) and setting its channel index holds the radio on one channel.
 * r_lld_scan_start rewrites HOPCNTL (hopping on) at every scan, so the pin is
 * re-applied from the scan-start hook.
 */
#include "scan_pin.h"

#include "esp32_bt_rom.h"

/* The pinned channel (37/38/39), or SCAN_PIN_OFF. Written from the command
 * handler, read in the scan-start hook; a byte access is atomic on this core. */
static volatile uint8_t s_channel;

/* Order the exchange-memory write the way the ROM's own lld paths do. */
static inline void scan_pin_barrier(void)
{
    __asm__ __volatile__("memw");
}

/* Hold the adv/scan radio on `channel` (fh_en = 0), or resume hopping. */
static void scan_pin_apply(uint8_t channel)
{
    em_ble_cs_hopcntl_t h = {.raw = em->ble.cs.elt[LLD_ADV_HDL].hopcntl.raw};
    if (channel == SCAN_PIN_OFF) {
        h.fields.fh_en = 1;
    } else {
        h.fields.ch_idx = channel;
        h.fields.hop_int = 0;
        h.fields.fh_en = 0;
    }
    em->ble.cs.elt[LLD_ADV_HDL].hopcntl.raw = h.raw;
    scan_pin_barrier();
}

_Bool scan_pin_set(uint8_t channel)
{
    if (channel != SCAN_PIN_OFF &&
        (channel < LLD_ADV_CHANNEL_37 || channel > LLD_ADV_CHANNEL_39)) {
        return 0;
    }
    s_channel = channel;
    scan_pin_apply(channel);
    return 1;
}

uint8_t scan_pin_get(void)
{
    return s_channel;
}

void scan_pin_on_scan_start(void)
{
    if (s_channel != SCAN_PIN_OFF) {
        scan_pin_apply(s_channel);
    }
}
