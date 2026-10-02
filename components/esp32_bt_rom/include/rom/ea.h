/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Event Arbiter related definitions.
 */
#ifndef ROM_EA_H
#define ROM_EA_H

#include "rom/co.h"

#include <stdint.h>
#include <stdbool.h>

/** Event Arbiter Element */
struct ea_elt_tag
{
    struct co_list_hdr hdr;            /**< List header to link events */
    struct ea_elt_tag *linked_element; /**< Pointer to the next element linked to the current event */
    uint32_t unk1;
    uint32_t unk2;
    uint16_t unk3;
    uint16_t unk4;
    uint16_t unk5;
    uint8_t unk6;
    uint8_t unk7;
    uint8_t unk8;
    uint8_t unk9;
    void (*ea_cb_start)(struct ea_elt_tag *);  /** Start callback */
    void (*ea_cb_stop)(struct ea_elt_tag *);   /** Stop callback */
    void (*ea_cb_cancel)(struct ea_elt_tag *); /** Cancel callback */
    void *env;                                 /** BT/BLE environment variable */
};

/** Interval Element */
struct ea_interval_tag
{
    struct co_list_hdr hdr; /**< List header for chaining in the interval list */
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t conhdl_used; /**< Connection handle used */
    uint16_t unk4;
    bool unk5;
    uint16_t unk6;
};

#endif /* ROM_EA_H */
