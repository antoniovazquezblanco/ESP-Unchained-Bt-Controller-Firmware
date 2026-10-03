/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Pin BLE advertising reception (scanning) to a single primary channel on the C3.
 *
 * Scanning normally hops the three primary advertising channels (37/38/39). The
 * scan control structure's hop-control register (CS+0x16) drives that: clearing
 * its frequency-hop enable (fh_en) and setting its channel index holds the radio
 * on one channel. The scan scheduler runs once per window, so the pin is
 * re-applied from the scan-sched hook; unpinning re-enables hopping once.
 */
#include "scan_pin.h"

#include <stddef.h>

#include "bt_rom_esp32c3.h"

/* The pinned channel (37/38/39), or SCAN_PIN_OFF. Written from the command
 * handler, read in the scan-sched hook; a byte access is atomic on this core. */
static volatile uint8_t s_channel;

/* Set when a pin is lifted, so the sched hook re-enables hopping once. */
static volatile uint8_t s_restore;

/* Order the exchange-memory write before the radio acts on it (RISC-V fence). */
static inline void scan_pin_barrier(void)
{
    __asm__ __volatile__("fence" ::: "memory");
}

_Bool scan_pin_set(uint8_t channel)
{
    if (channel != SCAN_PIN_OFF &&
        (channel < LLD_ADV_CHANNEL_37 || channel > LLD_ADV_CHANNEL_39)) {
        return 0;
    }
    if (channel == SCAN_PIN_OFF && s_channel != SCAN_PIN_OFF) {
        s_restore = 1; /* was pinned: let the next scan window resume hopping */
    }
    s_channel = channel;
    return 1;
}

uint8_t scan_pin_get(void)
{
    return s_channel;
}

/* The hop-control register of the CS element serving scan activity scan_idx. */
static em_ble_cs_hopcntl_t *scan_hopcntl(uint32_t scan_idx)
{
    if (lld_scan_env == NULL) {
        return NULL;
    }
    lld_scan_sub_env_t *sub = lld_scan_env[scan_idx];
    if (sub == NULL) {
        return NULL;
    }
    em_ble_cs_t *cs = (em_ble_cs_t *)r_plf_funcs_p->em_buf_get(EM_REGION_CS) + sub->cs_idx;
    return &cs->hopcntl;
}

void scan_pin_on_scan_sched(uint32_t scan_idx)
{
    uint8_t channel = s_channel;
    if (channel == SCAN_PIN_OFF && s_restore == 0) {
        return; /* not pinned and nothing to restore */
    }
    em_ble_cs_hopcntl_t *hop = scan_hopcntl(scan_idx);
    if (hop == NULL) {
        return;
    }
    em_ble_cs_hopcntl_t v = {.raw = hop->raw};
    if (channel == SCAN_PIN_OFF) {
        v.fields.fh_en = 1; /* resume the three-channel hop */
        s_restore = 0;
    } else {
        v.fields.ch_idx = channel; /* 37/38/39 directly */
        v.fields.hop_int = 0;
        v.fields.fh_en = 0;
    }
    hop->raw = v.raw;
    scan_pin_barrier();
}
