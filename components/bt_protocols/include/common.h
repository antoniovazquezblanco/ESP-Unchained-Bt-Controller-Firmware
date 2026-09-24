/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Common Bluetooth types shared across protocol layers (HCI, LMP, LL).
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/**
 * A Bluetooth device address is six bytes on every protocol layer.
 */
#define BD_ADDR_LEN 6

/**
 * A Bluetooth device address, little endian as it travels on air and HCI. A lone
 * uint8_t array cannot be padded, so this needs no packing, but it is copied over
 * controller memory and onto the wire by size: hold that to the spec.
 */
typedef struct
{
    uint8_t addr[BD_ADDR_LEN];
} bd_addr_t;
_Static_assert(sizeof(bd_addr_t) == BD_ADDR_LEN, "bd_addr_t must be exactly BD_ADDR sized");

#endif /* COMMON_H */
