/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy Control Structures (CS) definitions.
 */
#ifndef HAL_EM_BLE_CS_H
#define HAL_EM_BLE_CS_H

#include "hal/em/em_ble_cs_reg.h"
#include "hal/types.h"

#include <stdint.h>
#include <assert.h>

/***
 * Control structure element.
 * The exchange memory is composed of a number of this elements.
 * Each one has a number of registers associated.
 */
typedef struct
{
    em_ble_cs_cntl_t cntl;
    em_ble_cs_fcntoffset_t fcntoffset;
    em_ble_cs_link_t link;
    em_ble_cs_syncwl_t syncwl;
    em_ble_cs_syncwh_t syncwh;
    em_ble_cs_crcinit0_t crcinit0;
    em_ble_cs_crcinit1_t crcinit1;
    em_ble_cs_filtpol_ralcntl_t filtpol_ralcntl;
    em_ble_cs_hopcntl_t hopcntl;
    em_ble_cs_txrxcntl_t txrxcntl;
    em_ble_cs_rxwincntl_t rxwincntl;
    em_ble_cs_txdescptr_t txdescptr;
    em_ble_cs_winoffset_t winoffset;
    em_ble_cs_maxevtime_t maxevtime;
    em_ble_cs_chmap0_t chmap0;
    em_ble_cs_chmap1_t chmap1;
    em_ble_cs_chmap2_t chmap2;
    em_ble_cs_rxmaxbuf_t rxmaxbuf;
    em_ble_cs_rxmaxtime_t rxmaxtime;
    union
    {
        em_ble_cs_peer_ralptr_t peer_ralptr;
        bdaddr_t adv_bd_addr;
    };
    em_ble_cs_adv_bd_addr_type_t adv_bd_addr_type;
    uint16_t unk0;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    iv_t ivm;
    iv_t ivs;
    em_ble_cs_txccmpktcnt0_t txccmpktcnt0;
    em_ble_cs_txccmpktcnt1_t txccmpktcnt1;
    em_ble_cs_txccmpktcnt2_t txccmpktcnt2;
    em_ble_cs_rxccmpktcnt0_t rxccmpktcnt0;
    em_ble_cs_rxccmpktcnt1_t rxccmpktcnt1;
    em_ble_cs_rxccmpktcnt2_t rxccmpktcnt2;
    uint16_t unk4;
    uint16_t unk5;
    em_ble_cs_btcntsync0_t btcntsync0;
    em_ble_cs_btcntsync1_t btcntsync1;
    em_ble_cs_fcntsync_t fcntsync;
    em_ble_cs_txrxdesccnt_t txrxdesccnt;
    em_ble_cs_isotxrxcntl_t isotxrxcntl;
    em_ble_cs_thrcntl_ratecntl_t thrcntl_ratecntl;
} em_ble_cs_elt_t;

// Size extracted from decompiled binaries
static_assert(sizeof(em_ble_cs_elt_t) == 0x5a);

/***
 * BLE control structures memory region definition.
 */
typedef struct
{
    em_ble_cs_elt_t elt[11];
} em_ble_cs_t;

#endif /* HAL_EM_BLE_CS_H */
