/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Low Energy Resolve Address List (RAL) register definitions.
 */
#ifndef HAL_EM_BLE_RAL_REG_H
#define HAL_EM_BLE_RAL_REG_H

#include <stdint.h>

/**
 * RAL_INFO register definition.
 *
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00         PEER_ID_TYPE   0
 *     01       PEER_IRK_VALID   0
 *     02       PEER_RPA_RENEW   0
 *     03       PEER_RPA_VALID   0
 *     04                    -
 *     05      LOCAL_IRK_VALID   0
 *     06      LOCAL_RPA_RENEW   0
 *     07      LOCAL_RPA_VALID   0
 *  08:12                    -
 *     13            IN_WHLIST   0
 *     14            CONNECTED   0
 *     15          ENTRY_VALID   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short peer_id_type : 1;
        unsigned short peer_irk_valid : 1;
        unsigned short peer_rpa_renew : 1;
        unsigned short peer_rpa_valid : 1;
        unsigned short : 1;
        unsigned short local_irk_valid : 1;
        unsigned short local_rpa_renew : 1;
        unsigned short local_rpa_valid : 1;
        unsigned short : 5;
        unsigned short in_whlist : 1;
        unsigned short connected : 1;
        unsigned short entry_valid : 1;
    } fields;
} em_ble_ral_info_t;
static_assert(sizeof(em_ble_ral_info_t) == 2);

#endif /* HAL_EM_BLE_RAL_REG_H */
