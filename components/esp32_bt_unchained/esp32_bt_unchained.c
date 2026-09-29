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
}
