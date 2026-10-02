/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy exchange memory. This includes a memory map definition for the BLE peripheral.
 */
#ifndef HAL_EM_BLE_H
#define HAL_EM_BLE_H

#include "hal/em/em_ble_enc.h"
#include "hal/em/em_ble_cs.h"
#include "hal/em/em_ble_white.h"
#include "hal/em/em_ble_ral.h"
#include "hal/em/em_ble_txdesc.h"
#include "hal/em/em_ble_rxdesc.h"
#include "hal/em/em_ble_txbuff.h"
#include "hal/em/em_ble_rxbuff.h"
#include <assert.h>
#include <stddef.h>

/***
 * Memory map definition for the BLE peripheral.
 */
typedef struct
{
    em_ble_enc_t enc;                 /**< Encryption definitions. */
    em_ble_cs_t cs;                   /**< Control structures definitions. */
    em_ble_white_t whitelist;         /**< Whitelist definitions. */
    em_ble_ral_t ral;                 /**< Resolve address list definitions. */
    em_ble_txdesc_t txdesc;           /**< Transmit descriptor. */
    em_ble_rxdesc_t rxdesc;           /**< Receive descriptor. */
    em_ble_txbuff_cntl_t txbuff_cntl; /**< Control transmit buffer. */
    em_ble_txbuff_data_t txbuff_data; /**< Data transmit buffer. */
    em_ble_rxbuff_t rxbuff;           /**< Receive buffer. */
} em_ble_t;
static_assert(offsetof(em_ble_t, enc) == 0x0000);
static_assert(offsetof(em_ble_t, cs) == 0x0020);
static_assert(offsetof(em_ble_t, whitelist) == 0x03fe);
static_assert(offsetof(em_ble_t, ral) == 0x048e);
static_assert(offsetof(em_ble_t, txdesc) == 0x052a);
static_assert(offsetof(em_ble_t, rxdesc) == 0x08b2);
static_assert(offsetof(em_ble_t, txbuff_cntl) == 0x0912);
static_assert(offsetof(em_ble_t, txbuff_data) == 0x0b00);
static_assert(offsetof(em_ble_t, rxbuff) == 0x1528);

#endif /* HAL_EM_BLE_H */
