/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the ESP32-C5 (NimBLE-lineage libble_app controller).
 *
 * The inverse of the traffic monitor: instead of tapping the PDU the controller
 * enqueues, we allocate an os_mbuf, write the crafted bytes into it, and call the
 * controller's own r_ble_ll_conn_enqueue_pkt -- the single primitive that queues
 * both data (LLID 1/2) and control/LLCP (LLID 3). It takes ownership of the mbuf
 * in every case (frees it on failure too), so we free it ourselves only before the
 * enqueue call. Runs in the LL-task context the vendor-command handler is called
 * in -- the context the controller's own senders use.
 *
 * The allocate->write->enqueue sequence mirrors the controller's genuine LLCP
 * senders (e.g. ble_ll_ctrl_start_enc_send); see bt_rom_esp32c5/include/ble_ll.h.
 * Not yet validated on hardware.
 */
#include "injector.h"

#include <stddef.h>

#include "ble_ll.h"
#include "hci.h"
#include "pdu_inject.h"

/* Inject one BLE LL PDU (data or LLCP) on the connection for handle. */
static uint8_t inject_ll(uint16_t handle, const uint8_t *pdu, uint16_t pdu_len)
{
    if (pdu_len < PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    uint8_t llid = pdu[PDU_INJECT_LL_OFF_LLID];
    uint8_t len = pdu[PDU_INJECT_LL_OFF_LENGTH];
    const uint8_t *payload = pdu + PDU_INJECT_LL_OFF_PAYLOAD;
    if (len == 0 || len > pdu_len - PDU_INJECT_LL_HDR_LEN)
        return HCI_ERR_INVALID_PARAMS;
    switch (llid) {
    case PDU_INJECT_LLID_CONTROL:
        /* The opcode (first payload byte) must be in range or enqueue_pkt asserts
         * and resets the controller; cap the length to a single control PDU. */
        if (len > BLE_LL_LLCP_PDU_MAX || payload[0] > BLE_LL_LLCP_OPCODE_MAX)
            return HCI_ERR_INVALID_PARAMS;
        break;
    case PDU_INJECT_LLID_DATA_CONT:
    case PDU_INJECT_LLID_DATA_START:
        if (len > BLE_LL_ACL_PDU_MAX)
            return HCI_ERR_INVALID_PARAMS;
        break;
    default:
        return HCI_ERR_INVALID_PARAMS;
    }

    struct ble_ll_conn_sm *connsm = r_ble_ll_conn_sm_find_active_by_handle(handle);
    if (connsm == NULL)
        return HCI_ERR_COMMAND_DISALLOWED;

    struct os_mbuf *om = r_ble_ll_mem_mbuf_get(len, BLE_LL_MBUF_HDR_LEN, BLE_LL_MBUF_LEADINGSPACE);
    if (om == NULL)
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    if (r_os_mbuf_append(om, payload, len) != 0) {
        r_os_mbuf_free_chain(om); /* still ours: enqueue has not taken it */
        return HCI_ERR_MEM_CAPACITY_EXCEEDED;
    }
    /* enqueue_pkt owns om from here, success or failure -- never free it below. */
    if (r_ble_ll_conn_enqueue_pkt(connsm, om, llid, len) != 0)
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
    uint16_t pdu_len = (uint16_t)(length - PDU_INJECT_HDR_LEN);

    switch (kind) {
    case PDU_INJECT_KIND_LL:
        return inject_ll(handle, pdu, pdu_len);
    case PDU_INJECT_KIND_LMP:
        return HCI_ERR_UNSUPPORTED_FEATURE; /* C5 is BLE-only, no BR/EDR */
    default:
        return HCI_ERR_INVALID_PARAMS;
    }
}
