/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the ESP32-S3 (same RivieraWaves BLE controller as the C3).
 *
 * The inverse of the traffic monitor: instead of tapping the PDU the ROM queues
 * with lld_con_llcp_tx / lld_con_data_tx, we allocate a TX buffer ourselves
 * (ble_util_buf.h), write the crafted bytes into its exchange-memory payload, and
 * call those same queue functions. The ROM frees the buffer after air time, so we
 * free it only when the submit is refused. Runs in the controller task context the
 * vendor-command handler is called in; the queue functions take their own osi lock.
 *
 * Sequence and buffer semantics are from the C3 rev3 ROM disassembly (the S3 ROM is
 * the same controller); see bt_rom_esp32s3/include/rom/{ble_util_buf.h,llc.h}.
 * Validated on hardware.
 */
#include "injector.h"

#include <stddef.h>
#include <string.h>

#include "bt_rom_esp32s3.h"
#include "hci.h"
#include "pdu_inject.h"

/* True when link is an active connection (and in range), so we never index
 * lld_con_env out of bounds or hand the ROM a dead link. */
static _Bool link_active(uint16_t link)
{
    return link < ESP32S3_LLD_CON_MAX && lld_con_env[link] != NULL;
}

/* Inject a BLE LL control (LLCP) PDU. The ROM's TX-confirm pops the head of LLC's
 * llcp_tx_queue on every LLCP ack (rom/llc.h), so we do what llc_llcp_tx_check
 * does for its own PDUs, minus its per-opcode gate: one in flight, our element at
 * the head with no cfm_cb, busy flag set. The confirm then frees it and resumes
 * LLC's queue. LLID 3 is implied by the control queue, so we set length only. */
static uint8_t inject_ll_control(uint16_t link, const uint8_t *payload, uint8_t len)
{
    if (len > BLE_UTIL_BUF_LLCP_MAX)
        return HCI_ERR_INVALID_PARAMS;
    llc_env_t *env = llc_env[link];
    if (env == NULL || (env->flags & LLC_FLAG_LLCP_TX_BUSY))
        return HCI_ERR_COMMAND_DISALLOWED; /* an LLCP is in flight: retry */
    llc_llcp_tx_elt_t *llc_elt = r_modules_funcs_p->ke_malloc(sizeof(*llc_elt) + len, KE_MEM_KE_MSG);
    if (llc_elt == NULL)
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    lld_tx_elem_t *elt = r_ble_util_buf_llcp_tx_alloc();
    if (elt == NULL) {
        r_modules_funcs_p->ke_free(llc_elt);
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    }
    llc_elt->cfm_cb = NULL;
    llc_elt->length = len;
    memcpy(llc_elt->pdu, payload, len);
    uint8_t *buf = (uint8_t *)r_plf_funcs_p->em_buf_get(elt->buf_handle);
    memcpy(buf, payload, len);
    memset(elt->_hdr, 0, sizeof(elt->_hdr)); /* co_list next = NULL */
    elt->length_flags = len;
    r_modules_funcs_p->co_list_push_front(&env->llcp_tx_queue, llc_elt);
    env->flags |= LLC_FLAG_LLCP_TX_BUSY;
    if (r_ip_funcs_p->lld_con_llcp_tx(link, elt) != 0) {
        env->flags &= (uint16_t)~LLC_FLAG_LLCP_TX_BUSY;
        r_modules_funcs_p->co_list_extract(&env->llcp_tx_queue, llc_elt);
        r_modules_funcs_p->ke_free(llc_elt);
        r_ble_util_buf_llcp_tx_free(elt->buf_handle);
        return HCI_ERR_COMMAND_DISALLOWED;
    }
    return HCI_SUCCESS;
}

/* Inject a BLE LL data PDU via the ACL pool: alloc returns a handle, mapped to its
 * element and EM payload. lld_con_tx_prog stamps the data LLID (start) at program
 * time, so the host LLID 1/2 distinction is not honoured in v1. */
static uint8_t inject_ll_data(uint16_t link, const uint8_t *payload, uint8_t len)
{
    if (len > BLE_UTIL_BUF_ACL_MAX)
        return HCI_ERR_INVALID_PARAMS;
    uint16_t handle = r_ble_util_buf_acl_tx_alloc(len);
    if (handle == 0)
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    lld_tx_elem_t *elt = r_ble_util_buf_acl_tx_elt_get(handle);
    uint8_t *buf = (uint8_t *)r_plf_funcs_p->em_buf_get(handle);
    memcpy(buf, payload, len);
    memset(elt->_hdr, 0, sizeof(elt->_hdr));
    elt->length_flags = len;
    if (r_ip_funcs_p->lld_con_data_tx(link, elt) != 0) {
        r_ble_util_buf_acl_tx_free(handle);
        return HCI_ERR_COMMAND_DISALLOWED;
    }
    return HCI_SUCCESS;
}

/* Parse the LL PDU sub-header ([llid][length][payload]) and dispatch by LLID. */
static uint8_t inject_ll(uint16_t link, const uint8_t *pdu, uint16_t pdu_len)
{
    if (pdu_len < PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    uint8_t llid = pdu[PDU_INJECT_LL_OFF_LLID];
    uint8_t len = pdu[PDU_INJECT_LL_OFF_LENGTH];
    const uint8_t *payload = pdu + PDU_INJECT_LL_OFF_PAYLOAD;
    if (len > pdu_len - PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    if (!link_active(link))
        return HCI_ERR_COMMAND_DISALLOWED;
    switch (llid) {
    case PDU_INJECT_LLID_CONTROL:
        return inject_ll_control(link, payload, len);
    case PDU_INJECT_LLID_DATA_CONT:
    case PDU_INJECT_LLID_DATA_START:
        return inject_ll_data(link, payload, len);
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
}

uint8_t inject_pdu(const uint8_t *params, uint16_t length)
{
    if (length < PDU_INJECT_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    uint8_t kind = params[PDU_INJECT_OFF_KIND];
    uint16_t handle = (uint16_t)(params[PDU_INJECT_OFF_HANDLE] | (params[PDU_INJECT_OFF_HANDLE + 1] << 8));
    const uint8_t *pdu = params + PDU_INJECT_OFF_PDU;
    uint16_t pdu_len = (uint16_t)(length - PDU_INJECT_HDR_LEN);

    switch (kind) {
    case PDU_INJECT_KIND_LL:
        return inject_ll(handle, pdu, pdu_len);
    case PDU_INJECT_KIND_LMP:
        return HCI_ERR_UNSUPPORTED_FEATURE; /* S3 is BLE-only, no BR/EDR */
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
}
