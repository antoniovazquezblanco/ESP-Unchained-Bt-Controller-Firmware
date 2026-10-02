/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy transmit descriptor (TXDESC) memory definitions.
 */
#ifndef HAL_EM_BLE_TXDESC_H
#define HAL_EM_BLE_TXDESC_H

#include "hal/em/em_ble_txdesc_reg.h"

typedef struct
{
    em_ble_txdesc_txcntl_t txcntl;
    union
    {
        em_ble_txdesc_txphce_t txphce;
        em_ble_txdesc_txphadv_t txphadv;
    };
    em_ble_txdesc_txdataptr_t txdataptr;
    em_ble_txdesc_txdle_t txdle;
} em_ble_txdesc_elt_t;
static_assert(sizeof(em_ble_txdesc_elt_t) == 8);

typedef struct
{
    em_ble_txdesc_elt_t elt[113];
} em_ble_txdesc_t;

#endif /* HAL_EM_BLE_TXDESC_H */
