/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 controller exchange-memory (EM) buffer elements.
 */

#ifndef BT_EM_BUF_H
#define BT_EM_BUF_H

#include <stddef.h>
#include <stdint.h>

/** RivieraWaves intrusive singly-linked list header (ip/common/co_list.h). */
typedef struct co_list_hdr
{
    struct co_list_hdr *next;
} co_list_hdr_t;

/**
 * An outgoing BR/EDR LMP PDU buffer element, as handed to ld_acl_lmp_tx.
 *
 * The PDU bytes are in EM at `buf_ptr`; em_buf_tx_buff_addr_get(elt) returns
 * their CPU address (0x3ffb0000 + buf_ptr). Offsets verified against the ROM
 * decompilation of r_ld_acl_lmp_tx (reads +4 as the EM offset, +6 as length).
 */
typedef struct
{
    co_list_hdr_t hdr; /**< +0: list chaining while queued for TX */
    uint16_t buf_ptr;  /**< +4: EM offset of the LMP PDU bytes    */
    uint8_t length;    /**< +6: PDU length in bytes               */
} bt_em_lmp_buf_elt_t;

_Static_assert(offsetof(bt_em_lmp_buf_elt_t, buf_ptr) == 4, "buf_ptr must be at +4");
_Static_assert(offsetof(bt_em_lmp_buf_elt_t, length) == 6, "length must be at +6");

#endif /* BT_EM_BUF_H */
