/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Base pointers for the BLE core registers and exchange memory, and a single
 * translation unit that pulls in every model header so they are compile-checked.
 */
#include "hal/hal.h"

#include "rom/co.h"
#include "rom/ke.h"
#include "rom/ea.h"
#include "rom/hli.h"
#include "rom/em.h"
#include "rom/lld.h"
#include "rom/lld_evt.h"

#define BLE_CORE_BASE_ADDR 0x3ff71200
#define BLE_EM_BASE_ADDR 0x3ffb0000

volatile core_ble_t *ble = (volatile core_ble_t *)(BLE_CORE_BASE_ADDR);
volatile em_t *em = (volatile em_t *)(BLE_EM_BASE_ADDR);
