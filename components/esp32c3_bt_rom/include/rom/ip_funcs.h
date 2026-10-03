/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 controller dispatch tables (the r_ip_funcs analog of the classic).
 *
 * Like the classic ESP32, the C3 reaches the rest of the controller through
 * writable function-pointer tables in BTDM RAM, exported as absolute symbols by
 * esp32c3.rom.ld: r_ip_funcs_p (HCI/link glue), r_modules_funcs_p (kernel) and
 * r_plf_funcs_p (platform/exchange-memory). Overwriting a slot hooks that call
 * for every caller -- the same technique the esp32_bt_unchained layer uses. Only
 * the slots the unchained layer needs are named; the rest is reserved. Offsets
 * verified against ip_funcs.o / modules_funcs.o relocations and the rev3 ROM.
 */

#ifndef ESP32C3_ROM_IP_FUNCS_H
#define ESP32C3_ROM_IP_FUNCS_H

#include <stddef.h>
#include <stdint.h>

/* r_ip_funcs slot signatures (only the ones the unchained layer touches). */
typedef void (*r_hci_cmd_received_fn_t)(uint16_t opcode, uint16_t param_len, uint8_t *param_buf);
typedef void *(*r_hci_look_for_cmd_desc_fn_t)(uint16_t opcode);
typedef void *(*r_hci_look_for_evt_desc_fn_t)(uint8_t evt_code);
typedef void (*r_hci_send_2_host_fn_t)(void *evt_msg);
typedef uint32_t (*r_ld_read_clock_fn_t)(void);
typedef uint32_t (*r_lld_con_rx_llcp_check_fn_t)(uint32_t link_id, void *con_env, uint32_t llid, uint16_t length);

/*
 * The r_ip_funcs dispatch table (partial model). The true table is much larger;
 * only the slots we hook or call are named, the gaps are reserved.
 */
typedef struct r_ip_funcs
{
    uint8_t _reserved_00[0x2c];
    r_hci_cmd_received_fn_t hci_cmd_received;           /* +0x2c HCI command ingress */
    uint8_t _reserved_30[0x5c];                         /* 0x30..0x8b */
    r_hci_send_2_host_fn_t hci_send_2_host;             /* +0x8c send an event/CC to the host */
    r_hci_look_for_cmd_desc_fn_t hci_look_for_cmd_desc; /* +0x90 command-descriptor lookup */
    uint8_t _reserved_94[0x04];                         /* 0x94..0x97 */
    r_hci_look_for_evt_desc_fn_t hci_look_for_evt_desc; /* +0x98 event-descriptor lookup */
    uint8_t _reserved_9c[0x264 - 0x9c];
    r_ld_read_clock_fn_t ld_read_clock; /* +0x264 current BT clock (312.5us ticks) */
    uint8_t _reserved_268[0x394 - 0x268];
    r_lld_con_rx_llcp_check_fn_t lld_con_rx_llcp_check; /* +0x394 handle one received connection PDU */
} r_ip_funcs_t;

_Static_assert(offsetof(r_ip_funcs_t, hci_cmd_received) == 0x2c, "hci_cmd_received @ +0x2c");
_Static_assert(offsetof(r_ip_funcs_t, hci_send_2_host) == 0x8c, "hci_send_2_host @ +0x8c");
_Static_assert(offsetof(r_ip_funcs_t, hci_look_for_cmd_desc) == 0x90, "hci_look_for_cmd_desc @ +0x90");
_Static_assert(offsetof(r_ip_funcs_t, hci_look_for_evt_desc) == 0x98, "hci_look_for_evt_desc @ +0x98");
_Static_assert(offsetof(r_ip_funcs_t, ld_read_clock) == 0x264, "ld_read_clock @ +0x264");
_Static_assert(offsetof(r_ip_funcs_t, lld_con_rx_llcp_check) == 0x394, "lld_con_rx_llcp_check @ +0x394");

/* The live table (absolute symbol r_ip_funcs_p @ 0x3fcdff8c). */
extern r_ip_funcs_t *r_ip_funcs_p;

/* r_modules_funcs slot signatures. ke_msg_alloc returns a param_len-byte buffer
 * for the message parameters; ke_msg_send queues a filled message. */
typedef uint8_t *(*r_ke_msg_alloc_fn_t)(uint16_t id, uint16_t dest_id, uint16_t src_id, uint16_t param_len);
typedef void (*r_ke_msg_send_fn_t)(void *param_ptr);

/*
 * The r_modules_funcs dispatch table (partial model), the kernel half.
 */
typedef struct r_modules_funcs
{
    uint8_t _reserved_00[0xc8];
    r_ke_msg_alloc_fn_t ke_msg_alloc; /* +0xc8 allocate a kernel message */
    uint8_t _reserved_cc[0x14];       /* 0xcc..0xdf */
    r_ke_msg_send_fn_t ke_msg_send;   /* +0xe0 queue a kernel message */
} r_modules_funcs_t;

_Static_assert(offsetof(r_modules_funcs_t, ke_msg_alloc) == 0xc8, "ke_msg_alloc @ +0xc8");
_Static_assert(offsetof(r_modules_funcs_t, ke_msg_send) == 0xe0, "ke_msg_send @ +0xe0");

/* The live table (absolute symbol r_modules_funcs_p @ 0x3fcdff88). */
extern r_modules_funcs_t *r_modules_funcs_p;

/*
 * The platform/exchange-memory functions table (r_plf_funcs_p @ 0x3fcdff80).
 * em_buf_get maps an exchange-memory region id OR a buffer handle to its CPU
 * address: region ids (hal/em.h EM_REGION_*) give the region base, and a buffer
 * handle (e.g. a received PDU's, from the RX descriptor) gives that buffer's
 * address.
 */
typedef void *(*r_em_buf_get_fn_t)(uint32_t region_or_handle);

typedef struct r_plf_funcs
{
    uint8_t _reserved_00[0xbc];
    r_em_buf_get_fn_t em_buf_get; /* +0xbc exchange-memory address for a region/handle */
} r_plf_funcs_t;

_Static_assert(offsetof(r_plf_funcs_t, em_buf_get) == 0xbc, "em_buf_get @ +0xbc");

extern r_plf_funcs_t *r_plf_funcs_p;

#endif /* ESP32C3_ROM_IP_FUNCS_H */
