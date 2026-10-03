/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 BLE core registers (memory-mapped). The C3 uses the same RivieraWaves
 * BLE core as the classic ESP32, at base 0x60031000 instead of 0x3ff71200, so the
 * standard register block shares the classic's offsets. Only the block the ROM was
 * seen to use is named here; offsets marked "verified" were confirmed against the
 * rev3 ROM (r_rwip_time_get, r_rwble_isr), the rest follow the classic layout they
 * bracket. Registers are plain 32-bit cells -- bitfields are left to the few masks
 * documented inline, since the C3 core's bit assignments can differ from the classic.
 */

#ifndef ESP32C3_HAL_CORE_BLE_H
#define ESP32C3_HAL_CORE_BLE_H

#include <stddef.h>
#include <stdint.h>

typedef struct core_ble
{
    volatile uint32_t rwblecntl;           /* 0x00 BLE control */
    volatile uint32_t version;             /* 0x04 version */
    volatile uint32_t rwbleconf;           /* 0x08 configuration */
    volatile uint32_t intcntl;             /* 0x0c interrupt enable */
    volatile uint32_t intstat;             /* 0x10 interrupt status          (verified) */
    volatile uint32_t intrawstat;          /* 0x14 raw interrupt status */
    volatile uint32_t intack;              /* 0x18 interrupt acknowledge     (verified) */
    volatile uint32_t basetimecnt;         /* 0x1c coarse clock; write bit31 to sample, poll it clear (verified) */
    volatile uint32_t finetimecnt;         /* 0x20 fine time within the slot (verified) */
    volatile uint32_t bdaddrl;             /* 0x24 device address, low */
    volatile uint32_t bdaddru;             /* 0x28 device address, high */
    volatile uint32_t et_currentrxdescptr; /* 0x2c current RX descriptor pointer */
    volatile uint32_t _reserved_30[8];     /* 0x30..0x4f */
    volatile uint32_t diagcntl;            /* 0x50 diagnostics select        (verified) */
    volatile uint32_t diagstat;            /* 0x54 diagnostics value         (verified) */
    volatile uint32_t debugaddmax;         /* 0x58 debug memory upper bound */
    volatile uint32_t debugaddmin;         /* 0x5c debug memory lower bound */
    volatile uint32_t errortypestat;       /* 0x60 error type status         (verified) */
    volatile uint32_t swprofiling;         /* 0x64 software profiling */
} core_ble_t;

_Static_assert(offsetof(core_ble_t, intstat) == 0x10, "intstat @ 0x10");
_Static_assert(offsetof(core_ble_t, intack) == 0x18, "intack @ 0x18");
_Static_assert(offsetof(core_ble_t, basetimecnt) == 0x1c, "basetimecnt @ 0x1c");
_Static_assert(offsetof(core_ble_t, finetimecnt) == 0x20, "finetimecnt @ 0x20");
_Static_assert(offsetof(core_ble_t, diagcntl) == 0x50, "diagcntl @ 0x50");
_Static_assert(offsetof(core_ble_t, errortypestat) == 0x60, "errortypestat @ 0x60");

/* Write this to basetimecnt to latch the clock, then poll it until it clears. */
#define CORE_BLE_BASETIMECNT_SAMP (1u << 31)

/* The BLE core register block. */
#define CORE_BLE_BASE 0x60031000u
#define core_ble ((core_ble_t *)CORE_BLE_BASE)

#endif /* ESP32C3_HAL_CORE_BLE_H */
