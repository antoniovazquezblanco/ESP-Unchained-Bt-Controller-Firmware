/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy receive descriptor (RXDESC) memory definitions.
 */
#ifndef HAL_EM_BLE_RXDESC_H
#define HAL_EM_BLE_RXDESC_H

#include "hal/em/em_ble_rxdesc_reg.h"

typedef struct
{
    em_ble_rxdesc_rxcntl_t rxcntl;
    em_ble_rxdesc_rxstat_t rxstat;
    union
    {
        em_ble_rxdesc_rxphce_t rxphce;
        em_ble_rxdesc_rxphadv_t rxphadv;
    };
    em_ble_rxdesc_rxchass_t rxchass;
    em_ble_rxdesc_rxdataptr_t rxdataptr;
    em_ble_rxdesc_rxralptr_t rxralptr;
} em_ble_rxdesc_elt_t;
static_assert(sizeof(em_ble_rxdesc_elt_t) == 12);

/* Number of receive descriptors in the ring; the controller advances the
 * current index modulo this count. */
#define EM_BLE_RXDESC_COUNT 8

typedef struct
{
    em_ble_rxdesc_elt_t elt[EM_BLE_RXDESC_COUNT];
} em_ble_rxdesc_t;

#endif /* HAL_EM_BLE_RXDESC_H */
