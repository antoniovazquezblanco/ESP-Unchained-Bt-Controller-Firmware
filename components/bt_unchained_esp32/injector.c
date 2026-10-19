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
 * BLE LL injection follows the controller's own two senders, split by LLID:
 *  - data (llid 1/2): clone the data path. Build the small request
 *    {conhdl, llid, length, node} the ROM makes and hand it to lld_pdu_data_send,
 *    which allocates the TX descriptor, pushes it on the connection and frees the
 *    buffer after air time. The request's node is an ACL TX buffer (em_buf_node
 *    from bt_util_buf_acl_tx_alloc) whose EM payload we fill. Request layout and
 *    buffer semantics were pinned on-chip (a slot-597 tap read the request of a
 *    genuine host-ACL send, then dereferenced it to the EM payload).
 *  - control (llid 3): the data path forces a data LLID, so control goes through
 *    r_llc_llcp_tester_send (rom/llc.h), the ROM's raw-LLCP test path, which puts
 *    the given bytes on air verbatim as an LLID-3 control PDU (any opcode).
 *
 * Sequence/offsets are from the rev300 ROM disassembly and on-chip capture; see
 * bt_rom_esp32/include/rom/{r_ip_funcs.h,bt_em_buf.h,em.h,ld_env.h,llc.h}.
 * Validated on hardware.
 */
#include "injector.h"

#include <stddef.h>
#include <string.h>

#include "bt_rom_esp32.h"
#include "hci.h"
#include "pdu_inject.h"

/* Largest BR/EDR LMP PDU on air (the LMP TX pool buffer holds this). */
#define LMP_PDU_MAX 17

/* Largest BLE LL PDU we inject in a single shot. lld_pdu_data_send transmits one
 * PDU without fragmenting, so this stays at the default max TX octets -- a longer
 * payload would need the connection's negotiated size. */
#define LL_PDU_MAX 27

/*
 * The request lld_pdu_data_send (slot 597) consumes, as the ROM's own LL data
 * path builds it. Only these four fields are read; the padding keeps the node
 * pointer 4-byte aligned (the ROM loads it with a 32-bit access) and the struct
 * at the 16 bytes the path allocates. Pinned on-chip.
 */
typedef struct
{
    uint16_t conhdl;          /* +0  connection handle */
    uint8_t llid;             /* +2  on-air LLID: 1/2 data, 3 control */
    uint8_t _pad3;            /* +3 */
    uint16_t length;          /* +4  PDU length */
    uint16_t _pad6;           /* +6 */
    struct em_buf_node *node; /* +8  ACL TX buffer holding the EM payload */
    uint32_t _pad12;          /* +12 */
} lld_data_tx_req_t;

_Static_assert(sizeof(lld_data_tx_req_t) == 16, "request is 16 bytes");
_Static_assert(offsetof(lld_data_tx_req_t, node) == 8, "node at +8");

/* True when link is an active BR/EDR ACL link (and in range), so we never index
 * ld_acl_env out of bounds or hand the ROM a dead link. */
static _Bool acl_link_active(uint16_t link)
{
    return link < BT_ACL_CONN_MAX && ld_acl_env[link] != NULL;
}

/* True when handle is an active BLE connection, so we never index llc_env out of
 * bounds or hand the ROM data-send path a dead link. */
static _Bool le_link_active(uint16_t handle)
{
    return handle < BT_LE_CONN_MAX && llc_env[handle] != NULL;
}

/* Inject one BLE LL data PDU (llid 1/2) via the ROM's data-send path. */
static uint8_t inject_ll_data(uint16_t handle, uint8_t llid, const uint8_t *payload, uint8_t len)
{
    if (len == 0 || len > LL_PDU_MAX)
        return HCI_ERR_INVALID_PARAMS;
    if (!le_link_active(handle))
        return HCI_ERR_COMMAND_DISALLOWED;
    struct em_buf_node *node = r_ip_funcs_p->bt_util_buf_acl_tx_alloc();
    if (node == NULL)
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    memcpy((uint8_t *)em + node->buf_ptr, payload, len);
    lld_data_tx_req_t req = {.conhdl = handle, .llid = llid, .length = len, .node = node};
    /* data_send owns the buffer on success (frees it after TX); on failure (its
     * own descriptor alloc failed) it never took it, so we free it by index. */
    if (!r_ip_funcs_p->lld_pdu_data_send((uint16_t *)&req)) {
        r_ip_funcs_p->bt_util_buf_acl_tx_free(node->idx);
        return HCI_ERR_COMMAND_DISALLOWED;
    }
    return HCI_SUCCESS;
}

/* Inject one BLE LL control (LLCP) PDU verbatim via the ROM's raw LLCP sender --
 * any opcode, well-formed or not, exactly as given (opcode first). */
static uint8_t inject_ll_control(uint16_t handle, const uint8_t *payload, uint8_t len)
{
    if (len == 0 || len > LLC_LLCP_PDU_MAX)
        return HCI_ERR_INVALID_PARAMS;
    if (!le_link_active(handle))
        return HCI_ERR_COMMAND_DISALLOWED;
    r_llc_llcp_tester_send(handle, len, payload);
    return HCI_SUCCESS;
}

/* Parse the LL PDU sub-header ([llid][length][payload]) and inject by LLID. */
static uint8_t inject_ll(uint16_t handle, const uint8_t *pdu, uint16_t pdu_len)
{
    if (pdu_len < PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    uint8_t llid = pdu[PDU_INJECT_LL_OFF_LLID];
    uint8_t len = pdu[PDU_INJECT_LL_OFF_LENGTH];
    const uint8_t *payload = pdu + PDU_INJECT_LL_OFF_PAYLOAD;
    if (len > pdu_len - PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    switch (llid) {
    case PDU_INJECT_LLID_CONTROL:
        return inject_ll_control(handle, payload, len);
    case PDU_INJECT_LLID_DATA_CONT:
    case PDU_INJECT_LLID_DATA_START:
        return inject_ll_data(handle, llid, payload, len);
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
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
        return inject_ll(handle, pdu, pdu_len);
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
}
