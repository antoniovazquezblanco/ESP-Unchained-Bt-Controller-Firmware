/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Base pointers for the BLE core registers and exchange memory.
 */
#ifndef HAL_HAL_H
#define HAL_HAL_H

#include "hal/core/core_ble.h"
#include "hal/em/em.h"

extern volatile core_ble_t *ble;
extern volatile em_t *em;

#endif /* HAL_HAL_H */
