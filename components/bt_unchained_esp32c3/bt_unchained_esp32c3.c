/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Unlocks the ESP32-C3 controller with our own vendor-specific commands.
 *
 * The C3 controller is RivieraWaves, reached through the same writable
 * function-pointer tables as the classic ESP32 (r_ip_funcs_p / r_modules_funcs_p
 * from esp32c3.rom.ld). We hook the HCI command ingress slot to answer our own
 * vendor group (Company 0xF00D: INFO, SUPPORTED_CMDS, SET_BDADDR,
 * SET_TRAFFIC_MONITOR, SET_SCAN_CHANNEL) and chain the controller for everything
 * else, and tap the link layer for the traffic monitor and scan-channel pin.
 */
#include "bt_unchained.h"

#include "esp_log.h"

#include "bt_rom_esp32c3.h"
#include "scan_pin.h"
#include "traffic_monitor.h"
#include "vsc.h"

static const char *TAG = "UNCHAINED";

/* Identifiable company id stamped over the controller default (0x02E5). */
#define UNCHAINED_COMPID 0xF00D

/* The controller HCI command ingress we replace (r_ip_funcs slot +0x2c), saved
 * so we can chain it for every command that is not ours. */
static r_hci_cmd_received_fn_t s_orig_hci_cmd_received;

/* The command-descriptor lookup we replace (r_ip_funcs slot +0x90), saved so we
 * can chain it for every opcode that is not ours. */
static r_hci_look_for_cmd_desc_fn_t s_orig_look_for_cmd_desc;

/* The event-descriptor lookup we replace (r_ip_funcs slot +0x98), saved so we can
 * chain it; our capture event (0xFF) needs a descriptor to pack cleanly. */
static r_hci_look_for_evt_desc_fn_t s_orig_look_for_evt_desc;

/* The received-PDU handler we replace (r_ip_funcs slot +0x394), saved so we can
 * chain it; tapped for the LL RX traffic monitor. */
static r_lld_con_rx_llcp_check_fn_t s_orig_lld_con_rx_llcp_check;

/* The outgoing data/control PDU queues we replace (r_ip_funcs slots +0x330 and
 * +0x368), saved so we can chain them; tapped for the LL TX traffic monitor. */
static r_lld_con_tx_fn_t s_orig_lld_con_data_tx;
static r_lld_con_tx_fn_t s_orig_lld_con_llcp_tx;

/* The scan scheduler we replace (r_ip_funcs slot +0x430), saved so we can chain
 * it; used to pin advertising reception to one primary channel. */
static r_lld_scan_sched_fn_t s_orig_lld_scan_sched;

/* Our replacement for the HCI command ingress: our vendor group is ours, every
 * other command stays with the controller. */
static void unchained_hci_cmd_received(uint16_t opcode, uint16_t param_len, uint8_t *param_buf)
{
    if (vsc_owns(opcode)) {
        /* Consume the host command credit the controller would have consumed in
         * r_hci_cmd_received; our Command Complete returns one in hci_tx_start,
         * and an unmatched return trips the hci_tl.c:1008 assert. */
        hci_tl_env.nb_h2c_cmd_pkts--;
        vsc_cmd_received(opcode, param_len, param_buf);
        return;
    }
    s_orig_hci_cmd_received(opcode, param_len, param_buf);
}

/* Our replacement for the command-descriptor lookup: the controller packs our
 * Command Complete with our status intact instead of stamping 0x01 over it. */
static void *unchained_look_for_cmd_desc(uint16_t opcode)
{
    esp32c3_hci_cmd_desc_t *desc = vsc_cmd_desc(opcode);
    if (desc != NULL)
        return desc;
    return s_orig_look_for_cmd_desc(opcode);
}

/* Our replacement for the event-descriptor lookup: the controller packs our
 * traffic-capture event (0xFF) instead of dropping it on a missing descriptor. */
static void *unchained_look_for_evt_desc(uint8_t evt_code)
{
    esp32c3_hci_evt_desc_t *desc = traffic_monitor_evt_desc(evt_code);
    if (desc != NULL)
        return desc;
    return s_orig_look_for_evt_desc(evt_code);
}

