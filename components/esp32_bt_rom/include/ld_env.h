/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 classic (BR/EDR) link driver state.
 */

#ifndef LD_ENV_H
#define LD_ENV_H

#include "hci.h"

/**
 * BR/EDR link driver environment.
 *
 * Only the leading BD_ADDR is mapped: everything after it is link driver state
 * we have not typed, so this struct describes a prefix of the real thing and
 * sizeof() it means nothing.
 */
typedef struct
{
    bd_addr_t bd_addr;
} ld_env_t;

/**
 * The live link driver environment, an absolute ROM-data symbol (esp32.rom.ld).
 *
 * Its BD_ADDR is the controller's BR/EDR public address: r_ld_bd_addr_get()
 * copies it out, which is what HCI Read BD_ADDR returns, and the page/inquiry
 * scan setup reads it when arming a scan. The LE side keeps a separate copy,
 * reached through llm_util_set_public_addr(), so setting the address for both
 * transports means writing here as well.
 */
extern ld_env_t ld_env;

#endif /* LD_ENV_H */
