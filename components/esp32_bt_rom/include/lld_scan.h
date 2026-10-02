/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 BLE control-structure HOPCNTL register, which drives the channel the
 * adv/scan radio uses. Layout cross-checked against Tarlogic's esp32-bluetooth-rw
 * HAL and verified on-chip.
 */

#ifndef LLD_SCAN_H
#define LLD_SCAN_H

#include <stdint.h>

/* The three BLE primary advertising channel indices. */
#define LLD_ADV_CHANNEL_37 37
#define LLD_ADV_CHANNEL_39 39

/*
 * HOPCNTL register of a BLE control structure (one 16-bit word in exchange
 * memory). The radio reads it each event to pick the channel and whether to hop:
 * with fh_en set it advances ch_idx every event (the three-channel scan sweep);
 * cleared, it stays on ch_idx.
 */
typedef union
{
    uint16_t raw;
    struct
    {
        uint16_t ch_idx : 6;  /* current channel index            */
        uint16_t : 2;         /*                                  */
        uint16_t hop_int : 5; /* channel hop interval             */
        uint16_t : 2;         /*                                  */
        uint16_t fh_en : 1;   /* frequency hopping enable         */
    } fields;
} lld_hopcntl_t;
_Static_assert(sizeof(lld_hopcntl_t) == 2, "HOPCNTL is a 16-bit register");

/*
 * HOPCNTL of the adv/scan control structure. The adv/scan event uses CS index 10
 * in exchange memory, so the register sits at EM base (0x3ffb0000) + em_ble
 * (0x96) + cs (0x20) + 10 * sizeof(cs_elt) (0x5a) + hopcntl offset (0x10) =
 * 0x3ffb044a, where r_lld_scan_start writes 0x8027 (fh_en set, ch_idx 39).
 */
#define LLD_ADV_HDL 10
#define LLD_SCAN_HOPCNTL (*(volatile lld_hopcntl_t *)0x3ffb044a)

#endif /* LLD_SCAN_H */
