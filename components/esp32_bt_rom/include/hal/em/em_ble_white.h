/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy Whitelist definitions.
 */
#ifndef HAL_EM_BLE_WHITE_H
#define HAL_EM_BLE_WHITE_H

#include "hal/types.h"
#include <assert.h>

#define BLE_WHITELIST_MAX 12

typedef struct
{
    bdaddr_t public[BLE_WHITELIST_MAX];
    bdaddr_t private[BLE_WHITELIST_MAX];
} em_ble_white_t;
static_assert(sizeof(em_ble_white_t) == 0x90);

#endif /* HAL_EM_BLE_WHITE_H */
