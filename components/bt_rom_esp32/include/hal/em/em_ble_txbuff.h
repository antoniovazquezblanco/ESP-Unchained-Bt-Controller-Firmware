/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy transmit buffer (TXBUFFER) memory definitions.
 */
#ifndef HAL_EM_BLE_TXBUFF_H
#define HAL_EM_BLE_TXBUFF_H

#include <stdint.h>

typedef struct
{
    uint8_t buff[38];
} em_ble_txbuff_cntl_elt_t;
static_assert(sizeof(em_ble_txbuff_cntl_elt_t) == 38);

typedef struct
{
    em_ble_txbuff_cntl_elt_t elt[13];
} em_ble_txbuff_cntl_t;
static_assert(sizeof(em_ble_txbuff_cntl_t) == 494);

typedef struct
{
    uint8_t buff[260];
} em_ble_txbuff_data_elt_t;
static_assert(sizeof(em_ble_txbuff_data_elt_t) == 260);

typedef struct
{
    em_ble_txbuff_data_elt_t elt[10];
} em_ble_txbuff_data_t;
static_assert(sizeof(em_ble_txbuff_data_t) == 2600);

#endif /* HAL_EM_BLE_TXBUFF_H */