/* Our replacement for the received-PDU handler: tap the incoming LL PDU (a no-op
 * unless monitoring is on), then run the real handler unchanged. */
static uint32_t unchained_lld_con_rx_llcp_check(uint32_t link_id, void *con_env, uint32_t llid, uint16_t length)
{
    traffic_monitor_on_ll_rx(link_id, llid, length);
    return s_orig_lld_con_rx_llcp_check(link_id, con_env, llid, length);
}

/* Our replacements for the outgoing-PDU queues: tap the PDU as it is queued (a
 * no-op unless monitoring is on), then run the real handler unchanged. The tap
 * runs before lld_con_tx_prog fragments the element, so it sees the whole PDU. */
static uint8_t unchained_lld_con_data_tx(uint32_t link_id, void *tx_elem)
{
    traffic_monitor_on_ll_tx(link_id, (const lld_tx_elem_t *)tx_elem, 2 /* data */);
    return s_orig_lld_con_data_tx(link_id, tx_elem);
}

static uint8_t unchained_lld_con_llcp_tx(uint32_t link_id, void *tx_elem)
{
    traffic_monitor_on_ll_tx(link_id, (const lld_tx_elem_t *)tx_elem, 3 /* control */);
    return s_orig_lld_con_llcp_tx(link_id, tx_elem);
}

/* Our replacement for the scan scheduler: re-apply the channel pin (a no-op unless
 * one is set) before the controller sets up the next scan window. */
static void unchained_lld_scan_sched(uint32_t scan_idx, uint32_t param2, uint32_t param3)
{
    scan_pin_on_scan_sched(scan_idx);
    s_orig_lld_scan_sched(scan_idx, param2, param3);
}

void bt_unchained_init(void)
{
    /* Validate the controller dispatch tables are available. */
    if (r_ip_funcs_p == NULL || r_modules_funcs_p == NULL) {
        ESP_LOGE(TAG, "controller dispatch tables not available");
        return;
    }

    /* Hook the HCI command ingress so our vendor group is answered here. */
    s_orig_hci_cmd_received = r_ip_funcs_p->hci_cmd_received;
    r_ip_funcs_p->hci_cmd_received = &unchained_hci_cmd_received;

    /* Hook the descriptor lookup so our replies keep their status byte. */
    s_orig_look_for_cmd_desc = r_ip_funcs_p->hci_look_for_cmd_desc;
    r_ip_funcs_p->hci_look_for_cmd_desc = &unchained_look_for_cmd_desc;

    /* Hook the event descriptor lookup so our capture events pack cleanly. */
    s_orig_look_for_evt_desc = r_ip_funcs_p->hci_look_for_evt_desc;
    r_ip_funcs_p->hci_look_for_evt_desc = &unchained_look_for_evt_desc;

    /* Tap incoming LL PDUs: installed once, gated by the traffic monitor flags. */
    s_orig_lld_con_rx_llcp_check = r_ip_funcs_p->lld_con_rx_llcp_check;
    r_ip_funcs_p->lld_con_rx_llcp_check = &unchained_lld_con_rx_llcp_check;

    /* Tap outgoing LL PDUs (data + control): installed once, gated by the flags. */
    s_orig_lld_con_data_tx = r_ip_funcs_p->lld_con_data_tx;
    r_ip_funcs_p->lld_con_data_tx = &unchained_lld_con_data_tx;
    s_orig_lld_con_llcp_tx = r_ip_funcs_p->lld_con_llcp_tx;
    r_ip_funcs_p->lld_con_llcp_tx = &unchained_lld_con_llcp_tx;

    /* Re-apply the scan channel pin on each scan window (a no-op unless pinned). */
    s_orig_lld_scan_sched = r_ip_funcs_p->lld_scan_sched;
    r_ip_funcs_p->lld_scan_sched = &unchained_lld_scan_sched;

    /* Re-brand the controller with an identifiable company id. */
    sdk_cfg_priv_opts.company_id = UNCHAINED_COMPID;
    ESP_LOGI(TAG, "unchained vendor commands installed, company id 0x%04X", UNCHAINED_COMPID);

    ESP_LOGI(TAG, "bt_rom_esp32c3 selftest: %s",
             bt_rom_esp32c3_selftest() ? "ok" : "FAILED");
}
