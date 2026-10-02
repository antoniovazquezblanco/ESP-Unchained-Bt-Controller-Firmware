/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy register definitions.
 */
#ifndef HAL_EM_BLE_CS_REG_H
#define HAL_EM_BLE_CS_REG_H

#include <stdint.h>
#include <assert.h>

/**
 * CNTL FORMAT
 */
typedef enum __attribute__((packed))
{
    EM_BLE_CNTL_FMT_LLD_OFF = 0x00,              /**< Off */
    EM_BLE_CNTL_FMT_LLD_MASTER_CONNECTED = 0x02, /**< Master Connect */
    EM_BLE_CNTL_FMT_LLD_SLAVE_CONNECTED = 0x03,  /**< Slave Connect */
    EM_BLE_CNTL_FMT_LLD_LD_ADVERTISER = 0x04,    /**< Low Duty Cycle Advertiser */
    EM_BLE_CNTL_FMT_LLD_HD_ADVERTISER = 0x05,    /**< High Duty Cycle Advertiser */
    EM_BLE_CNTL_FMT_LLD_PASSIVE_SCANNING = 0x08, /**< Passive Scanner */
    EM_BLE_CNTL_FMT_LLD_ACTIVE_SCANNING = 0x09,  /**< Active Scanner */
    EM_BLE_CNTL_FMT_LLD_INITIATING = 0x0f,       /**< Initiator */
    EM_BLE_CNTL_FMT_LLD_TXTEST_MODE = 0x1c,      /**< Tx Test Mode */
    EM_BLE_CNTL_FMT_LLD_RXTEST_MODE = 0x1d,      /**< Rx Test Mode */
    EM_BLE_CNTL_FMT_LLD_TXRXTEST_MODE = 0x1e,    /**< Tx / Rx Test Mode */
} em_ble_cs_cntl_format_t;

