/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 controller dispatch tables (the r_ip_funcs analog of the classic).
 *
 * Like the classic ESP32, the C3 reaches the rest of the controller through
 * writable function-pointer tables in BTDM RAM, exported as absolute symbols by
 * esp32c3.rom.ld: r_ip_funcs_p (HCI/link glue) and r_modules_funcs_p (kernel).
 * Overwriting a slot hooks that call for every caller -- the same technique the
 * esp32_bt_unchained layer uses. Only the slots the unchained layer needs are
 * named; the rest is reserved. Offsets verified against ip_funcs.o /
 * modules_funcs.o relocations and the rev3 ROM disassembly.
 */

#ifndef ESP32C3_IP_FUNCS_H
#define ESP32C3_IP_FUNCS_H

#include <stddef.h>
#include <stdint.h>

/* ke_msg id the controller uses for an HCI Command Complete event. Allocate with
 * ke_msg_alloc(id, 0, opcode, param_len), fill the return parameters (status
 * first), then hand the buffer to hci_send_2_host. Seen in every CC emitter
 * (hci_rd_bd_addr_cmd_handler, the vendor handlers). */
#define HCI_CC_EVT_KE_ID 0x1101

/* r_ip_funcs slot signatures (only the ones the unchained layer touches). */
typedef void (*r_hci_cmd_received_fn_t)(uint16_t opcode, uint16_t param_len, uint8_t *param_buf);
typedef void *(*r_hci_look_for_cmd_desc_fn_t)(uint16_t opcode);
typedef void *(*r_hci_look_for_evt_desc_fn_t)(uint8_t evt_code);
typedef void (*r_hci_send_2_host_fn_t)(void *evt_msg);

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
} r_ip_funcs_t;

_Static_assert(offsetof(r_ip_funcs_t, hci_cmd_received) == 0x2c, "hci_cmd_received @ +0x2c");
_Static_assert(offsetof(r_ip_funcs_t, hci_send_2_host) == 0x8c, "hci_send_2_host @ +0x8c");
_Static_assert(offsetof(r_ip_funcs_t, hci_look_for_cmd_desc) == 0x90, "hci_look_for_cmd_desc @ +0x90");
_Static_assert(offsetof(r_ip_funcs_t, hci_look_for_evt_desc) == 0x98, "hci_look_for_evt_desc @ +0x98");

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
 * The link-manager environment. Only the public identity address is modelled:
 * Read_BD_ADDR copies p_llm_env->bd_addr, and the advertising/scan paths read it
 * when they build PDUs, so writing it re-brands the controller's public address.
 */
typedef struct llm_env
{
    uint8_t _reserved_00[0x0c];
    uint8_t bd_addr[6]; /* +0x0c public BD_ADDR */
} llm_env_t;

_Static_assert(offsetof(llm_env_t, bd_addr) == 0x0c, "public bd_addr @ +0x0c");

/* The live link-manager environment (absolute symbol p_llm_env @ 0x3fcdff98). */
extern llm_env_t *p_llm_env;

/*
 * The HCI transport environment. The controller tracks the host command credit
 * (num_hci_command_packets) in nb_h2c_cmd_pkts: r_hci_cmd_received consumes one
 * per command received, hci_tx_start returns one per Command Complete and
 * asserts (hci_tl.c:1008) if the credit climbs past 5. A hook that answers a
 * command itself bypasses r_hci_cmd_received, so it must consume the credit by
 * hand or the returned-but-never-consumed credit trips that assert.
 */
typedef struct hci_tl_env
{
    uint8_t _reserved_00[0x15];
    int8_t nb_h2c_cmd_pkts; /* +0x15 host-to-controller command credit */
} hci_tl_env_t;

_Static_assert(offsetof(hci_tl_env_t, nb_h2c_cmd_pkts) == 0x15, "command credit @ +0x15");

/* The live HCI transport environment (absolute symbol hci_tl_env @ 0x3fcdfe00). */
extern hci_tl_env_t hci_tl_env;

#endif /* ESP32C3_IP_FUNCS_H */
