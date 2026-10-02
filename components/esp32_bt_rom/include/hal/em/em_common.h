/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Common exchange table.
 * The use of this table is not fully documented for the time being.
 */
#ifndef HAL_EM_COMMON_H
#define HAL_EM_COMMON_H

#include <stdint.h>
#include <assert.h>

#include "hal/em/em_common_reg.h"

typedef struct __attribute__((packed))
{
    em_common_extab0_t extab0;
    em_common_extab1_t extab1;
} em_common_exchange_table_elt_t;
static_assert(sizeof(em_common_exchange_table_elt_t) == 0x0004);

typedef struct __attribute__((packed))
{
    em_common_exchange_table_elt_t elt[16];
} em_common_exchange_table_t;
static_assert(sizeof(em_common_exchange_table_t) == 0x0040);

typedef struct __attribute__((packed))
{
    em_common_exchange_table_t exchange_table;
    uint8_t frequency_table[0x50];
    uint8_t rf_sw_spi[0x06];
} em_common_t;
static_assert(sizeof(em_common_t) == 0x0096);

#endif /* HAL_EM_COMMON_H */
