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
