/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Exchange Memory definitions.
 * This file contains a memory map definition of a region that Espressif refers to as "EM" or "Exchange Memory".
 * This region is mapped to DRAM.
 * For the time being, it is not fully clear how this interacts with the hardware and if it is only used for
 * the Bluetooth controller software implementation or if it is read by other cores...
 */
#ifndef HAL_EM_H
#define HAL_EM_H

#include <stddef.h>

#include "hal/em/em_common.h"
#include "hal/em/em_ble.h"

/***
 * Exchange memory definition.
 */
typedef struct __attribute__((packed))
{
    em_common_t common;
    em_ble_t ble;
} em_t;
static_assert(offsetof(em_t, ble) == 0x0096);

/** @brief Pointer to the exchange memory. */
extern volatile em_t *em;

#endif /* HAL_EM_H */
