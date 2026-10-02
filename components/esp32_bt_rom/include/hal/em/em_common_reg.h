/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Exchange memory common register definitions.
 */
#ifndef HAL_EM_COMMON_REG_H
#define HAL_EM_COMMON_REG_H

#include <stdint.h>
#include <assert.h>

/**
 * EXTAB0 MODE
 *  0x0: No mode selected, nothing to be performed
 *  0x1: BR/EDR Mode
 *  0x2: BLE Mode
 *  0x3-0xF: Reserved for future use           -
 */
typedef enum __attribute__((packed))
{
    EM_ET_MODE_NONE = 0x00,
    EM_ET_MODE_BREDR = 0x01,
    EM_ET_MODE_BLE = 0x02,
} em_common_extab0_mode_t;
static_assert(sizeof(em_common_extab0_mode_t) == 1);

/**
 * EXTAB0 STATUS
 *  00: Control Structure Pointer is ready for processing
 *  01: Control Structure Pointer is currently under process
 *  10: Reserved
 *  11: Reserved
 */
typedef enum __attribute__((packed))
{
    EM_ET_STATUS_READY = 0x00,
    EM_ET_STATUS_UNDER_PROCESS = 0x01,
} em_common_extab0_status_t;
static_assert(sizeof(em_common_extab0_status_t) == 1);

/**
 * EXTAB0 register definition.
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  00:03                 MODE   0
 *  04:05               STATUS   0
 *  06:15                    -
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        em_common_extab0_mode_t mode : 4;
        em_common_extab0_status_t status : 2;
        unsigned short : 10;
    } fields;
} em_common_extab0_t;
static_assert(sizeof(em_common_extab0_t) == 2);

/**
 * EXTAB1 register definition.
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  14:00                CSPTR   0
 *     15           EXCPTRNRDY   0
 */
typedef union
{
    uint16_t raw;
    struct __attribute__((packed))
    {
        unsigned short csptr : 15;
        unsigned short excptrnrdy : 1;
    } fields;
} em_common_extab1_t;
static_assert(sizeof(em_common_extab1_t) == 2);

#endif /* HAL_EM_COMMON_REG_H */
