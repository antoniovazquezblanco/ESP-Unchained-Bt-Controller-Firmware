/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy receive buffer (RXBUFFER) memory definitions.
 */
#ifndef HAL_EM_BLE_RXBUFF_H
#define HAL_EM_BLE_RXBUFF_H

#include <stdint.h>

typedef struct
{
    uint8_t buff[260];
} em_ble_rxbuff_elt_t;
static_assert(sizeof(em_ble_rxbuff_elt_t) == 260);

typedef struct
{
    em_ble_rxbuff_elt_t elt[8];
} em_ble_rxbuff_t;
static_assert(sizeof(em_ble_rxbuff_t) == 2080);

#endif /* HAL_EM_BLE_RXBUFF_H */
