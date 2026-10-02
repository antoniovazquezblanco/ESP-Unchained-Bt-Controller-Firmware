/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy core register definitions.
 */
#ifndef HAL_CORE_BLE_REG_H
#define HAL_CORE_BLE_REG_H

#include <stdint.h>
#include <assert.h>

/**
 * RWBLECNTL register definition.
 * This is a BLE peripheral  control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:02              SYNCERR   0x0
 *  04:07           RXWINSZDEF   0x0
 *     08             RWBLE_EN   0
 *     09        ADVERTFILT_EN   0
 *  10:15                    -
 *     16        HOP_REMAP_DSB   0
 *     17              CRC_DSB   0
 *     18             WHIT_DSB   0
 *     19            CRYPT_DSB   0
 *     20             NESN_DSB   0
 *     21               SN_DSB   0
 *     22               MD_DSB   0
 *     23                    -
 *     24           SCAN_ABORT   0
 *     25         ADVERT_ABORT   0
 *     26         RFTEST_ABORT   0
 *     27                    -
 *     28            SWINT_REQ   0
 *     29         REG_SOFT_RST   0
 *     30    MASTER_TGSOFT_RST   0
 *     31      MASTER_SOFT_RST   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned syncerr : 3;
        unsigned rxwinszdef : 4;
        unsigned rwble_en : 1; /**< Core enable/disable */
        unsigned advertfilt_en : 1;
        unsigned : 6;
        unsigned hop_remap_dsb : 1;
        unsigned crc_dsb : 1;
        unsigned whit_dsb : 1; /**< Whitening disable */
        unsigned crypt_dsb : 1;
        unsigned nesn_dsb : 1;
        unsigned sn_dsb : 1;
        unsigned md_dsb : 1;
        unsigned : 1;
        unsigned scan_abort : 1;
        unsigned advert_abort : 1;
        unsigned rftest_abort : 1;
        unsigned : 1;
        unsigned swint_req : 1;
        unsigned reg_soft_rst : 1;
        unsigned master_tgsoft_rst : 1; /**< Master timing generator reset */
        unsigned master_soft_rst : 1;   /**< Master state machine reset */
    } fields;
} core_ble_rwblecntl_t;
static_assert(sizeof(core_ble_rwblecntl_t) == 4);

