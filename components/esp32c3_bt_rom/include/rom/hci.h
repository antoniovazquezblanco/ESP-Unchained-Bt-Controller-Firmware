/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 controller HCI layer: the command/event descriptor formats the ROM
 * resolves, the transport environment, the kernel-message ids for events, and the
 * ROM HCI entry points. The dispatch slots these flow through live in
 * rom/ip_funcs.h (hci_cmd_received, look_for_cmd_desc, hci_send_2_host, ...).
 */

#ifndef ESP32C3_ROM_HCI_H
#define ESP32C3_ROM_HCI_H

#include <stddef.h>
#include <stdint.h>

/* ke_msg id the controller uses for an HCI Command Complete event. Allocate with
 * ke_msg_alloc(id, 0, opcode, param_len), fill the return parameters (status
 * first), then hand the buffer to hci_send_2_host. Seen in every CC emitter
 * (hci_rd_bd_addr_cmd_handler, the vendor handlers). */
#define HCI_CC_EVT_KE_ID 0x1101

/* ke_msg id for an unsolicited HCI event (not a Command Complete). Allocate with
 * ke_msg_alloc(id, 0, event_code, param_len), fill the event parameters, then
 * hand the buffer to hci_send_2_host; hci_tx_start packs it with r_hci_build_evt,
 * which looks the event code up with look_for_evt_desc. */
#define HCI_EVT_KE_ID 0x1103

/*
 * HCI command descriptor entry (12 bytes). r_hci_look_for_cmd_desc() returns one
 * of these; the lookup matches on the OCF (opcode & 0x3ff).
 */
typedef struct
{
    uint16_t opcode;      /* +0 OGF<<10 | OCF */
    uint8_t flags;        /* +2 low nibble CC/CS dest; 0x40 handler self-unpacks; 0x80 ret_fmt is a pack fn */
    uint8_t par_size_max; /* +3 max accepted parameter length (r_hci_cmd_get_max_param_size) */
    void *fn;             /* +4 param-unpack format (std cmds) or inline handler (vendor cmds) */
    void *ret_fmt;        /* +8 return-parameter pack format (0x80) or format table */
} esp32c3_hci_cmd_desc_t;

/*
 * HCI event descriptor entry. r_hci_build_evt() resolves one via look_for_evt_desc
 * to pack an unsolicited event; without it the packer drops the event. For a
 * pre-laid-out event set flags non-zero (ret_fmt is a self-pack function) and
 * point ret_fmt at a no-op packer returning 0.
 */
typedef struct
{
    uint16_t code;     /* +0 event code */
    uint8_t flags;     /* +2 non-zero: ret_fmt is a self-pack function */
    uint8_t _reserved; /* +3 */
    void *ret_fmt;     /* +4 return-parameter pack format / function */
} esp32c3_hci_evt_desc_t;

/*
 * ESP vendor sub-dispatch entry (8 bytes), walked by
 * r_esp_vendor_hci_command_handler(). Matched on the full opcode; the handler
 * receives the opcode in a3.
 */
typedef struct
{
    uint16_t opcode;
    uint16_t _reserved;
    void *handler; /* void handler(uint16_t opcode) */
} esp32c3_esp_vendor_cmd_t;

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

/* ---- ROM HCI entry points (absolute symbols from esp32c3.rom.bt_funcs.ld) ---- */

/* HCI command entry: resolves the descriptor, unpacks params, routes the cmd. */
void r_hci_cmd_received(uint16_t opcode, uint16_t param_len, const void *params);

/*
 * Descriptor lookup: OGF -> per-group table, match on OCF. Returns an
 * esp32c3_hci_cmd_desc_t* or NULL. For opcodes > 0xFC80 it also scans the
 * RAM-registered vendor table (the custom-VSC hook, see the reversing doc).
 */
void *r_hci_look_for_cmd_desc(uint16_t opcode);

/* Install the ROM-default vendor command tables. */
void r_hci_register_vendor_desc_tab(void);
void r_register_esp_vendor_cmd_handler(void);

/* Message/command handler-table getters (return the dispatch table base). */
void *r_llc_hci_cmd_handler_tab_p_get(void);
void *r_llm_msg_handler_tab_p_get(void);
void *r_misc_msg_handler_tab_p_get(void);

#endif /* ESP32C3_ROM_HCI_H */
