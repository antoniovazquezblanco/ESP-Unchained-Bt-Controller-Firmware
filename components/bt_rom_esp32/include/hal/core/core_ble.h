/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy core registers structure.
 */
#ifndef HAL_CORE_BLE_H
#define HAL_CORE_BLE_H

#include <assert.h>
#include <stddef.h>

#include "hal/core/core_ble_reg.h"

typedef struct
{
    core_ble_rwblecntl_t rwblecntl;                     /**< BLE control register */
    core_ble_version_t version;                         /**< Version register */
    core_ble_rwbleconf_t rwbleconf;                     /**< Configuration register */
    core_ble_intcntl_t intcntl;                         /**< Interrupt controller register */
    core_ble_intstat_t intstat;                         /**< Interrupt status register */
    core_ble_intrawstat_t intrawstat;                   /**< Interrupt raw status register */
    core_ble_intack_t intack;                           /**< Interrupt acknowledge register */
    core_ble_basetimecnt_t basetimecnt;                 /**< Base time reference counter */
    core_ble_finetimecnt_t finetimecnt;                 /**< Fine time reference counter */
    core_ble_bdaddrl_t bdaddrl;                         /**< BLE device address LSB register */
    core_ble_bdaddru_t bdaddru;                         /**< BLE device address MSB register */
    core_ble_et_currentrxdescptr_t et_currentrxdescptr; /**< RX descriptor pointer for the receive buffer chained list */
    uint32_t unk1;
    uint32_t unk2;
    uint32_t unk3;
    uint32_t unk4;
    uint32_t unk5;
    uint32_t unk6;
    uint32_t unk7;
    uint32_t unk8;
    core_ble_diagcntl_t diagcntl; /**< Diagnostics register */
    core_ble_diagstat_t diagstat;
    core_ble_debugaddmax_t debugaddmax;     /**< Upper limit for the memory zone */
    core_ble_debugaddmin_t debugaddmin;     /**< Lower limit for the memory zone */
    core_ble_errortypestat_t errortypestat; /**< Error type status register */
    core_ble_swprofiling_t swprofiling;     /**< Software profiling register */
    uint32_t unk9;
    uint32_t unk10;
    core_ble_radiocntl0_t radiocntl0; /**< Radio interface control register */
    core_ble_radiocntl1_t radiocntl1; /**< Radio interface control register */
    uint32_t unk11;
    uint32_t unk12;
    core_ble_radiopwrupdn0_t radiopwrupdn0; /**< RX/TX power up/down phase register */
    uint32_t unk13;
    uint32_t unk14;
    uint32_t unk15;
    core_ble_advchmap_t advchmap; /**< Advertising channel map */
    uint32_t unk16;
    uint32_t unk17;
    uint32_t unk18;
    core_ble_advtim_t advtim;           /**< Advertising packet interval */
    core_ble_actscanstat_t actscanstat; /**< Active scan register */
    uint32_t unk19;
    uint32_t unk20;
    core_ble_wlpubaddptr_t wlpubaddptr;   /**< Start address of public devices list */
    core_ble_wlprivaddptr_t wlprivaddptr; /**< Start address of private devices list */
    core_ble_wlnbdev_t wlnbdev;           /**< Devices in white list */
    uint32_t unk21;
    core_ble_aescntl_t aescntl;           /**< Start AES register */
    core_ble_aeskey31_0_t aeskey31_0;     /**< AES encryption key */
    core_ble_aeskey63_32_t aeskey63_32;   /**< AES encryption key */
    core_ble_aeskey95_64_t aeskey95_64;   /**< AES encryption key */
    core_ble_aeskey127_96_t aeskey127_96; /**< AES encryption key */
    core_ble_aesptr_t aesptr;             /**< Pointer to the block to encrypt/decrypt */
    uint32_t unk22;
    uint32_t unk23;
    core_ble_rftestcntl_t rftestcntl; /**< RF testing register */
    uint32_t unk24;
    uint32_t unk25;
    uint32_t unk26;
    core_ble_timgencntl_t timgencntl; /**< Timing generator register */
    uint32_t unk27;
    uint32_t unk28;
    uint32_t unk29;
    core_ble_coexifcntl0_t coexifcntl0; /**< Coexistence configuration register */
    uint32_t unk30;
    uint32_t unk31;
    uint32_t unk32;
    uint32_t unk33;
    uint32_t unk34;
    uint32_t unk35;
    uint32_t unk36;
    core_ble_ralptr_t ralptr;     /**< Resolve address list pointer */
    core_ble_ralnbdev_t ralnbdev; /**< Resolve address list number of devices */
    core_ble_ral_local_rnd_t ral_local_rnd;
    core_ble_ral_peer_rnd_t ral_peer_rnd;
    uint32_t unk37;
    uint32_t unk38;
    uint32_t unk39;
    uint32_t unk40;
    uint32_t unk41;
    uint32_t unk42;
    uint32_t unk43;
    uint32_t unk44;
    uint32_t unk45;
    uint32_t unk46;
    uint32_t unk47;
    uint32_t unk48;
    uint32_t unk49;
    uint32_t unk50;
    uint32_t unk51;
    uint32_t unk52;
    uint32_t unk53;
    uint32_t unk54;
    uint32_t unk55;
    uint32_t unk56;
    uint32_t unk57;
    uint32_t unk58;
    uint32_t unk59;
    uint32_t unk60;
    core_ble_bleprioscharb_t bleprioscharb;
} core_ble_t;

