/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Patch layer over the ESP32 BR/EDR controller ROM.
 */
#include "bt_unchained.h"

#include <stddef.h>
#include <stdint.h>

#include "esp32_bt_rom.h"
#include "hci.h"
#include "lmp_monitor.h"
#include "scan_pin.h"
#include "vsc.h"

/* Identifiable company id stamped over the ROM default (0x0060). */
#define UNCHAINED_COMPID 0xF00D

/* The command-descriptor lookup we replace (slot 85), saved so we can chain it. */
static r_hci_look_for_cmd_desc_hack_fn_t s_orig_look_for_cmd_desc;

/* The event-descriptor lookup we replace (slot 86), saved so we can chain it. */
static r_hci_look_for_evt_desc_fn_t s_orig_look_for_evt_desc;

/* The vendor-command handler we replace (slot 102), saved so we can chain it. */
static r_hci_cmd_received_fn_t s_orig_hci_cmd_received;

/* The outgoing-LMP handler we replace (slot 336), saved so we can chain it. */
static r_ld_acl_lmp_tx_fn_t s_orig_ld_acl_lmp_tx;

/* The incoming-LMP unpacker we replace (slot 21), saved so we can chain it. */
static r_lmp_unpack_fn_t s_orig_lmp_unpack;

/* The outgoing-LL submit we replace (slot 596), saved so we can chain it. */
static r_lld_pdu_data_tx_push_fn_t s_orig_lld_pdu_data_tx_push;

/* The incoming-LL drain we replace (slot 603), saved so we can chain it. */
static r_lld_pdu_rx_handler_fn_t s_orig_lld_pdu_rx_handler;

/* The scan-start we replace (slot 576), saved so we can chain it. */
static r_lld_scan_start_hack_fn_t s_orig_lld_scan_start;

/* Our replacement for hci_look_for_cmd_desc_hack. The ROM calls it while packing
 * a Command Complete and stamps status 0x01 over the reply when it comes back
 * empty, so our own commands have to answer with a descriptor. Everything else
 * stays with the Espressif lookup. */
static hci_cmd_desc_t *unchained_look_for_cmd_desc(uint16_t opcode)
{
    hci_cmd_desc_t *desc = vsc_cmd_desc(opcode);
    if (desc != NULL) {
        return desc;
    }
    return s_orig_look_for_cmd_desc(opcode);
}

/* Our replacement for hci_look_for_evt_desc. The traffic monitor emits a vendor
 * event (0xFF) that the ROM has no descriptor for; without one the pack path
 * errors, so we answer for our code and chain the ROM for the rest. */
static hci_evt_desc_t *unchained_look_for_evt_desc(uint8_t evt_code)
{
    hci_evt_desc_t *desc = lmp_monitor_evt_desc(evt_code);
    if (desc != NULL) {
        return desc;
    }
    return s_orig_look_for_evt_desc(evt_code);
}

/* Our replacement for r_hci_cmd_received: the whole vendor group is ours, every
 * other group stays with the ROM. */
static void unchained_hci_cmd_received(uint16_t opcode, uint8_t length, uint8_t *payload)
{
    if (HCI_OPCODE_OGF(opcode) == HCI_OGF_VENDOR) {
        vsc_cmd_received(opcode, length, payload);
        return;
    }
    s_orig_hci_cmd_received(opcode, length, payload);
}

/* Our replacement for r_ld_acl_lmp_tx: tap the outgoing LMP PDU (a no-op unless
 * monitoring is on), then run the real handler unchanged. */
static uint32_t unchained_ld_acl_lmp_tx(uint32_t link_id, bt_em_lmp_buf_elt_t *buf_elt)
{
    lmp_monitor_on_lmp_tx(link_id, buf_elt);
    return s_orig_ld_acl_lmp_tx(link_id, buf_elt);
}

/* On-air length of a received LMP PDU, opcode byte(s) included; 0 when the opcode
 * has no descriptor. The same lookup lmp_unpack does, repeated here because the
 * length it resolves is not one it hands back -- see the hook below. */
static uint8_t lmp_pdu_len(const uint8_t *pdu)
{
    const lmp_desc_t *tab = lmp_desc_tab;
    size_t count = LMP_DESC_TAB_SIZE;
    uint8_t opcode = pdu[0] >> 1;

    if (opcode == LMP_OPCODE_ESCAPE) {
        tab = lmp_ext_desc_tab;
        count = LMP_EXT_DESC_TAB_SIZE;
        opcode = pdu[1];
    }

    for (size_t i = 0; i < count; i++)
        if (tab[i].opcode == opcode)
            return tab[i].len;

    return 0;
}

