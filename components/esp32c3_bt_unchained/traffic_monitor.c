/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C3.
 *
 * The controller hands each received connection PDU to lld_con_rx_llcp_check with
 * its LLID and length; the payload sits in an exchange-memory buffer the current
 * RX descriptor names. We read it there and emit a vendor event. The tap runs in
 * the radio RX ISR, but the ROM itself calls ke_msg_alloc/ke_msg_send from that
 * same path, so allocating and queuing an event here is safe.
 */
#include "traffic_monitor.h"

#include <stddef.h>
#include <string.h>

#include "esp32c3_bt_rom.h"
#include "hci.h"

/* Which capture sources are live. Written from the command handler, read in the
 * RX tap (ISR context); a byte access is atomic on this core. */
static volatile uint8_t s_flags;

void traffic_monitor_set(uint8_t flags)
{
    s_flags = flags;
}

uint8_t traffic_monitor_get(void)
{
    return s_flags;
}

/* Emit one captured PDU as a 0xFF vendor HCI event (see traffic_monitor.h for the
 * layout). The PDU is a reconstructed 2-byte LL header (llid, length) then the
 * payload still in exchange memory. */
static void monitor_emit(uint8_t direction, uint8_t link_id, uint8_t llid,
                         const uint8_t *payload, uint8_t length)
{
    uint8_t header[2] = {llid, length};
    uint16_t pdu_len = (uint16_t)(sizeof(header) + length);
    uint16_t param_len = (uint16_t)(8 + pdu_len);
    if (param_len > 0xFF) {
        return; /* will not fit a one-byte HCI event length */
    }
    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_EVT_KE_ID, 0, HCI_EVT_VENDOR_SPECIFIC, param_len);
    if (p == NULL) {
        return;
    }
    uint32_t clock = r_ip_funcs_p->lld_read_clock();
    p[0] = TRAFFIC_MONITOR_EVT_SUBCODE;
    p[1] = direction;
    p[2] = link_id;
    p[3] = (uint8_t)clock;
    p[4] = (uint8_t)(clock >> 8);
    p[5] = (uint8_t)(clock >> 16);
    p[6] = (uint8_t)(clock >> 24);
    p[7] = (uint8_t)pdu_len;
    memcpy(&p[8], header, sizeof(header));
    if (length) {
        memcpy(&p[8 + sizeof(header)], payload, length);
    }
    r_ip_funcs_p->hci_send_2_host(p);
}

void traffic_monitor_on_ll_rx(uint32_t link_id, uint32_t llid, uint16_t length)
{
    if ((s_flags & TRAFFIC_MONITOR_LL_RX) == 0 || length == 0) {
        return;
    }
    /* The descriptor being processed names the received PDU's buffer; map its
     * handle through exchange memory to the payload bytes. */
    uint8_t idx = p_lld_env->rx_desc_idx;
    em_rxdesc_t *rxdesc = (em_rxdesc_t *)r_plf_funcs_p->em_buf_get(EM_REGION_RXDESC) + idx;
    const uint8_t *payload = (const uint8_t *)r_plf_funcs_p->em_buf_get(rxdesc->buf_handle);
    monitor_emit(TRAFFIC_MONITOR_DIR_RX, (uint8_t)link_id, (uint8_t)llid, payload, (uint8_t)length);
}

void traffic_monitor_on_ll_tx(uint32_t link_id, const lld_tx_elem_t *tx_elem, uint8_t llid)
{
    if ((s_flags & TRAFFIC_MONITOR_LL_TX) == 0 || tx_elem == NULL) {
        return;
    }
    /* Tapped as the PDU is queued, before lld_con_tx_prog fragments it, so the
     * element still carries the whole PDU: its length and the buffer handle to
     * map through exchange memory. */
    uint16_t length = LLD_TX_ELEM_LEN(tx_elem);
    if (length == 0) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)r_plf_funcs_p->em_buf_get(tx_elem->buf_handle);
    monitor_emit(TRAFFIC_MONITOR_DIR_TX, (uint8_t)link_id, llid, payload, (uint8_t)length);
}

/* The event twin of the vendor command descriptor: no packer, params already
 * laid out, so packing is a no-op reporting success. */
static uint32_t evt_pack_in_place(uint8_t *out, uint8_t *in, uint16_t *out_len, uint16_t in_len)
{
    (void)out;
    (void)in;
    (void)out_len;
    (void)in_len;
    return 0;
}

static const esp32c3_hci_evt_desc_t s_monitor_evt_desc = {
    .code = HCI_EVT_VENDOR_SPECIFIC,
    .flags = 1, /* ret_fmt is a self-pack function */
    .ret_fmt = evt_pack_in_place,
};

esp32c3_hci_evt_desc_t *traffic_monitor_evt_desc(uint8_t evt_code)
{
    if (evt_code == HCI_EVT_VENDOR_SPECIFIC) {
        return (esp32c3_hci_evt_desc_t *)&s_monitor_evt_desc;
    }
    return NULL;
}
