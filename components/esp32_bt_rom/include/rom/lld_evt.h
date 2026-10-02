/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Link Layer Driver definitions for Event scheduling.
 */
#ifndef ROM_LLD_EVT_H
#define ROM_LLD_EVT_H

#include "rom/co.h"

#include <stdbool.h>

/** Structure with information about the latest synchronization */
struct lld_evt_anchor
{
    uint32_t basetime_cnt;
    uint16_t finetime_cnt;
    uint16_t evt_cnt;
};

/** Non connected event information */
struct lld_non_conn
{
    uint32_t window;
    uint32_t anchor;
    uint32_t end_ts;
    bool initiate;
    bool connect_req_sent;
};

/** Connected event information */
struct lld_conn
{
    uint32_t sync_win_size;
    uint32_t sca_drift;
    uint16_t instant;
    uint16_t latency;
    uint16_t counter;
    uint16_t missed_cnt;
    uint16_t duration_dft;
    uint16_t update_offset;
    uint16_t eff_max_tx_time;
    uint16_t eff_max_tx_size;
    uint8_t update_size;
    uint8_t instant_action;
    uint8_t mst_sca;
    uint8_t last_md_rx;
    uint8_t tx_prog_pkt_cnt;
    uint8_t rx_win_off_dft;
    uint8_t rx_win_pathdly_comp;
    bool wait_con_up_sync;
    uint32_t win_size_backup;
};

/** Structure describing an event */
struct lld_evt_tag
{
    struct lld_evt_anchor anchor_point;   /**< Information about synchronization */
    struct co_list tx_acl_rdy;            /**< List of TX data descriptors ready for transmission */
    struct co_list tx_acl_tofree;         /**< List of TX data descriptors ready to be freed */
    struct co_list tx_prog;               /**< List of TX LLCP descriptors programmed for transmission */
    struct ea_interval_tag *interval_elt; /**< Interval element linked to this event */
    union lld_evt_info
    {
        struct lld_non_conn non_conn; /**< Non connected event information */
        struct lld_conn conn;         /**< Connected event information */
    } evt;                            /**< Event information for connected and non-connected activity */
    uint16_t conhdl;                  /**< Connection handle */
    uint16_t cs_ptr;                  /**< Control Structure address */
    uint16_t interval;                /**< Connection interval */
    uint8_t rx_cnt;                   /**< Number of RX descriptors already handled in the event */
    uint8_t mode;                     /**< Mode of the link */
    uint8_t tx_pwr;                   /**< TX power */
    uint8_t default_prio;             /**< Default priority */
    uint8_t evt_flag;                 /**< Internal status */
    bool delete_ongoing;
};

#endif /* ROM_LLD_EVT_H */