/* Our replacement for r_lmp_unpack: run the real unpacker first, then tap the
 * incoming PDU on success (a no-op unless monitoring is on). The unpacker only
 * writes `out`, leaving `in` -- the wire bytes -- intact, but it hands *len back
 * as the length of the unpacked struct, which alignment padding pushes past the
 * PDU, so the capture length comes from the opcode descriptor instead. A
 * non-zero status is a malformed/unknown PDU the ROM itself discards, so we
 * leave those uncaptured. */
static uint8_t unchained_lmp_unpack(uint8_t *out, uint8_t *in, uint8_t *len)
{
    uint8_t status = s_orig_lmp_unpack(out, in, len);
    if (status == 0) {
        lmp_monitor_on_lmp_rx(in, lmp_pdu_len(in));
    }
    return status;
}

/* Our replacement for r_lld_pdu_data_tx_push: tap the outgoing LL PDU (a no-op
 * unless monitoring is on) while the descriptor still describes it, then run the
 * real handler unchanged. */
static void unchained_lld_pdu_data_tx_push(int32_t lld_env, int32_t tx_desc, uint8_t prog)
{
    lmp_monitor_on_ll_tx(tx_desc);
    s_orig_lld_pdu_data_tx_push(lld_env, tx_desc, prog);
}

/* Our replacement for r_lld_pdu_rx_handler: tap the received LL PDUs before the
 * real handler drains the ring and frees their buffers. */
static void unchained_lld_pdu_rx_handler(int32_t lld_env, uint8_t nb_rx)
{
    lmp_monitor_on_ll_rx(nb_rx);
    s_orig_lld_pdu_rx_handler(lld_env, nb_rx);
}

/* Our replacement for r_lld_scan_start: let the ROM set the scan up with the
 * full channel map, then re-apply the channel pin (a no-op unless one is set). */
static int32_t unchained_lld_scan_start(int32_t scan_par, int32_t pdu)
{
    int32_t evt = s_orig_lld_scan_start(scan_par, pdu);
    scan_pin_on_scan_start();
    return evt;
}

void bt_unchained_init(void)
{
    // Validate that the required function pointers are available
    if (r_ip_funcs_p == NULL || r_modules_funcs_p == NULL) {
        return;
    }

    // Re-brand controller with an identifiable Company ID
    co_default_compid = UNCHAINED_COMPID;

    // Override the command descriptor lookup so our replies keep their status
    s_orig_look_for_cmd_desc = r_ip_funcs_p->hci_look_for_cmd_desc_hack;
    r_ip_funcs_p->hci_look_for_cmd_desc_hack = &unchained_look_for_cmd_desc;

    // Override the event descriptor lookup so our capture events pack cleanly
    s_orig_look_for_evt_desc = r_ip_funcs_p->hci_look_for_evt_desc;
    r_ip_funcs_p->hci_look_for_evt_desc = &unchained_look_for_evt_desc;

    // Override the vendor command handler
    s_orig_hci_cmd_received = r_ip_funcs_p->hci_cmd_received;
    r_ip_funcs_p->hci_cmd_received = &unchained_hci_cmd_received;

    // Tap outgoing LMP: installed once, gated by the traffic monitor flags
    s_orig_ld_acl_lmp_tx = r_ip_funcs_p->ld_acl_lmp_tx;
    r_ip_funcs_p->ld_acl_lmp_tx = &unchained_ld_acl_lmp_tx;

    // Tap incoming LMP: installed once, gated by the traffic monitor flags
    s_orig_lmp_unpack = r_ip_funcs_p->lmp_unpack;
    r_ip_funcs_p->lmp_unpack = &unchained_lmp_unpack;

    // Tap outgoing LL: installed once, gated by the traffic monitor flags
    s_orig_lld_pdu_data_tx_push = r_ip_funcs_p->lld_pdu_data_tx_push;
    r_ip_funcs_p->lld_pdu_data_tx_push = &unchained_lld_pdu_data_tx_push;

    // Tap incoming LL: installed once, gated by the traffic monitor flags
    s_orig_lld_pdu_rx_handler = r_ip_funcs_p->lld_pdu_rx_handler;
    r_ip_funcs_p->lld_pdu_rx_handler = &unchained_lld_pdu_rx_handler;

    // Re-apply the scan channel pin after each scan starts
    s_orig_lld_scan_start = r_ip_funcs_p->lld_scan_start_hack;
    r_ip_funcs_p->lld_scan_start_hack = &unchained_lld_scan_start;
}
