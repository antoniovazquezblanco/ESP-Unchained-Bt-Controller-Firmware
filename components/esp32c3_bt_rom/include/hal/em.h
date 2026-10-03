/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 exchange memory (EM): the hardware memory the BLE radio reads and
 * writes. Unlike the classic ESP32 (a struct at a fixed base), the C3 reaches EM
 * through the ROM accessor em_buf_get (see rom/ip_funcs.h): a region id returns
 * that region's base, a buffer handle returns that buffer's address. Only the
 * slices the unchained layer touches are modelled; offsets are from the rev3 ROM
 * disassembly. No core radio registers are modelled yet (a future hal/core).
 */

#ifndef ESP32C3_HAL_EM_H
#define ESP32C3_HAL_EM_H

#include <stddef.h>
#include <stdint.h>

/* Exchange-memory region ids passed to em_buf_get. */
#define EM_REGION_CS 0x400      /* control structures, element stride 0x5a */
#define EM_REGION_RXDESC 0x1000 /* RX descriptors, element stride 0x14 */
#define EM_REGION_ET 0x1400     /* event table */

/* Control-structure element stride (one per link/activity); the full CS field
 * layout is not modelled yet (needed for the scan-channel pin, see the doc). */
#define EM_CS_STRIDE 0x5a

/*
 * A BLE RX descriptor (EM_REGION_RXDESC, stride EM_RXDESC_STRIDE). Only the
 * received PDU's buffer handle is modelled: em_buf_get(buf_handle) yields the PDU
 * bytes. The descriptor in use sits at index p_lld_env->rx_desc_idx (rom/lld.h).
 */
typedef struct em_rxdesc
{
    uint8_t _reserved_00[0x12];
    uint16_t buf_handle; /* +0x12 handle of the received PDU payload buffer */
} em_rxdesc_t;

_Static_assert(offsetof(em_rxdesc_t, buf_handle) == 0x12, "rxdesc buf_handle @ +0x12");

#define EM_RXDESC_STRIDE 0x14

#endif /* ESP32C3_HAL_EM_H */
