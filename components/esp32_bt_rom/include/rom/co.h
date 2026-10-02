/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Common element definitions.
 */
#ifndef ROM_CO_H
#define ROM_CO_H

#include <stdint.h>

// List element header
struct co_list_hdr
{
    struct co_list_hdr *next;
};

/** Structure of a list */
struct co_list
{
    struct co_list_hdr *first; /**< Pointer to first list element */
    struct co_list_hdr *last;  /**< Pointer to last list element */
    uint32_t cnt;              /**< Number of elements in the list */
    uint32_t maxcnt;           /**< Maximum number of elements in the list */
    uint32_t mincnt;           /**< Minimum number of element in the list */
};

#endif /* ROM_CO_H */
