/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy Encription (ENC) memory definitions.
 */
#ifndef HAL_EM_BLE_ENC_H
#define HAL_EM_BLE_ENC_H

#include <stdint.h>
#include <assert.h>

typedef struct
{
    uint8_t plain[16];
    uint8_t cipher[16];
} em_ble_enc_t;
static_assert(sizeof(em_ble_enc_t) == 32);

#endif /* HAL_EM_BLE_ENC_H */