/**
 * CNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:04               FORMAT   0x00
 *  05:07                    -
 *     08              DNABORT   0x00
 *     09             RXBSY_EN   0x00
 *     10             TXBSY_EN   0x00
 *     11                    -
 *  12:15                  PTI   0x00
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        em_ble_cs_cntl_format_t format : 5;
        unsigned short unk1 : 3;     // Unused bits
        unsigned short dnabort : 1;  // Do not abort TX enable
        unsigned short rxbsy_en : 1; // RX busy signal enable
        unsigned short txbsy_en : 1; // TX busy signal enable
        unsigned short unk2 : 1;     // Unused bits
        unsigned short pti : 4;      // Priority Threshold?/Tx? Interrupt?. May be related with coexistence. The 0xf value sets hardware default priority.
    } fields;
} em_ble_cs_cntl_t;
static_assert(sizeof(em_ble_cs_cntl_t) == 2);

/**
 * FCNTOFFSET register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:09           FCNTOFFSET   0x0
 *  10:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short fcntoffset : 10; // Finetimer counter offset
        unsigned short : 6;
    } fields;
} em_ble_cs_fcntoffset_t;
static_assert(sizeof(em_ble_cs_fcntoffset_t) == 2);

/**
 * LINK register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:04              LINKLBL   0
 *  05:07                    -
 *     08           RXCRYPT_EN   0
 *     09           TXCRYPT_EN   0
 *     10           CRYPT_MODE   0
 *     11        NULLRXLLIDFLT   0
 *  12:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short linklbl : 5;
        unsigned short : 3;
        unsigned short rxcrypt_en : 1;
        unsigned short txcrypt_en : 1;
        unsigned short crypt_mode : 1;
        unsigned short nullrxllidflt : 1; // Null LLID filtering
        unsigned short : 4;
    } fields;
} em_ble_cs_link_t;
static_assert(sizeof(em_ble_cs_link_t) == 2);

/**
 * SYNCWL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            SYNCWORDL   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short syncwordl : 16;
    } fields;
} em_ble_cs_syncwl_t;
static_assert(sizeof(em_ble_cs_syncwl_t) == 2);

/**
 * SYNCWH register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            SYNCWORDH   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short syncwordh : 16;
    } fields;
} em_ble_cs_syncwh_t;
static_assert(sizeof(em_ble_cs_syncwh_t) == 2);

/**
 * CRCINIT0 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15             CRCINIT0   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short crcinit0 : 16;
    } fields;
} em_ble_cs_crcinit0_t;
static_assert(sizeof(em_ble_cs_crcinit0_t) == 2);

/**
 * CRCINIT1 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07             CRCINIT1   0x0
 *  08:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short crcinit1 : 8;
        unsigned short : 8;
    } fields;
} em_ble_cs_crcinit1_t;
static_assert(sizeof(em_ble_cs_crcinit1_t) == 2);

/**
 * FILTPOL_RALCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00               RAL_EN   0
 *     01             RAL_MODE   0
 *     02        LOCAL_RPA_SEL   0
 *  03:07                    -
 *  08:15        FILTER_POLICY   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short ral_en : 1;
        unsigned short ral_mode : 1;
        unsigned short local_rpa_sel : 1;
        unsigned short : 5;
        unsigned short filter_policy : 8;
    } fields;
} em_ble_cs_filtpol_ralcntl_t;
static_assert(sizeof(em_ble_cs_filtpol_ralcntl_t) == 2);

/**
 * HOPCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:05               CH_IDX   0x0
 *  06:07                    -
 *  08:12              HOP_INT   0x0
 *  13:14                    -
 *     15                FH_EN   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short ch_idx : 6; // Current channel index
        unsigned short : 2;
        unsigned short hop_int : 5; // Channel hop interval
        unsigned short : 2;
        unsigned short fh_en : 1; // Frequency hop enable
    } fields;
} em_ble_cs_hopcntl_t;
static_assert(sizeof(em_ble_cs_hopcntl_t) == 2);

/**
 * TXRXCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07                TXPWR   0
 *  08:10                    -
 *     11           RXBFMICERR   0
 *     12                 NESN   0
 *     13                   SN   0
 *     14            LASTEMPTY   0
 *     15          RXBUFF_FULL   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txpwr : 8;
        unsigned short : 3;
        unsigned short rxbfmicerr : 1;
        unsigned short nesn : 1;
        unsigned short sn : 1;
        unsigned short lastempty : 1;
        unsigned short rxbuff_full : 1;
    } fields;
} em_ble_cs_txrxcntl_t;
static_assert(sizeof(em_ble_cs_txrxcntl_t) == 2);

/**
 * RXWINCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:13              RXWINSZ   0x0
 *     14                    -
 *     15               RXWIDE   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxwinsz : 14;
        unsigned short : 1;
        unsigned short rxwide : 1;
    } fields;
} em_ble_cs_rxwincntl_t;
static_assert(sizeof(em_ble_cs_rxwincntl_t) == 2);

/**
 * TXDESCPTR register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:14            TXDESCPTR   0x0
 *     15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txdescptr : 15;
        unsigned short : 1;
    } fields;
} em_ble_cs_txdescptr_t;
static_assert(sizeof(em_ble_cs_txdescptr_t) == 2);

/**
 * WINOFFSET register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            WINOFFSET   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short winoffset : 16;
    } fields;
} em_ble_cs_winoffset_t;
static_assert(sizeof(em_ble_cs_winoffset_t) == 2);

/**
 * MAXEVTIME register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            MAXEVTIME   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short maxevtime : 16;
    } fields;
} em_ble_cs_maxevtime_t;
static_assert(sizeof(em_ble_cs_maxevtime_t) == 2);

/**
 * CHMAP0 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15             LLCHMAP0   0xFFFF
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short llchmap0 : 16;
    } fields;
} em_ble_cs_chmap0_t;
static_assert(sizeof(em_ble_cs_chmap0_t) == 2);

/**
 * CHMAP1 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15             LLCHMAP1   0xFFFF
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short llchmap1 : 16;
    } fields;
} em_ble_cs_chmap1_t;
static_assert(sizeof(em_ble_cs_chmap1_t) == 2);

/**
 * CHMAP2 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:04             LLCHMAP2   0x1F
 *  05:07                    -
 *  08:13             NBCHGOOD   0x25
 *  14:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short llchmap2 : 5;
        unsigned short : 3;
        unsigned short nbchgood : 6;
        unsigned short : 2;
    } fields;
} em_ble_cs_chmap2_t;
static_assert(sizeof(em_ble_cs_chmap2_t) == 2);

/**
 * RXMAXBUF register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07             RXMAXBUF   0x0
 *  08:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxmaxbuf : 8;
        unsigned short : 8;
    } fields;
} em_ble_cs_rxmaxbuf_t;
static_assert(sizeof(em_ble_cs_rxmaxbuf_t) == 2);

/**
 * RXMAXTIME register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:12            RXMAXTIME   0x0
 *  13:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxmaxtime : 13;
        unsigned short : 3;
    } fields;
} em_ble_cs_rxmaxtime_t;
static_assert(sizeof(em_ble_cs_rxmaxtime_t) == 2);

/**
 * PEER_RALPTR register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15          PEER_RALPTR   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short peer_ralptr : 16;
    } fields;
} em_ble_cs_peer_ralptr_t;
static_assert(sizeof(em_ble_cs_peer_ralptr_t) == 2);

/**
 * ADV_BD_ADDR_TYPE register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00     ADV_BD_ADDR_TYPE   0
 *  01:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short adv_bd_addr_type : 1;
        unsigned short : 15;
    } fields;
} em_ble_cs_adv_bd_addr_type_t;
static_assert(sizeof(em_ble_cs_adv_bd_addr_type_t) == 2);

/**
 * TXCCMPKTCNT0 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15         TXCCMPKTCNT0   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txccmpktcnt0 : 16;
    } fields;
} em_ble_cs_txccmpktcnt0_t;
static_assert(sizeof(em_ble_cs_txccmpktcnt0_t) == 2);

/**
 * TXCCMPKTCNT1 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15         TXCCMPKTCNT1   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txccmpktcnt1 : 16;
    } fields;
} em_ble_cs_txccmpktcnt1_t;
static_assert(sizeof(em_ble_cs_txccmpktcnt1_t) == 2);

/**
 * TXCCMPKTCNT2 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:06         TXCCMPKTCNT2   0x0
 *  07:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txccmpktcnt2 : 7;
        unsigned short : 9;
    } fields;
} em_ble_cs_txccmpktcnt2_t;
static_assert(sizeof(em_ble_cs_txccmpktcnt2_t) == 2);

/**
 * RXCCMPKTCNT0 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15         RXCCMPKTCNT0   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxccmpktcnt0 : 16;
    } fields;
} em_ble_cs_rxccmpktcnt0_t;
static_assert(sizeof(em_ble_cs_rxccmpktcnt0_t) == 2);

/**
 * RXCCMPKTCNT1 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15         RXCCMPKTCNT1   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxccmpktcnt1 : 16;
    } fields;
} em_ble_cs_rxccmpktcnt1_t;
static_assert(sizeof(em_ble_cs_rxccmpktcnt1_t) == 2);

/**
 * RXCCMPKTCNT2 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:06         RXCCMPKTCNT2   0x0
 *  07:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short rxccmpktcnt2 : 7;
        unsigned short : 9;
    } fields;
} em_ble_cs_rxccmpktcnt2_t;
static_assert(sizeof(em_ble_cs_rxccmpktcnt2_t) == 2);

/**
 * BTCNTSYNC0 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15           BTCNTSYNC0   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short btcntsync0 : 16;
    } fields;
} em_ble_cs_btcntsync0_t;
static_assert(sizeof(em_ble_cs_btcntsync0_t) == 2);

/**
 * BTCNTSYNC1 register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:10           BTCNTSYNC1   0x0
 *  11:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short btcntsync1 : 11;
        unsigned short : 5;
    } fields;
} em_ble_cs_btcntsync1_t;
static_assert(sizeof(em_ble_cs_btcntsync1_t) == 2);

/**
 * FCNTSYNC register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:09           FCNTRXSYNC   0x0
 *  10:14                    -
 *     15              EVTRXOK   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short fcntrxsync : 10;
        unsigned short : 5;
        unsigned short evtrxok : 1;
    } fields;
} em_ble_cs_fcntsync_t;
static_assert(sizeof(em_ble_cs_fcntsync_t) == 2);

/**
 * TXRXDESCCNT register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07         ACLTXDESCCNT   0x0
 *  08:15         ACLRXDESCCNT   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short acltxdesccnt : 8;
        unsigned short aclrxdesccnt : 8; // Number of RX descriptors with pending information.
    } fields;
} em_ble_cs_txrxdesccnt_t;
static_assert(sizeof(em_ble_cs_txrxdesccnt_t) == 2);

/**
 * ISOTXRXCNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00            ISOLASTSN   0
 *     01          ISOLASTNESN   0
 *     02            ISOLASTMD   0
 *     03           ISOWAITACK   0
 *  04:07                    -
 *     08              ISORSVD   0
 *     09              ISORETX   0
 *     10                    -
 *     11              ISONESN   0
 *     12                ISOSN   0
 *     13                ISOMD   0
 *     14         ISOLASTEMPTY   0
 *     15       ISORXBUFF_FULL   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short isolastsn : 1;
        unsigned short isolastnesn : 1;
        unsigned short isolastmd : 1;
        unsigned short isowaitack : 1;
        unsigned short : 4;
        unsigned short isorsvd : 1;
        unsigned short isoretx : 1;
        unsigned short : 1;
        unsigned short isonesn : 1;
        unsigned short isosn : 1;
        unsigned short isomd : 1;
        unsigned short isolastempty : 1;
        unsigned short isorxbuff_full : 1;
    } fields;
} em_ble_cs_isotxrxcntl_t;
static_assert(sizeof(em_ble_cs_isotxrxcntl_t) == 2);

/**
 * THRCNTL_RATECNTL register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:01               TXRATE   0x0
 *  02:03               RXRATE   0x0
 *  04:11                    -
 *  12:15                RXTHR   0x0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short txrate : 2;
        unsigned short rxrate : 2;
        unsigned short : 8;
        unsigned short rxthr : 4;
    } fields;
} em_ble_cs_thrcntl_ratecntl_t;
static_assert(sizeof(em_ble_cs_thrcntl_ratecntl_t) == 2);

#endif /* HAL_EM_BLE_CS_REG_H */
