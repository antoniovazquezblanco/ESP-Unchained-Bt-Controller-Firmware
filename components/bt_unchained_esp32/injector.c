/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the classic ESP32 (dual-mode RivieraWaves BTDM controller).
 *
 * BR/EDR LMP injection is the inverse of the LMP monitor: instead of tapping the
 * element the ROM hands to ld_acl_lmp_tx, we allocate an LMP TX element ourselves
 * (bt_util_buf_lmp_tx_alloc), write the raw PDU bytes into its exchange-memory
 * payload (em_buf_tx_buff_addr_get), set the length, and call ld_acl_lmp_tx. This
 * mirrors the controller's own r_lc_send_lmp (0x4001c1a8) minus the packer -- we
 * inject raw bytes, so any opcode, well-formed or not. The ROM frees the element
 * after air time; like r_lc_send_lmp we do not free after submit. Runs in the
 * controller task context the vendor-command handler is called in.
 *
 * Sequence/offsets are from the rev300 ROM disassembly; see
 * bt_rom_esp32/include/rom/{r_ip_funcs.h,bt_em_buf.h,ld_env.h}. LMP validated on
 * hardware.
 */
#include "injector.h"

#include <stddef.h>
#include <string.h>

#include "bt_rom_esp32.h"
#include "hci.h"
#include "pdu_inject.h"

/* Largest BR/EDR LMP PDU on air (the LMP TX pool buffer holds this). */
#define LMP_PDU_MAX 17

/* True when link is an active BR/EDR ACL link (and in range), so we never index
 * ld_acl_env out of bounds or hand the ROM a dead link. */
static _Bool acl_link_active(uint16_t link)
{
    return link < BT_ACL_CONN_MAX && ld_acl_env[link] != NULL;
}

/* Inject a BR/EDR LMP PDU (raw bytes, opcode first). */
static uint8_t inject_lmp(uint16_t link, const uint8_t *pdu, uint8_t len)
{
    if (len == 0 || len > LMP_PDU_MAX)
        return HCI_ERR_INVALID_PARAMS;
    if (!acl_link_active(link))
        return HCI_ERR_COMMAND_DISALLOWED;
    bt_em_lmp_buf_elt_t *elt = r_ip_funcs_p->bt_util_buf_lmp_tx_alloc();
    if (elt == NULL)
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    uint8_t *buf = r_ip_funcs_p->em_buf_tx_buff_addr_get(elt);
    memcpy(buf, pdu, len);
    elt->length = len;
    /* Owns elt from here (frees after TX), like r_lc_send_lmp -- never free below. */
    if (r_ip_funcs_p->ld_acl_lmp_tx(link, elt) != 0)
        return HCI_ERR_COMMAND_DISALLOWED;
    return HCI_SUCCESS;
}

uint8_t inject_pdu(const uint8_t *params, uint8_t length)
{
    if (length < PDU_INJECT_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    uint8_t kind = params[PDU_INJECT_OFF_KIND];
    uint16_t handle = (uint16_t)(params[PDU_INJECT_OFF_HANDLE] | (params[PDU_INJECT_OFF_HANDLE + 1] << 8));
    const uint8_t *pdu = params + PDU_INJECT_OFF_PDU;
    uint8_t pdu_len = (uint8_t)(length - PDU_INJECT_HDR_LEN);

    switch (kind) {
    case PDU_INJECT_KIND_LMP:
        return inject_lmp(handle, pdu, pdu_len);
    case PDU_INJECT_KIND_LL:
        /* Classic BLE-LL injection (em_desc_node + lld_pdu_data_tx_push) not yet
         * implemented -- see injector.h. The C3/S3/C5 builds inject LL. */
        return HCI_ERR_UNSUPPORTED_FEATURE;
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
}
