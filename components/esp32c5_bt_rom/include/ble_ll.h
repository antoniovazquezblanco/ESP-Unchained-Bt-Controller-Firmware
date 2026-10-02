/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C5 BLE link-layer controller surface (libble_app, NimBLE lineage).
 *
 * The C5 has no Bluetooth ROM; the controller ships in libble_app.a and every
 * symbol below resolves at link time. These declarations model the slice the
 * unchained layer builds on: the vendor-specific HCI command dispatch and the
 * public-address setter. Verified against c5_ble_ll_hci.o / ble_ll.c.o.
 */

#ifndef ESP32C5_BLE_LL_H
#define ESP32C5_BLE_LL_H

#include <stddef.h>
#include <stdint.h>

/*
 * A vendor-specific (OGF 0x3F) command handler.
 *
 * r_ble_ll_hci_cmd_proc resolves the handler by matching the command's OCF
 * (opcode & 0x3ff) against the vs-command list, calls it, then packs and sends
 * the Command Complete itself from the returned status and the rsp buffer -- the
 * handler never allocates or sends an event.
 *
 *   params   command parameters (the bytes after the 3-byte HCI command header)
 *   len      number of parameter bytes
 *   rsp      return-parameter buffer; write the Command Complete return
 *            parameters here (room for a full HCI command buffer)
 *   rsp_len  out: number of return-parameter bytes written to rsp
 *   returns  BLE/HCI status (0 = success)
 */
typedef int (*ble_ll_hci_vs_cmd_fn_t)(const uint8_t *params, uint8_t len, uint8_t *rsp, uint8_t *rsp_len);

/*
 * One node in the controller's vendor-specific command list. The dispatcher
 * walks the singly linked list at ble_ll_hci_env_p->vs_cmds and matches `ocf`;
 * head-inserting a node adds a command (and shadows any later node with the same
 * ocf).
 */
typedef struct ble_ll_hci_vs_cmd
{
    uint16_t ocf;                   /* +0 matched against opcode & 0x3ff */
    uint16_t _reserved;             /* +2 alignment pad */
    ble_ll_hci_vs_cmd_fn_t cb;      /* +4 handler */
    struct ble_ll_hci_vs_cmd *next; /* +8 next node, NULL at the tail */
} ble_ll_hci_vs_cmd_t;

_Static_assert(offsetof(ble_ll_hci_vs_cmd_t, cb) == 4, "vs-cmd handler must sit at +4");
_Static_assert(offsetof(ble_ll_hci_vs_cmd_t, next) == 8, "vs-cmd next must sit at +8");

/*
 * The HCI environment. Only the vendor-command list head (offset 0x34) is
 * modelled; the rest is controller-private. Allocated by the controller at
 * init, so the pointer is valid only after esp_bt_controller_enable().
 */
typedef struct ble_ll_hci_env
{
    uint8_t _reserved_00[0x34];   /* 0x00..0x33 controller-private */
    ble_ll_hci_vs_cmd_t *vs_cmds; /* 0x34 head of the vendor-command list */
} ble_ll_hci_env_t;

_Static_assert(offsetof(ble_ll_hci_env_t, vs_cmds) == 0x34, "vs_cmds list head must sit at 0x34");

/* The live HCI environment (libble_app BSS pointer; NULL before controller init). */
extern ble_ll_hci_env_t *ble_ll_hci_env_p;

/*
 * Set the controller public identity address. Copies the six bytes into the
 * controller's public-address storage and programs them into the PHY address
 * filter (r_ble_phy_set_dev_address, type 0 = public). One call re-brands the
 * address reported by HCI Read_BD_ADDR and used on air. Returns 0.
 */
extern int r_esp_ble_ll_set_public_addr(const uint8_t *addr);

#endif /* ESP32C5_BLE_LL_H */
