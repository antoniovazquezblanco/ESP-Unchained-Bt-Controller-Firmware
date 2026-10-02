/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy receive descriptor (RXDESC) register definitions.
 */
#ifndef HAL_EM_BLE_RXDESC_REG_H
#define HAL_EM_BLE_RXDESC_REG_H

#include <stdint.h>

/**
 * RXCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:14              NEXTPTR   0x0
 *     15               RXDONE   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short nextptr : 15; // Offset of the next rx descriptor in EM.
        unsigned short rxdone : 1;   // Packet available in this descriptor. Set by the hardware. Clear this bit after processing the data.
    } fields;
} em_ble_rxdesc_rxcntl_t;
static_assert(sizeof(em_ble_rxdesc_rxcntl_t) == 2);

/**
 * RXSTAT register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00             SYNC_ERR   0
 *     01             TYPE_ERR   0
 *     02              LEN_ERR   0
 *     03              CRC_ERR   0
 *     04              MIC_ERR   0
 *     05               SN_ERR   0
 *     06             NESN_ERR   0
 *     07         BDADDR_MATCH   0
 *     08            RXTIMEERR   0
 *     09           PRIV_ERROR   0
 *     10       PEER_ADD_MATCH   0
 *  11:15            RXLINKLBL   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short sync_err : 1;
        unsigned short type_err : 1;
        unsigned short len_err : 1;
        unsigned short crc_err : 1;
        unsigned short mic_err : 1;
        unsigned short sn_err : 1;
        unsigned short nesn_err : 1;
        unsigned short bdaddr_match : 1;
        unsigned short rxtimeerr : 1;
        unsigned short priv_error : 1;
        unsigned short peer_addr_match : 1;
        unsigned short rxlinklbl : 5; // Index of the corresponding control structure (?)
    } fields;
} em_ble_rxdesc_rxstat_t;
static_assert(sizeof(em_ble_rxdesc_rxstat_t) == 2);

/**
 * RXPHCE register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:01               RXLLID   0x0
 *     02               RXNESN   0
 *     03                 RXSN   0
 *     04                 RXMD   0
 *  05:07                    -
 *  08:15                RXLEN   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxllid : 2;
        unsigned short rxnesn : 1;
        unsigned short rxsn : 1;
        unsigned short rxmd : 1;
        unsigned short : 3;
        unsigned short rxlen : 8;
    } fields;
} em_ble_rxdesc_rxphce_t;
static_assert(sizeof(em_ble_rxdesc_rxphce_t) == 2);

/**
 * RXPHADV register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:03               RXTYPE   0x0
 *  04:05                    -
 *     06              RXTXADD   0
 *     07              RXRXADD   0
 *  08:15             RXADVLEN   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxtype : 4;
        unsigned short : 2;
        unsigned short rxtxadd : 1;
        unsigned short rxrxadd : 1;
        unsigned short rxadvlen : 8;
    } fields;
} em_ble_rxdesc_rxphadv_t;
static_assert(sizeof(em_ble_rxdesc_rxphadv_t) == 2);

/**
 * RXCHASS register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07                 RSSI   0x0
 *  08:13          USED_CH_IDX   0x0
 *     14                    -
 *     15               IS_ISO   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        signed short rssi : 8;          // Received packet signal strength
        unsigned short used_ch_idx : 6; // Received packet channel number
        unsigned short : 1;
        unsigned short is_iso : 1;
    } fields;
} em_ble_rxdesc_rxchass_t;
static_assert(sizeof(em_ble_rxdesc_rxchass_t) == 2);

/**
 * RXDATAPTR register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            RXDATAPTR   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxdataptr : 16;
    } fields;
} em_ble_rxdesc_rxdataptr_t;
static_assert(sizeof(em_ble_rxdesc_rxdataptr_t) == 2);

/**
 * RXRALPTR register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15             RXRALPTR   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxralptr : 16;
    } fields;
} em_ble_rxdesc_rxralptr_t;
static_assert(sizeof(em_ble_rxdesc_rxralptr_t) == 2);

#endif /* HAL_EM_BLE_RXDESC_REG_H */
