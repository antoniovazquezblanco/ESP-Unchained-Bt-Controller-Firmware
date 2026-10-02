/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy Resolve Address List (RAL) memory definitions.
 */
#ifndef HAL_EM_BLE_RAL_H
#define HAL_EM_BLE_RAL_H

#include "hal/em/em_ble_ral_reg.h"

typedef struct
{
    em_ble_ral_info_t info;
    uint8_t unk[50];
} em_ble_ral_elt_t;
static_assert(sizeof(em_ble_ral_elt_t) == 52);

typedef struct
{
    em_ble_ral_elt_t elt[3];
} em_ble_ral_t;

#endif /* HAL_EM_BLE_RAL_H */