/**
 * VERSION register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07                BUILD   0x0
 *  08:15                  UPG   0x9
 *  16:23                  REL   0x0
 *  24:31                  TYP   0x8
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned build : 8;
        unsigned upg : 8;
        unsigned rel : 8;
        unsigned typ : 8;
    } fields;
} core_ble_version_t;
static_assert(sizeof(core_ble_version_t) == 4);

/**
 * RWBLECONF register definition.
 * This is a configuration register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:04           ADDR_WIDTH   0xD
 *     05           DATA_WIDTH   0
 *     06             BUS_TYPE   0
 *     07              INTMODE   1
 *  08:13              CLK_SEL   0xD
 *     14             USECRYPT   0
 *     15               USEDBG   1
 *  16:20                 RFIF   0x1
 *     21             WLANCOEX   1
 *     23             DECIPHER   1
 *  24:25            ISOPORTNB   0x3
 *  26:30                    -
 *     31               DMMODE   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned addr_width : 5;
        unsigned data_width : 1;
        unsigned bus_type : 1;
        unsigned intmode : 1;
        unsigned clk_sel : 6;
        unsigned usecrypt : 1;
        unsigned usedbg : 1;
        unsigned rfif : 5;
        unsigned wlancoex : 1;
        unsigned decipher : 1;
        unsigned isoportnb : 2;
        unsigned : 5;
        unsigned dmmode : 1;
    } fields;
} core_ble_rwbleconf_t;
static_assert(sizeof(core_ble_rwbleconf_t) == 4);

/**
 * INTCNTL register definition.
 * Interrupt controller control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00          CSCNTINTMSK   1
 *     01             RXINTMSK   1
 *     02            SLPINTMSK   1
 *     03          EVENTINTMSK   1
 *     04          CRYPTINTMSK   1
 *     05          ERRORINTMSK   0
 *     06     GROSSTGTIMINTMSK   0
 *     07      FINETGTIMINTMSK   0
 *     08      EVENTAPFAINTMSK   1
 *     09             SWINTMSK   0
 *     10         AUDIOINT0MSK   0
 *     11         AUDIOINT1MSK   0
 *     12         AUDIOINT2MSK   0
 *  13:14                    -
 *     15          CSCNTDEVMSK   1
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned cscntintmsk : 1;
        unsigned rxintmsk : 1;
        unsigned slpintmsk : 1;
        unsigned eventintmsk : 1;
        unsigned cryptintmsk : 1;
        unsigned errorintmsk : 1;
        unsigned grosstgtimintmsk : 1;
        unsigned finetgtimintmsk : 1;
        unsigned eventapfaintmsk : 1;
        unsigned swintmsk : 1;
        unsigned audioint0msk : 1;
        unsigned audioint1msk : 1;
        unsigned audioint2msk : 1;
        unsigned : 2;
        unsigned cscntdevmsk : 1;
        unsigned : 16;
    } fields;
} core_ble_intcntl_t;
static_assert(sizeof(core_ble_intcntl_t) == 4);

/**
 * INTSTAT register definition.
 * Interrupt status register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00         CSCNTINTSTAT   0
 *     01            RXINTSTAT   0
 *     02           SLPINTSTAT   0
 *     03         EVENTINTSTAT   0
 *     04         CRYPTINTSTAT   0
 *     05         ERRORINTSTAT   0
 *     06    GROSSTGTIMINTSTAT   0
 *     07     FINETGTIMINTSTAT   0
 *     08     EVENTAPFAINTSTAT   0
 *     09            SWINTSTAT   0
 *     10        AUDIOINT0STAT   0
 *     11        AUDIOINT1STAT   0
 *     12        AUDIOINT2STAT   0
 *  13:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned cscntintstat : 1;
        unsigned rxintstat : 1;
        unsigned slpintstat : 1;
        unsigned eventintstat : 1;
        unsigned cryptintstat : 1;
        unsigned errorintstat : 1;
        unsigned grosstgtimintstat : 1;
        unsigned finetgtimintstat : 1;
        unsigned eventapfaintstat : 1;
        unsigned swintstat : 1;
        unsigned audioint0stat : 1;
        unsigned audioint1stat : 1;
        unsigned audioint2stat : 1;
        unsigned : 19;
    } fields;
} core_ble_intstat_t;
static_assert(sizeof(core_ble_intstat_t) == 4);

/**
 * INTRAWSTAT register definition.
 * Interrupt raw status register.
 *
 *   Bits             Field Name   Reset Value
 *  -----   --------------------   -----------
 *     00        CSCNTINTRAWSTAT   0
 *     01           RXINTRAWSTAT   0
 *     02          SLPINTRAWSTAT   0
 *     03        EVENTINTRAWSTAT   0
 *     04        CRYPTINTRAWSTAT   0
 *     05        ERRORINTRAWSTAT   0
 *     06   GROSSTGTIMINTRAWSTAT   0
 *     07    FINETGTIMINTRAWSTAT   0
 *     08    EVENTAPFAINTRAWSTAT   0
 *     09           SWINTRAWSTAT   0
 *     10       AUDIOINT0RAWSTAT   0
 *     11       AUDIOINT1RAWSTAT   0
 *     12       AUDIOINT2RAWSTAT   0
 *  13:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned cscntintrawstat : 1;
        unsigned rxintrawstat : 1;
        unsigned slpintrawstat : 1;
        unsigned eventintrawstat : 1;
        unsigned cryptintrawstat : 1;
        unsigned errorintrawstat : 1;
        unsigned grosstgtimintrawstat : 1;
        unsigned finetgtimintrawstat : 1;
        unsigned eventapfaintrawstat : 1;
        unsigned swintrawstat : 1;
        unsigned audioint0rawstat : 1;
        unsigned audioint1rawstat : 1;
        unsigned audioint2rawstat : 1;
        unsigned : 19;
    } fields;
} core_ble_intrawstat_t;
static_assert(sizeof(core_ble_intrawstat_t) == 4);

/**
 * INTACK register definition.
 * Interrupt acknowledge register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00          CSCNTINTACK   0
 *     01             RXINTACK   0
 *     02            SLPINTACK   0
 *     03          EVENTINTACK   0
 *     04          CRYPTINTACK   0
 *     05          ERRORINTACK   0
 *     06     GROSSTGTIMINTACK   0
 *     07      FINETGTIMINTACK   0
 *     08      EVENTAPFAINTACK   0
 *     09             SWINTACK   0
 *     10         AUDIOINT0ACK   0
 *     11         AUDIOINT1ACK   0
 *     12         AUDIOINT2ACK   0
 *  13:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned cscntintack : 1;
        unsigned rxintack : 1;
        unsigned slpintack : 1;
        unsigned eventintack : 1;
        unsigned cryptintack : 1;
        unsigned errorintack : 1;
        unsigned grosstgtimintack : 1;
        unsigned finetgtimintack : 1;
        unsigned eventapfaintack : 1;
        unsigned swintack : 1;
        unsigned audioint0ack : 1;
        unsigned audioint1ack : 1;
        unsigned audioint2ack : 1;
        unsigned : 19;
    } fields;
} core_ble_intack_t;
static_assert(sizeof(core_ble_intack_t) == 4);

/**
 * BASETIMECNT register definition.
 * Base time reference counter.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:26          BASETIMECNT   0x0
 *  27:30                    -
 *     31                 SAMP   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned basetimecnt : 27;
        unsigned : 4;
        unsigned samp : 1;
    } fields;
} core_ble_basetimecnt_t;
static_assert(sizeof(core_ble_basetimecnt_t) == 4);

/**
 * FINETIMECNT register definition.
 * Fine time reference counter.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:09              FINECNT   0x0
 *  10:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned finecnt : 10;
        unsigned : 12;
    } fields;
} core_ble_finetimecnt_t;
static_assert(sizeof(core_ble_finetimecnt_t) == 4);

/**
 * BDADDRL register definition.
 * BLE device address LSB register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:31              BDADDRL   0x0
 */
