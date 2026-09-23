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
#include "vsc.h"

/* Identifiable company id stamped over the ROM default (0x0060). */
#define UNCHAINED_COMPID 0xF00D

/* The vendor-command handler we replace (slot 102), saved so we can chain it. */
static r_hci_cmd_received_fn_t s_orig_hci_cmd_received;

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

void bt_unchained_init(void)
{
    // Validate that the required function pointers are available
    if (r_ip_funcs_p == NULL || r_modules_funcs_p == NULL) {
        return;
    }

    // Re-brand controller with an identifiable Company ID
    co_default_compid = UNCHAINED_COMPID;

    // Override the vendor command handler
    s_orig_hci_cmd_received = r_ip_funcs_p->hci_cmd_received;
    r_ip_funcs_p->hci_cmd_received = &unchained_hci_cmd_received;
}
