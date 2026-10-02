/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Internal API for the EM.
 */
#ifndef ROM_EM_H
#define ROM_EM_H

#include <stddef.h>
#include <assert.h>

#include "rom/co.h"

struct em_buf_node
{
    struct co_list_hdr hdr; /**< List header */
    uint16_t idx;           /**< Index of the buffer */
    uint16_t buf_ptr;       /**< EM buffer pointer */
};

struct em_desc_node
{
    struct co_list_hdr hdr; /**< List header */
    uint16_t idx;           /**< Index of the buffer */
    uint16_t buffer_idx;    /**< EM buffer index */
    uint16_t buffer_ptr;    /**< EM offset of the payload */
    uint8_t llid;           /**< Logical Link Identifier */
    uint8_t length;         /**< Data length */
};

struct em_buf_env_tag
{
    struct co_list tx_desc_free;           /**< List of free TX descriptors */
    struct co_list tx_buff_free;           /**< List of free TX buffers */
    struct em_desc_node tx_desc_node[113]; /**< Array of TX descriptors (SW tag) */
    struct em_buf_node tx_buff_node[10];   /**< Array of TX buffer (SW tag) */
    struct em_buf_tx_desc *tx_desc;        /**< Pointer to TX descriptors */
    uint8_t rx_current;                    /**< Index of the current RX buffer */
};

static_assert(offsetof(struct em_buf_env_tag, rx_current) == 0x5c8,
              "rx_current must sit at em_buf_env + 0x5c8");

extern struct em_buf_env_tag em_buf_env;

#endif /* ROM_EM_H */