typedef uint32_t core_ble_bdaddrl_t;
static_assert(sizeof(core_ble_bdaddrl_t) == 4);

/**
 * BDADDRU register definition.
 * BLE device address MSB register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15              BDADDRU   0x0
 *     16            PRIV_NPUB   0
 *  17:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned bdaddru : 16;
        unsigned priv_npub : 1;
        unsigned : 15;
    } fields;
} core_ble_bdaddru_t;
static_assert(sizeof(core_ble_bdaddru_t) == 4);

/**
 * ET_CURRENTRXDESCPTR register definition.
 * RX descriptor pointer for the receive buffer chained list.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:14     CURRENTRXDESCPTR   0x0
 *     15                    -
 *  16:31                ETPTR   0x0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned currentrxdescptr : 15;
        unsigned : 1;
        unsigned etptr : 15;
    } fields;
} core_ble_et_currentrxdescptr_t;
static_assert(sizeof(core_ble_et_currentrxdescptr_t) == 4);

/**
 * DIAGCNTL register definition.
 * Diagnostics control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:05                DIAG0   0x0
 *     06                    -
 *     07             DIAG0_EN   0
 *  08:13                DIAG1   0x0
 *     14                    -
 *     15             DIAG1_EN   0
 *  16:21                DIAG2   0x0
 *     22                    -
 *     23             DIAG2_EN   0
 *  24:29                DIAG3   0x0
 *     30                    -
 *     31             DIAG3_EN   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned diag0 : 6;
        unsigned : 1;
        unsigned diag0_en : 1;
        unsigned diag1 : 6;
        unsigned : 1;
        unsigned diag1_en : 1;
        unsigned diag2 : 6;
        unsigned : 1;
        unsigned diag2_en : 1;
        unsigned diag3 : 6;
        unsigned : 1;
        unsigned diag3_en : 1;
    } fields;
} core_ble_diagcntl_t;
static_assert(sizeof(core_ble_diagcntl_t) == 4);

/**
 * DIAGSTAT register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07            DIAG0STAT   0x0
 *  08:15            DIAG1STAT   0x0
 *  16:23            DIAG2STAT   0x0
 *  24:31            DIAG3STAT   0x0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned diag0stat : 8;
        unsigned diag1stat : 8;
        unsigned diag2stat : 8;
        unsigned diag3stat : 8;
    } fields;
} core_ble_diagstat_t;
static_assert(sizeof(core_ble_diagstat_t) == 4);

/**
 * DEBUGADDMAX register definition.
 * Upper limit for the memory zone.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            EM_ADDMAX   0x0
 *  16:31           REG_ADDMAX   0x0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned em_addmax : 16;
        unsigned reg_addmax : 16;
    } fields;
} core_ble_debugaddmax_t;
static_assert(sizeof(core_ble_debugaddmax_t) == 4);

/**
 * DEBUGADDMIN register definition.
 * Lower limit for the memory zone.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15            EM_ADDMIN   0x0
 *  16:31           REG_ADDMIN   0x0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned em_addmin : 16;
        unsigned reg_addmin : 16;
    } fields;
} core_ble_debugaddmin_t;
static_assert(sizeof(core_ble_debugaddmin_t) == 4);

/**
 * ERRORTYPESTAT register definition.
 * Error type status register.
 *
 *   Bits             Field Name   Reset Value
 *  -----   ---------------------   -----------
 *     00           TXCRYPT_ERROR   0
 *     01           RXCRYPT_ERROR   0
 *     02     PKTCNTL_EMACC_ERROR   0
 *     03       RADIO_EMACC_ERROR   0
 *     04   EVT_SCHDL_EMACC_ERROR   0
 *     05   EVT_SCHDL_ENTRY_ERROR   0
 *     06    EVT_SCHDL_APFM_ERROR   0
 *     07     EVT_CNTL_APFM_ERROR   0
 *     08         WHITELIST_ERROR   0
 *     09            IFS_UNDERRUN   0
 *     10            ADV_UNDERRUN   0
 *     11           LLCHMAP_ERROR   0
 *     12          CSFORMAT_ERROR   0
 *     13      TXDESC_EMPTY_ERROR   0
 *     14      RXDESC_EMPTY_ERROR   0
 *     15        TXDATA_PTR_ERROR   0
 *     16        RXDATA_PTR_ERROR   0
 *     17        CONCEVTIRQ_ERROR   0
 *     18               RAL_ERROR   0
 *     19            RAL_UNDERRUN   0
 *  20:31                       -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned txcrypt_error : 1;
        unsigned rxcrypt_error : 1;
        unsigned pktcntl_emacc_error : 1;
        unsigned radio_emacc_error : 1;
        unsigned evt_schdl_emacc_error : 1;
        unsigned evt_schdl_entry_error : 1;
        unsigned evt_schdl_apfm_error : 1;
        unsigned evt_cntl_apfm_error : 1;
        unsigned whitelist_error : 1;
        unsigned ifs_underrun : 1;
        unsigned adv_underrun : 1;
        unsigned llchmap_error : 1;
        unsigned csformat_error : 1;
        unsigned txdesc_empty_error : 1;
        unsigned rxdesc_empty_error : 1;
        unsigned txdata_ptr_error : 1;
        unsigned rxdata_ptr_error : 1;
        unsigned concevtirq_error : 1;
        unsigned ral_error : 1;
        unsigned ral_underrun : 1;
        unsigned : 12;
    } fields;
} core_ble_errortypestat_t;
static_assert(sizeof(core_ble_errortypestat_t) == 4);

/**
 * SWPROFILING register definition.
 * Software profiling register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00              SWPROF0   0
 *     01              SWPROF1   0
 *     02              SWPROF2   0
 *     03              SWPROF3   0
 *     04              SWPROF4   0
 *     05              SWPROF5   0
 *     06              SWPROF6   0
 *     07              SWPROF7   0
 *     08              SWPROF8   0
 *     09              SWPROF9   0
 *     10             SWPROF10   0
 *     11             SWPROF11   0
 *     12             SWPROF12   0
 *     13             SWPROF13   0
 *     14             SWPROF14   0
 *     15             SWPROF15   0
 *     16             SWPROF16   0
 *     17             SWPROF17   0
 *     18             SWPROF18   0
 *     19             SWPROF19   0
 *     20             SWPROF20   0
 *     21             SWPROF21   0
 *     22             SWPROF22   0
 *     23             SWPROF23   0
 *     24             SWPROF24   0
 *     25             SWPROF25   0
 *     26             SWPROF26   0
 *     27             SWPROF27   0
 *     28             SWPROF28   0
 *     29             SWPROF29   0
 *     30             SWPROF30   0
 *     31             SWPROF31   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned swprof0 : 1;
        unsigned swprof1 : 1;
        unsigned swprof2 : 1;
        unsigned swprof3 : 1;
        unsigned swprof4 : 1;
        unsigned swprof5 : 1;
        unsigned swprof6 : 1;
        unsigned swprof7 : 1;
        unsigned swprof8 : 1;
        unsigned swprof9 : 1;
        unsigned swprof10 : 1;
        unsigned swprof11 : 1;
        unsigned swprof12 : 1;
        unsigned swprof13 : 1;
        unsigned swprof14 : 1;
        unsigned swprof15 : 1;
        unsigned swprof16 : 1;
        unsigned swprof17 : 1;
        unsigned swprof18 : 1;
        unsigned swprof19 : 1;
        unsigned swprof20 : 1;
        unsigned swprof21 : 1;
        unsigned swprof22 : 1;
        unsigned swprof23 : 1;
        unsigned swprof24 : 1;
        unsigned swprof25 : 1;
        unsigned swprof26 : 1;
        unsigned swprof27 : 1;
        unsigned swprof28 : 1;
        unsigned swprof29 : 1;
        unsigned swprof30 : 1;
        unsigned swprof31 : 1;
    } fields;
} core_ble_swprofiling_t;
static_assert(sizeof(core_ble_swprofiling_t) == 4);

/**
 * RADIOCNTL0 register definition.
 * Radio interface control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00                SPIGO   0
 *     01              SPICOMP   1
 *  02:03                    -
 *  04:05              SPIFREQ   0x0
 *  16:31               SPIPTR   0x0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned spigo : 1;
        unsigned spicomp : 1;
        unsigned : 2;
        unsigned spifreq : 2;
        unsigned spiptr : 26;
    } fields;
} core_ble_radiocntl0_t;
static_assert(sizeof(core_ble_radiocntl0_t) == 4);

/**
 * RADIOCNTL1 register definition.
 * Radio interface control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:03           SUBVERSION   0x0
 *  04:08               XRFSEL   0x0
 *  09:11                    -
 *     12           JEF_SELECT   0
 *     13            DPCORR_EN   0
 *     14                    -
 *     15      SYNC_PULSE_MODE   0
 *  16:27      FORCEAGC_LENGTH   0x0
 *  28:29                    -
 *     30           FORCEBLEIQ   0
 *     31          FORCEAGC_EN   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned subversion : 4;
        unsigned xrfsel : 5;
        unsigned : 2;
        unsigned jef_select : 1;
        unsigned dpcorr_en : 1;
        unsigned : 1;
        unsigned forceagc_length : 12;
        unsigned : 2;
        unsigned forcebleiq : 1;
        unsigned forceagc_en : 1;
    } fields;
} core_ble_radiocntl1_t;
static_assert(sizeof(core_ble_radiocntl1_t) == 4);

/**
 * RADIOPWRUPDN0 register definition.
 * RX/TX power up/down phase register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07             TXPWRUP0   0x0
 *  08:12             TXPWRDN0   0x0
 *  13:15                    -
 *  16:23             RXPWRUP0   0x0
 *  24:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned txpwrup0 : 8;
        unsigned txpwrdn0 : 5;
        unsigned : 3;
        unsigned rxpwrup0 : 8;
        unsigned : 8;
    } fields;
} core_ble_radiopwrupdn0_t;
static_assert(sizeof(core_ble_radiopwrupdn0_t) == 4);

/**
 * ADVCHMAP register definition.
 * Advertising channel map.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:02             ADVCHMAP   0x7
 *  03:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned advchmap : 3;
        unsigned : 29;
    } fields;
} core_ble_advchmap_t;
static_assert(sizeof(core_ble_advchmap_t) == 4);

/**
 * ADVTIM register definition.
 * Advertising packet interval.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:13               ADVINT   0x0
 *  14:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned advint : 14;
        unsigned : 18;
    } fields;
} core_ble_advtim_t;
static_assert(sizeof(core_ble_advtim_t) == 4);

/**
 * ACTSCANSTAT register definition.
 * Active scan status register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:08           UPPERLIMIT   0x1
 *  09:15                    -
 *  16:24              BACKOFF   0x1
 *  25:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned upperlimit : 9;
        unsigned : 7;
        unsigned backoff : 9;
        unsigned : 7;
    } fields;
} core_ble_actscanstat_t;
static_assert(sizeof(core_ble_actscanstat_t) == 4);

/**
 * WLPUBADDPTR register definition.
 * Start address of public devices list.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15          WLPUBADDPTR   0x0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned wlpubaddptr : 16;
        unsigned : 16;
    } fields;
} core_ble_wlpubaddptr_t;
static_assert(sizeof(core_ble_wlpubaddptr_t) == 4);

/**
 * WLPRIVADDPTR register definition.
 * Start address of private devices list.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15         WLPRIVADDPTR   0x0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned wlprivaddptr : 16;
        unsigned : 16;
    } fields;
} core_ble_wlprivaddptr_t;
static_assert(sizeof(core_ble_wlprivaddptr_t) == 4);

/**
 * WLNBDEV register definition.
 * Devices in white list.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07             NBPUBDEV   0x0
 *  08:15            NBPRIVDEV   0x0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned nbpubdev : 8;
        unsigned nbprivdev : 8;
        unsigned : 16;
    } fields;
} core_ble_wlnbdev_t;
static_assert(sizeof(core_ble_wlnbdev_t) == 4);

/**
 * AESCNTL register definition.
 * Start AES register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00            AES_START   0
 *     01             AES_MODE   0
 *  02:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned aes_start : 1;
        unsigned aes_mode : 1;
        unsigned : 30;
    } fields;
} core_ble_aescntl_t;
static_assert(sizeof(core_ble_aescntl_t) == 4);

/**
 * AESKEY31_0 register definition.
 * AES encryption key.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:31           AESKEY31_0   0x0
 */
