/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy transmit descriptor (TXDESC) register definitions.
 */
#ifndef HAL_EM_BLE_TXDESC_REG_H
#define HAL_EM_BLE_TXDESC_REG_H

#include <stdint.h>

/**
 * TXCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:14              NEXTPTR   0x0
 *     15               TXDONE   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short nextptr : 15;
        unsigned short txdone : 1;
    } fields;
} em_ble_txdesc_txcntl_t;
static_assert(sizeof(em_ble_txdesc_txcntl_t) == 2);

/**
 * TXPHCE register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:01               TXLLID   0x0
 *     02               TXNESN   0
 *     03                 TXSN   0
 *     04                 TXMD   0
 *  05:07                    -
 *  08:15                TXLEN   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txllid : 2;
        unsigned short txnesn : 1;
        unsigned short txsn : 1;
        unsigned short txmd : 1;
        unsigned short : 3;
        unsigned short txlen : 8;
    } fields;
} em_ble_txdesc_txphce_t;
static_assert(sizeof(em_ble_txdesc_txphce_t) == 2);

/**
 * TXPHADV register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:03               TXTYPE   0x0
 *  04:05                    -
 *     06              TXTXADD   0
 *     07              TXRXADD   0
 *  08:15             TXADVLEN   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txtype : 4;
        unsigned short : 2;
        unsigned short txtxadd : 1;
        unsigned short txrxadd : 1;
        unsigned short txadvlen : 8;
    } fields;
} em_ble_txdesc_txphadv_t;
static_assert(sizeof(em_ble_txdesc_txphadv_t) == 2);

/**
 * TXDATAPTR register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            TXDATAPTR   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txdataptr : 16;
    } fields;
} em_ble_txdesc_txdataptr_t;
static_assert(sizeof(em_ble_txdesc_txdataptr_t) == 2);

/**
 * TXDLE register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07              BUFFIDX   0x0
 *  08:11              FRAGCNT   0x0
 *  12:14                    -
 *     15             FREEBUFF   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short buffidx : 8;
        unsigned short fragcnt : 4;
        unsigned short : 3;
        unsigned short freebuff : 1;
    } fields;
} em_ble_txdesc_txdle_t;
static_assert(sizeof(em_ble_txdesc_txdle_t) == 2);

#endif /* HAL_EM_BLE_TXDESC_REG_H */