static_assert(offsetof(core_ble_t, rwblecntl) == 0x0000);
static_assert(offsetof(core_ble_t, version) == 0x0004);
static_assert(offsetof(core_ble_t, rwbleconf) == 0x0008);
static_assert(offsetof(core_ble_t, intcntl) == 0x000c);
static_assert(offsetof(core_ble_t, intstat) == 0x0010);
static_assert(offsetof(core_ble_t, intrawstat) == 0x0014);
static_assert(offsetof(core_ble_t, intack) == 0x0018);
static_assert(offsetof(core_ble_t, basetimecnt) == 0x001c);
static_assert(offsetof(core_ble_t, finetimecnt) == 0x0020);
static_assert(offsetof(core_ble_t, bdaddrl) == 0x0024);
static_assert(offsetof(core_ble_t, bdaddru) == 0x0028);
static_assert(offsetof(core_ble_t, et_currentrxdescptr) == 0x002c);
static_assert(offsetof(core_ble_t, diagcntl) == 0x0050);
static_assert(offsetof(core_ble_t, diagstat) == 0x0054);
static_assert(offsetof(core_ble_t, debugaddmax) == 0x0058);
static_assert(offsetof(core_ble_t, debugaddmin) == 0x005c);
static_assert(offsetof(core_ble_t, errortypestat) == 0x0060);
static_assert(offsetof(core_ble_t, swprofiling) == 0x0064);
static_assert(offsetof(core_ble_t, radiocntl0) == 0x0070);
static_assert(offsetof(core_ble_t, radiocntl1) == 0x0074);
static_assert(offsetof(core_ble_t, radiopwrupdn0) == 0x0080);
static_assert(offsetof(core_ble_t, advchmap) == 0x0090);
static_assert(offsetof(core_ble_t, advtim) == 0x00a0);
static_assert(offsetof(core_ble_t, actscanstat) == 0x00a4);
static_assert(offsetof(core_ble_t, wlpubaddptr) == 0x00b0);
static_assert(offsetof(core_ble_t, wlprivaddptr) == 0x00b4);
static_assert(offsetof(core_ble_t, wlnbdev) == 0x00b8);
static_assert(offsetof(core_ble_t, aescntl) == 0x00c0);
static_assert(offsetof(core_ble_t, aeskey31_0) == 0x00c4);
static_assert(offsetof(core_ble_t, aeskey63_32) == 0x00c8);
static_assert(offsetof(core_ble_t, aeskey95_64) == 0x00cc);
static_assert(offsetof(core_ble_t, aeskey127_96) == 0x00d0);
static_assert(offsetof(core_ble_t, aesptr) == 0x00d4);
static_assert(offsetof(core_ble_t, rftestcntl) == 0x00e0);
static_assert(offsetof(core_ble_t, timgencntl) == 0x00f0);
static_assert(offsetof(core_ble_t, coexifcntl0) == 0x0100);
static_assert(offsetof(core_ble_t, ralptr) == 0x0120);
static_assert(offsetof(core_ble_t, ralnbdev) == 0x0124);
static_assert(offsetof(core_ble_t, ral_local_rnd) == 0x0128);
static_assert(offsetof(core_ble_t, ral_peer_rnd) == 0x012c);
static_assert(offsetof(core_ble_t, bleprioscharb) == 0x0190);

#endif /* HAL_CORE_BLE_H */