typedef uint32_t core_ble_aeskey31_0_t;
static_assert(sizeof(core_ble_aeskey31_0_t) == 4);

/**
 * AESKEY63_32 register definition.
 * AES encryption key.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:31          AESKEY63_32   0x0
 */
typedef uint32_t core_ble_aeskey63_32_t;
static_assert(sizeof(core_ble_aeskey63_32_t) == 4);

/**
 * AESKEY95_64 register definition.
 * AES encryption key.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:31          AESKEY95_64   0x0
 */
typedef uint32_t core_ble_aeskey95_64_t;
static_assert(sizeof(core_ble_aeskey95_64_t) == 4);

/**
 * AESKEY127_96 register definition.
 * AES encryption key.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:31         AESKEY127_96   0x0
 */
typedef uint32_t core_ble_aeskey127_96_t;
static_assert(sizeof(core_ble_aeskey127_96_t) == 4);

/**
 * AESPTR register definition.
 * Pointer to the block to encrypt/decrypt.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15               AESPTR   0x0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned aesptr : 16;
        unsigned : 16;
    } fields;
} core_ble_aesptr_t;
static_assert(sizeof(core_ble_aesptr_t) == 4);

/**
 * RFTESTCNTL register definition.
 * RF testing control register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:08             TXLENGTH   0x0
 *  09:10                    -
 *     11           TXPKTCNTEN   0
 *     12             TXPLDSRC   0
 *     13             PRBSTYPE   0
 *     14          TXLENGTHSRC   0
 *     15           INFINITETX   0
 *  16:26                    -
 *     27           RXPKTCNTEN   0
 *  28:30                    -
 *     31           INFINITERX   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned txlength : 9;
        unsigned : 2;
        unsigned txpktcnten : 1;
        unsigned txpldsrc : 1;
        unsigned prbstype : 1;
        unsigned txlengthsrc : 1;
        unsigned infinitetx : 1;
        unsigned : 11;
        unsigned rxpktcnten : 1;
        unsigned : 3;
        unsigned infiniterx : 1;
    } fields;
} core_ble_rftestcntl_t;
static_assert(sizeof(core_ble_rftestcntl_t) == 4);

/**
 * TIMGENCNTL register definition.
 * Timing generator register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:08        PREFETCH_TIME   0x96
 *  09:15                    -
 *  16:25   PREFETCHABORT_TIME   0x1FE
 *  26:30                    -
 *     31              APFM_EN   1
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned prefetch_time : 9;
        unsigned : 7;
        unsigned prefetchabort_time : 10;
        unsigned : 5;
        unsigned apfm_en : 1;
    } fields;
} core_ble_timgencntl_t;
static_assert(sizeof(core_ble_timgencntl_t) == 4);

/**
 * COEXIFCNTL0 register definition.
 * Coexistence configuration register.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00          WLANCOEX_EN   0
 *     01           SYNCGEN_EN   0
 *     02           MWSCOEX_EN   0
 *     03            MWSWCI_EN   0
 *  04:05            WLANRXMSK   0x1
 *  06:07            WLANTXMSK   0x0
 *  08:09             MWSRXMSK   0x0
 *  10:11             MWSTXMSK   0x0
 *  12:13          MWSRXFRQMSK   0x0
 *  14:15          MWSTXFRQMSK   0x0
 *  16:17        WLCTXPRIOMODE   0x0
 *  18:19                    -
 *  20:21        WLCRXPRIOMODE   0x0
 *  22:23                    -
 *  24:25       MWSSCANFREQMSK   0x0
 *  26:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned wlancoex_en : 1;
        unsigned syncgen_en : 1;
        unsigned mwscoex_en : 1;
        unsigned mwswci_en : 1;
        unsigned wlanrxmsk : 2;
        unsigned wlantxmsk : 2;
        unsigned mwsrxmsk : 2;
        unsigned mwstxmsk : 2;
        unsigned mwsrxfrqmsk : 2;
        unsigned mwstxfrqmsk : 2;
        unsigned wlctxpriomode : 2;
        unsigned : 2;
        unsigned wlcrxpriomode : 2;
        unsigned : 2;
        unsigned mwsscanfreqmsk : 2;
        unsigned : 5;
    } fields;
} core_ble_coexifcntl0_t;
static_assert(sizeof(core_ble_coexifcntl0_t) == 4);

/**
 * RALPTR register definition.
 * Resolve address list pointer.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:15               RALPTR   0x0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned ralptr : 16;
        unsigned : 16;
    } fields;
} core_ble_ralptr_t;
static_assert(sizeof(core_ble_ralptr_t) == 4);

/**
 * RALNBDEV register definition.
 * Resolve address list number of devices.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07             NBRALDEV   0x0
 *  08:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned nbraldev : 8;
        unsigned : 24;
    } fields;
} core_ble_ralnbdev_t;
static_assert(sizeof(core_ble_ralnbdev_t) == 4);

/**
 * RAL_LOCAL_RND register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:21             LRND_VAL   0x3F0F0F
 *  22:30                    -
 *     31            LRND_INIT   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned lrnd_val : 22;
        unsigned : 9;
        unsigned lrnd_init : 1;
    } fields;
} core_ble_ral_local_rnd_t;
static_assert(sizeof(core_ble_ral_local_rnd_t) == 4);

/**
 * RAL_PEER_RND register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:21             PRND_VAL   0x30F0F0
 *  22:30                    -
 *     31            PRND_INIT   0
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned prnd_val : 22;
        unsigned : 9;
        unsigned prnd_init : 1;
    } fields;
} core_ble_ral_peer_rnd_t;
static_assert(sizeof(core_ble_ral_peer_rnd_t) == 4);

/**
 * BLEPRIOSCHARB register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:07            BLEMARGIN   0x0
 *  08:14                    -
 *     15          BLEPRIOMODE   0
 *  16:31                    -
 */
typedef union
{
    uint32_t raw;
    struct __attribute__((packed))
    {
        unsigned blemargin : 8;
        unsigned : 7;
        unsigned blepriomode : 1;
        unsigned : 16;
    } fields;
} core_ble_bleprioscharb_t;
static_assert(sizeof(core_ble_bleprioscharb_t) == 4);

#endif /* HAL_CORE_BLE_REG_H */
