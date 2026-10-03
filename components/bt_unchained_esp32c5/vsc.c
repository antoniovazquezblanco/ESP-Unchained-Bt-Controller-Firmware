/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C5.
 *
 * The NimBLE-lineage controller (libble_app) dispatches OGF-0x3F commands from a
 * linked list of {ocf, handler} nodes and packs the Command Complete from each
 * handler's return status and output buffer. We head-insert our own nodes, so a
 * handler only fills the return parameters and returns a status -- no event
 * allocation, unlike the classic ESP32 path.
 */
#include "vsc.h"

#include <stddef.h>
#include <string.h>

#include "esp_app_desc.h"

#include "bt_rom_esp32c5.h"
#include "hci.h"
#include "scan_pin.h"
#include "traffic_monitor.h"

#ifndef BT_UNCHAINED_ESP32C5_BOARD
#define BT_UNCHAINED_ESP32C5_BOARD "unknown"
#endif

/*
 * Prototypes for the vendor-specific command handlers. Each matches the
 * controller's vs-command ABI (ble_ll_hci_vs_cmd_fn_t): read params[0..len),
 * write the Command Complete return parameters to rsp, set *rsp_len, return the
 * status the controller stamps into the event.
 */
static int vs_info(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);
static int vs_supported_cmds(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);
static int vs_set_bdaddr(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);
static int vs_set_traffic_monitor(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);
static int vs_set_scan_channel(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);

/* The commands we register, one list node each. `next` is wired in vsc_register. */
static ble_ll_hci_vs_cmd_t s_vs_cmds[] = {
    {.ocf = UNCHAINED_VS_INFO_OCF, .cb = vs_info},
    {.ocf = UNCHAINED_VS_SUPPORTED_CMDS_OCF, .cb = vs_supported_cmds},
    {.ocf = UNCHAINED_VS_SET_BDADDR_OCF, .cb = vs_set_bdaddr},
    {.ocf = UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF, .cb = vs_set_traffic_monitor},
    {.ocf = UNCHAINED_VS_SET_SCAN_CHANNEL_OCF, .cb = vs_set_scan_channel},
};

/* 0xFC00 INFO: firmware name, firmware version and board name, each prefixed by
 * a one-byte length. The controller adds the leading status byte. */
static int vs_info(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len)
{
    (void)params;
    (void)len;
    const esp_app_desc_t *desc = esp_app_get_description();
    size_t project_name_len = strnlen(desc->project_name, sizeof(desc->project_name));
    size_t version_len = strnlen(desc->version, sizeof(desc->version));
    size_t board_name_len = sizeof(BT_UNCHAINED_ESP32C5_BOARD) - 1;

    size_t n = 0;
    rsp[n++] = (uint8_t)project_name_len;
    memcpy(&rsp[n], desc->project_name, project_name_len);
    n += project_name_len;
    rsp[n++] = (uint8_t)version_len;
    memcpy(&rsp[n], desc->version, version_len);
    n += version_len;
    rsp[n++] = (uint8_t)board_name_len;
    memcpy(&rsp[n], BT_UNCHAINED_ESP32C5_BOARD, board_name_len);
    n += board_name_len;
    *rsp_len = (uint8_t)n;
    return HCI_SUCCESS;
}

/* 0xFC01 SUPPORTED_CMDS: a 64-bit bitfield, bit N set when the command with OCF
 * N is implemented. Our OCFs start at 0 and grow, so this covers the first 64. */
static int vs_supported_cmds(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len)
{
    (void)params;
    (void)len;
    uint64_t supported = 0;
    for (size_t i = 0; i < sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]); i++)
        if (s_vs_cmds[i].ocf < 64)
            supported |= (uint64_t)1 << s_vs_cmds[i].ocf;

    for (size_t i = 0; i < sizeof(supported); i++)
        rsp[i] = (uint8_t)(supported >> (8 * i)); /* little endian, as HCI wants */
    *rsp_len = (uint8_t)sizeof(supported);
    return HCI_SUCCESS;
}

/* 0xFC02 SET_BDADDR: set the controller public address from the 6-byte payload.
 * r_esp_ble_ll_set_public_addr stores it and programs the PHY address filter. */
static int vs_set_bdaddr(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len)
{
    (void)rsp;
    *rsp_len = 0;
    if (len < BD_ADDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    r_esp_ble_ll_set_public_addr(params);
    return HCI_SUCCESS;
}

/* 0xFC03 SET_TRAFFIC_MONITOR: enable/disable low-level LL PDU reporting. */
static int vs_set_traffic_monitor(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len)
{
    (void)rsp;
    *rsp_len = 0;
    if (len < 1)
        return HCI_ERR_INVALID_PARAMS;
    if ((params[0] & ~TRAFFIC_MONITOR_SUPPORTED) != 0)
        /* A flag bit we have no hook for -- refuse rather than silently drop it. */
        return HCI_ERR_UNSUPPORTED_FEATURE;
    traffic_monitor_set(params[0]);
    return HCI_SUCCESS;
}

/* 0xFC04 SET_SCAN_CHANNEL: pin scanning to one primary channel (scan_pin.h). */
static int vs_set_scan_channel(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len)
{
    (void)rsp;
    *rsp_len = 0;
    if (len < 1)
        return HCI_ERR_INVALID_PARAMS;
    if (!scan_pin_set(params[0]))
        return HCI_ERR_INVALID_PARAMS;
    return HCI_SUCCESS;
}

void vsc_register(void)
{
    if (ble_ll_hci_env_p == NULL)
        return;
    size_t count = sizeof(s_vs_cmds) / sizeof(s_vs_cmds[0]);
    /* Chain our nodes, then head-insert the chain so our opcodes resolve before
     * any vendor command the controller may already have registered with that OCF. */
    for (size_t i = 0; i + 1 < count; i++)
        s_vs_cmds[i].next = &s_vs_cmds[i + 1];
    s_vs_cmds[count - 1].next = ble_ll_hci_env_p->vs_cmds;
    ble_ll_hci_env_p->vs_cmds = &s_vs_cmds[0];
}
