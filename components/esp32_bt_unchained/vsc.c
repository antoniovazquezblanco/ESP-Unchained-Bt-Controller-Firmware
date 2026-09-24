/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set.
 */
#include "vsc.h"

#include <stddef.h>
#include <string.h>

#include "esp_app_desc.h"

#include "esp32_bt_rom.h"
#include "hci.h"

#ifndef ESP32_BT_UNCHAINED_BOARD
#define ESP32_BT_UNCHAINED_BOARD "unknown"
#endif

/* The common case: a Command Complete that returns nothing but status. */
static void vs_cmd_complete_status(uint16_t opcode, uint8_t status)
{
    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_CC_EVT_KE_ID, 0, opcode, 1);
    if (p == NULL)
        return;
    p[0] = status;
    r_ip_funcs_p->hci_send_2_host_hack(p);
}

/**
 * Command handler function type definition.
 */
typedef void (*vs_cmd_handler_fn_t)(uint16_t opcode, uint8_t length, const uint8_t *payload);

/**
 * Handler table entry for a single vendor-specific command.
 */
typedef struct
{
    uint16_t opcode;
    vs_cmd_handler_fn_t handler;
} vs_cmd_t;

/*
 * Prototypes for the vendor-specific command handlers.
 */
static void vs_info(uint16_t opcode, uint8_t length, const uint8_t *payload);
static void vs_supported_cmds(uint16_t opcode, uint8_t length, const uint8_t *payload);
static void vs_set_bdaddr(uint16_t opcode, uint8_t length, const uint8_t *payload);

/**
 * The table of vendor-specific commands, mapping opcodes to their handlers.
 */
static const vs_cmd_t s_vs_cmds[] = {
    {UNCHAINED_VS_INFO_OPCODE, vs_info},
    {UNCHAINED_VS_SUPPORTED_CMDS_OPCODE, vs_supported_cmds},
    {UNCHAINED_VS_SET_BDADDR_OPCODE, vs_set_bdaddr},
};

/*
 * Reply packing. Building a Command Complete, the ROM (hci_tx_start, ke_msg
 * 0x801) looks the opcode up with hci_look_for_cmd_desc_hack and, when that
 * returns NULL, overwrites the first return byte -- our status -- with 0x01
 * "Unknown HCI Command". Espressif's lookup rejects every vendor opcode <=
 * 0xFC80 before it even searches, which is all of ours, so we hand it a
 * descriptor of our own. It only reads the flags and the pack function off the
 * descriptor on that path, so one shared entry serves every command in
 * s_vs_cmds.
 */
/* The return parameters are already laid out by the handler, so packing is a
 * no-op: the ROM passes one buffer as both out and in and keeps the length we
 * allocated. Zero tells it the pack succeeded. */
static uint16_t vs_pack_in_place(uint8_t *out, uint8_t *in, uint16_t *out_len, uint16_t in_len)
{
    return 0;
}

static const hci_cmd_desc_t s_vs_cmd_desc = {
    .opcode = 0, /* never read back: the ROM already knows the opcode */
    /* No routing bits: the command never reaches the ROM's dispatch, we answer
     * it first. Only the flag saying ret_par_fmt is a function matters. */
    .dest_field = HCI_CMD_DEST_SPEC_RET_PK_MSK,
    /* hci_cmd_get_max_param_size() reads this to bound an incoming command and
     * answers 0xFF when there is no descriptor, so anything smaller would make
     * the hook itself start dropping commands that carry parameters. */
    .par_size_max = 0xFF,
    .par_fmt = NULL,
    .ret_par_fmt = vs_pack_in_place,
};

hci_cmd_desc_t *vsc_cmd_desc(uint16_t opcode)
{
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
        if (s_vs_cmds[i].opcode == opcode)
            return (hci_cmd_desc_t *)&s_vs_cmd_desc;

    return NULL;
}

/* 0xFC00 INFO: firmware name, firmware version and board name, each prefixed by
 * a one-byte length. */
static void vs_info(uint16_t opcode, uint8_t length, const uint8_t *payload)
{
    // Get the needed variables...
    const esp_app_desc_t *desc = esp_app_get_description();

    // Calculate field lengths for each component. The desc fields are fixed size
    // arrays that need not be terminated, the board name is a literal.
    size_t project_name_len = strnlen(desc->project_name, sizeof(desc->project_name));
    size_t version_len = strnlen(desc->version, sizeof(desc->version));
    size_t board_name_len = sizeof(ESP32_BT_UNCHAINED_BOARD) - 1;

    // Build the response...
    uint16_t len = 1 + 1 + project_name_len + 1 + version_len + 1 + board_name_len;
    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_CC_EVT_KE_ID, 0, opcode, len);
    if (p == NULL)
        return;
    size_t n = 0;
    p[n++] = HCI_SUCCESS;
    p[n++] = (uint8_t)project_name_len;
    memcpy(&p[n], desc->project_name, project_name_len);
    n += project_name_len;
    p[n++] = (uint8_t)version_len;
    memcpy(&p[n], desc->version, version_len);
    n += version_len;
    p[n++] = (uint8_t)board_name_len;
    memcpy(&p[n], ESP32_BT_UNCHAINED_BOARD, board_name_len);
    r_ip_funcs_p->hci_send_2_host_hack(p);
}

/* 0xFC01 SUPPORTED_CMDS: a 64-bit bitfield, bit N set when the command with OCF
 * N is implemented. Our OCFs start at 0 and grow, so this covers the first 64. */
static void vs_supported_cmds(uint16_t opcode, uint8_t length, const uint8_t *payload)
{
    uint64_t supported = 0;
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
    {
        uint16_t ocf = HCI_OPCODE_OCF(s_vs_cmds[i].opcode);
        if (ocf < 64)
            supported |= (uint64_t)1 << ocf;
    }

    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_CC_EVT_KE_ID, 0, opcode,
                                                 (uint16_t)(1 + sizeof(supported)));
    if (p == NULL)
        return;
    p[0] = HCI_SUCCESS;
    for (size_t i = 0; i < sizeof(supported); i++)
        p[1 + i] = (uint8_t)(supported >> (8 * i)); /* little endian, as HCI wants */
    r_ip_funcs_p->hci_send_2_host_hack(p);
}

/* 0xFC02 SET_BDADDR: set the controller public address from the 6-byte payload. */
static void vs_set_bdaddr(uint16_t opcode, uint8_t length, const uint8_t *payload)
{
    if (length < BD_ADDR_LEN)
    {
        vs_cmd_complete_status(opcode, HCI_ERR_INVALID_PARAMS);
        return;
    }
    /* BR/EDR keeps its public address in the link driver env. That is what Read
     * BD_ADDR reports and what page/inquiry scan pick up when they are armed;
     * llm_util_* below only reaches the LE copy. */
    memcpy(&ld_env.bd_addr, payload, sizeof(ld_env.bd_addr));
    /* LE: update the stored public address, then program it into the radio. */
    r_ip_funcs_p->llm_util_set_public_addr((const bd_addr_t *)payload);
    r_ip_funcs_p->llm_util_apply_bd_addr(0);
    vs_cmd_complete_status(opcode, HCI_SUCCESS);
}

void vsc_cmd_received(uint16_t opcode, uint8_t length, uint8_t *payload)
{
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
        if (s_vs_cmds[i].opcode == opcode)
        {
            s_vs_cmds[i].handler(opcode, length, payload);
            return;
        }

    /* We own the whole vendor group, so an unclaimed opcode has to be answered
     * here: nothing downstream will do it for us. */
    vs_cmd_complete_status(opcode, HCI_ERR_UNKNOWN_HCI_CMD);
}
