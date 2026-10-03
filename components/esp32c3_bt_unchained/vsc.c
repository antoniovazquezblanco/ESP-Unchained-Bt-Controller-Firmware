/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C3.
 *
 * The C3 reaches the host the same way the classic ESP32 does: allocate a
 * Command Complete message (ke_msg_alloc, id HCI_CC_EVT_KE_ID) with the opcode as
 * src, lay out status then return parameters, and hand it to hci_send_2_host.
 * esp32c3_bt_unchained wires our hci_cmd_received hook to call vsc_cmd_received
 * for our opcodes only; the controller keeps every other vendor command.
 */
#include "vsc.h"

#include <stddef.h>
#include <string.h>

#include "esp_app_desc.h"

#include "esp32c3_bt_rom.h"
#include "hci.h"
#include "traffic_monitor.h"

#ifndef ESP32C3_BT_UNCHAINED_BOARD
#define ESP32C3_BT_UNCHAINED_BOARD "unknown"
#endif

/* The common case: a Command Complete that returns nothing but status. */
static void vs_cmd_complete_status(uint16_t opcode, uint8_t status)
{
    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_CC_EVT_KE_ID, 0, opcode, 1);
    if (p == NULL)
        return;
    p[0] = status;
    r_ip_funcs_p->hci_send_2_host(p);
}

/* Command handler function type. */
typedef void (*vs_cmd_handler_fn_t)(uint16_t opcode, uint16_t length, const uint8_t *payload);

/* Handler table entry for a single vendor-specific command. */
typedef struct
{
    uint16_t opcode;
    vs_cmd_handler_fn_t handler;
} vs_cmd_t;

static void vs_info(uint16_t opcode, uint16_t length, const uint8_t *payload);
static void vs_supported_cmds(uint16_t opcode, uint16_t length, const uint8_t *payload);
static void vs_set_bdaddr(uint16_t opcode, uint16_t length, const uint8_t *payload);
static void vs_set_traffic_monitor(uint16_t opcode, uint16_t length, const uint8_t *payload);

/* The table of vendor-specific commands, mapping opcodes to their handlers. */
static const vs_cmd_t s_vs_cmds[] = {
    {HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_INFO_OCF), vs_info},
    {HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SUPPORTED_CMDS_OCF), vs_supported_cmds},
    {HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_BDADDR_OCF), vs_set_bdaddr},
    {HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF), vs_set_traffic_monitor},
};

/*
 * Reply packing. Building a Command Complete, r_hci_build_cc_evt looks the opcode
 * up with hci_look_for_cmd_desc and, when that returns NULL, overwrites the first
 * return byte -- our status -- with 0x01 "Unknown HCI Command". No ROM descriptor
 * covers our opcodes, so we hand it one of our own: the 0x80 flag says ret_fmt is
 * a self-pack function, and our return parameters are already laid out by the
 * handler, so packing is a no-op that reports success. par_size_max is 0xFF so an
 * incoming command carrying parameters is not clipped. One shared entry serves
 * every command in s_vs_cmds.
 */
static uint32_t vs_pack_in_place(uint8_t *out, uint8_t *in, uint16_t *len)
{
    (void)out;
    (void)in;
    (void)len;
    return 0;
}

static const esp32c3_hci_cmd_desc_t s_vs_cmd_desc = {
    .opcode = 0, /* never read back: r_hci_build_cc_evt takes the opcode from the message */
    .flags = 0x80,
    .par_size_max = 0xFF,
    .fn = NULL,
    .ret_fmt = vs_pack_in_place,
};

esp32c3_hci_cmd_desc_t *vsc_cmd_desc(uint16_t opcode)
{
    if (vsc_owns(opcode))
        return (esp32c3_hci_cmd_desc_t *)&s_vs_cmd_desc;
    return NULL;
}

/* 0xFC00 INFO: firmware name, firmware version and board name, each prefixed by
 * a one-byte length. */
static void vs_info(uint16_t opcode, uint16_t length, const uint8_t *payload)
{
    (void)length;
    (void)payload;
    const esp_app_desc_t *desc = esp_app_get_description();
    size_t project_name_len = strnlen(desc->project_name, sizeof(desc->project_name));
    size_t version_len = strnlen(desc->version, sizeof(desc->version));
    size_t board_name_len = sizeof(ESP32C3_BT_UNCHAINED_BOARD) - 1;

    uint16_t len = (uint16_t)(1 + 1 + project_name_len + 1 + version_len + 1 + board_name_len);
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
    memcpy(&p[n], ESP32C3_BT_UNCHAINED_BOARD, board_name_len);
    r_ip_funcs_p->hci_send_2_host(p);
}

/* 0xFC01 SUPPORTED_CMDS: a 64-bit bitfield, bit N set when the command with OCF
 * N is implemented. Our OCFs start at 0 and grow, so this covers the first 64. */
static void vs_supported_cmds(uint16_t opcode, uint16_t length, const uint8_t *payload)
{
    (void)length;
    (void)payload;
    uint64_t supported = 0;
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++) {
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
    r_ip_funcs_p->hci_send_2_host(p);
}

/* 0xFC02 SET_BDADDR: set the controller public address from the 6-byte payload.
 * p_llm_env->bd_addr is what Read_BD_ADDR reports and what the advertising/scan
 * paths read when they build PDUs. */
static void vs_set_bdaddr(uint16_t opcode, uint16_t length, const uint8_t *payload)
{
    if (length < BD_ADDR_LEN) {
        vs_cmd_complete_status(opcode, HCI_ERR_INVALID_PARAMS);
        return;
    }
    memcpy(p_llm_env->bd_addr, payload, BD_ADDR_LEN);
    vs_cmd_complete_status(opcode, HCI_SUCCESS);
}

/* 0xFC03 SET_TRAFFIC_MONITOR: enable/disable low-level LL PDU reporting. */
static void vs_set_traffic_monitor(uint16_t opcode, uint16_t length, const uint8_t *payload)
{
    if (length < 1) {
        vs_cmd_complete_status(opcode, HCI_ERR_INVALID_PARAMS);
        return;
    }
    if ((payload[0] & ~TRAFFIC_MONITOR_SUPPORTED) != 0) {
        /* A flag bit we have no hook for yet -- refuse rather than silently drop it. */
        vs_cmd_complete_status(opcode, HCI_ERR_UNSUPPORTED_FEATURE);
        return;
    }
    traffic_monitor_set(payload[0]);
    vs_cmd_complete_status(opcode, HCI_SUCCESS);
}

_Bool vsc_owns(uint16_t opcode)
{
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
        if (s_vs_cmds[i].opcode == opcode)
            return 1;
    return 0;
}

void vsc_cmd_received(uint16_t opcode, uint16_t length, const uint8_t *payload)
{
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
        if (s_vs_cmds[i].opcode == opcode) {
            s_vs_cmds[i].handler(opcode, length, payload);
            return;
        }
}
