/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 link-layer environments: the RAM structs the ROM's link manager and
 * link-layer driver keep. Only the fields the unchained layer reads are modelled.
 */

#ifndef ESP32C3_ROM_LLD_H
#define ESP32C3_ROM_LLD_H

#include <stddef.h>
#include <stdint.h>

/*
 * An outgoing link-layer TX buffer element: the node lld_con_data_tx /
 * lld_con_llcp_tx queue onto the connection. The PDU payload lives in exchange
 * memory at buf_handle (em_buf_get, hal/em.h). The length field carries the PDU
 * length in its low 10 bits; for data PDUs lld_con_tx_prog stores the LLID flag
 * in bits 12-13 (1 = continuation, else data start), set at program time.
 */
typedef struct lld_tx_elem
{
    uint8_t _hdr[4];       /* +0 co_list node header (next pointer) */
    uint16_t buf_handle;   /* +4 exchange-memory handle of the PDU payload */
    uint16_t length_flags; /* +6 length (bits 0-9); data LLID flag (bits 12-13) */
} lld_tx_elem_t;

_Static_assert(offsetof(lld_tx_elem_t, buf_handle) == 0x04, "tx elem buf_handle @ +4");
_Static_assert(offsetof(lld_tx_elem_t, length_flags) == 0x06, "tx elem length @ +6");

/* The PDU length carried in lld_tx_elem_t.length_flags (low 10 bits). */
#define LLD_TX_ELEM_LEN(elem) ((elem)->length_flags & 0x3ff)

/*
 * The link-manager environment. Only the public identity address is modelled:
 * Read_BD_ADDR copies p_llm_env->bd_addr, and the advertising/scan paths read it
 * when they build PDUs, so writing it re-brands the controller's public address.
 */
typedef struct llm_env
{
    uint8_t _reserved_00[0x0c];
    uint8_t bd_addr[6]; /* +0x0c public BD_ADDR */
} llm_env_t;

_Static_assert(offsetof(llm_env_t, bd_addr) == 0x0c, "public bd_addr @ +0x0c");

/* The live link-manager environment (absolute symbol p_llm_env @ 0x3fcdff98). */
extern llm_env_t *p_llm_env;

/*
 * The link-layer driver environment (p_lld_env @ 0x3fcdff9c). Only the current
 * RX descriptor index is modelled; the per-PDU RX path reads it to find the
 * descriptor (hal/em.h em_rxdesc_t) for the PDU being handled.
 */
typedef struct lld_env
{
    uint8_t _reserved_00[0xd8];
    uint8_t rx_desc_idx; /* +0xd8 index of the RX descriptor being processed */
} lld_env_t;

_Static_assert(offsetof(lld_env_t, rx_desc_idx) == 0xd8, "rx_desc_idx @ +0xd8");

extern lld_env_t *p_lld_env;

#endif /* ESP32C3_ROM_LLD_H */
