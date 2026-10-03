/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 exchange memory (EM): the hardware memory the BLE radio reads and
 * writes. Unlike the classic ESP32 (a struct at a fixed base), the C3 reaches EM
 * through the ROM accessor em_buf_get (see rom/ip_funcs.h): a region id returns
 * that region's base, a buffer handle returns that buffer's address. Only the
 * slices the unchained layer touches are modelled; offsets are from the rev3 ROM
 * disassembly. The memory-mapped core radio registers live in hal/core.
 */

#ifndef ESP32C3_HAL_EM_H
#define ESP32C3_HAL_EM_H

#include <stddef.h>
#include <stdint.h>

/* Exchange-memory region ids passed to em_buf_get. */
#define EM_REGION_CS 0x400      /* control structures, element stride 0x5a */
#define EM_REGION_RXDESC 0x1000 /* RX descriptors, element stride 0x14 */
#define EM_REGION_TXDESC 0x1400 /* TX descriptors, element stride 0x0e */

/* Control-structure element stride (one per link/activity); see em_ble_cs_t. */
#define EM_CS_STRIDE 0x5a

/*
 * The hop-control register of a scan/adv control structure (em_ble_cs.hopcntl).
 * fh_en enables the hardware primary-channel hop; clearing it and setting ch_idx
 * holds the radio on one channel. Matches the classic em_ble_cs_hopcntl layout.
 */
typedef union
{
    uint16_t raw;
    struct
    {
        uint16_t ch_idx : 6; /* current/pinned channel index (0-39) */
        uint16_t _rsv0 : 2;
        uint16_t hop_int : 5; /* hop increment */
        uint16_t _rsv1 : 2;
        uint16_t fh_en : 1; /* frequency-hop enable (bit 15) */
    } fields;
} em_ble_cs_hopcntl_t;

_Static_assert(sizeof(em_ble_cs_hopcntl_t) == 2, "hopcntl is a 16-bit register");

/*
 * A control-structure element (EM_REGION_CS, one per link/activity, stride
 * EM_CS_STRIDE). This is the classic em_ble_cs layout with six extra bytes inserted
 * before the sync word, so the fields sit 6 bytes later than on the classic. Only
 * the registers the model touches are named; offsets verified against the rev3 ROM
 * (lld_scan_start sets the access address / CRC init / hopcntl, lld_con_evt_start
 * the channel map).
 */
typedef struct em_ble_cs
{
    uint16_t cntl;                             /* 0x00 control / format */
    uint16_t _reserved_02[5];                  /* 0x02..0x0b */
    uint16_t syncwl;                           /* 0x0c access address, low */
    uint16_t syncwh;                           /* 0x0e access address, high */
    uint16_t crcinit0;                         /* 0x10 CRC init, low */
    uint16_t crcinit1;                         /* 0x12 CRC init, high */
    uint16_t _reserved_14;                     /* 0x14 */
    em_ble_cs_hopcntl_t hopcntl;               /* 0x16 hop control */
    uint16_t _reserved_18[5];                  /* 0x18..0x21 */
    uint16_t chmap0;                           /* 0x22 channel map, bits 0-15 */
    uint16_t chmap1;                           /* 0x24 channel map, bits 16-31 */
    uint16_t chmap2;                           /* 0x26 channel map, bits 32-39 */
    uint8_t _reserved_28[EM_CS_STRIDE - 0x28]; /* 0x28..0x59 */
} em_ble_cs_t;

_Static_assert(sizeof(em_ble_cs_t) == EM_CS_STRIDE, "CS element spans EM_CS_STRIDE");
_Static_assert(offsetof(em_ble_cs_t, syncwl) == 0x0c, "CS syncwl @ 0x0c");
_Static_assert(offsetof(em_ble_cs_t, crcinit0) == 0x10, "CS crcinit0 @ 0x10");
_Static_assert(offsetof(em_ble_cs_t, hopcntl) == 0x16, "CS hopcntl @ 0x16");
_Static_assert(offsetof(em_ble_cs_t, chmap0) == 0x22, "CS chmap0 @ 0x22");

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

/*
 * A BLE TX descriptor (EM_REGION_TXDESC, stride EM_TXDESC_STRIDE). lld_con_tx_prog
 * programs these from the queued lld_tx_elem_t; lld_con_tx reads them back on TX
 * completion. The unchained monitor taps the queue instead (lld_con_data_tx /
 * lld_con_llcp_tx), so this is modelled for reference only.
 */
typedef struct em_txdesc
{
    uint8_t _reserved_00[0x02];
    uint16_t hdr;        /* +0x02 on-air header: LLID (bits 0-1), length (bits 8-15) */
    uint16_t buf_handle; /* +0x04 handle of the transmitted PDU payload buffer */
} em_txdesc_t;

_Static_assert(offsetof(em_txdesc_t, hdr) == 0x02, "txdesc hdr @ +0x02");
_Static_assert(offsetof(em_txdesc_t, buf_handle) == 0x04, "txdesc buf_handle @ +0x04");

#define EM_TXDESC_STRIDE 0x0e

#endif /* ESP32C3_HAL_EM_H */
