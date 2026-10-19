/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 link controller (LLC) environment: the per-link LLCP TX bookkeeping.
 *
 * LLC owns the LLCP TX flow. r_llc_llcp_send queues an llc_llcp_tx_elt_t on
 * llcp_tx_queue; r_llc_llcp_tx_check sends one at a time, moving it to the queue
 * head and setting LLC_FLAG_LLCP_TX_BUSY. On air ack, lld_llcp_tx_cfm_handler pops
 * the head unconditionally, calls its cfm_cb if set, ke_frees it, clears the flag
 * and calls tx_check again. From the rev3 ROM disassembly (0x40010cc6, 0x40010dd2,
 * 0x4001110c).
 */

#ifndef ESP32C3_ROM_LLC_H
#define ESP32C3_ROM_LLC_H

#include <stddef.h>
#include <stdint.h>

/* An RW kernel list head (co_list): first / last element. */
typedef struct co_list
{
    void *first;
    void *last;
} co_list_t;

/* LLC flags bit: an LLCP PDU is in flight at LLD (one at a time). */
#define LLC_FLAG_LLCP_TX_BUSY 0x0002

/* The per-link LLC environment (partial model). */
typedef struct llc_env
{
    uint8_t _reserved_00[0x28];
    co_list_t llcp_tx_queue; /* +0x28 LLCP PDUs to send, in-flight one at head */
    uint8_t _reserved_30[0x42 - 0x30];
    uint16_t flags; /* +0x42 LLC_FLAG_* */
} llc_env_t;

_Static_assert(offsetof(llc_env_t, llcp_tx_queue) == 0x28, "llcp_tx_queue @ +0x28");
_Static_assert(offsetof(llc_env_t, flags) == 0x42, "flags @ +0x42");

/* An LLCP TX element on llcp_tx_queue (ke_malloc'd, KE_MEM_KE_MSG). cfm_cb, when
 * set, runs on air ack with (link_id, opcode). */
typedef struct llc_llcp_tx_elt
{
    uint8_t _hdr[4];                                 /* +0 co_list link */
    void (*cfm_cb)(uint8_t link_id, uint8_t opcode); /* +4 TX-confirm callback */
    uint8_t length;                                  /* +8 PDU length */
    uint8_t pdu[];                                   /* +9 PDU, opcode first */
} llc_llcp_tx_elt_t;

_Static_assert(offsetof(llc_llcp_tx_elt_t, cfm_cb) == 4, "cfm_cb @ +4");
_Static_assert(offsetof(llc_llcp_tx_elt_t, length) == 8, "length @ +8");
_Static_assert(offsetof(llc_llcp_tx_elt_t, pdu) == 9, "pdu @ +9");

/* Per-link LLC environments, indexed by link id like lld_con_env; NULL when
 * inactive (absolute symbol llc_env @ 0x3fcdfcec). */
extern llc_env_t *llc_env[];

#endif /* ESP32C3_ROM_LLC_H */
