/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor.
 */
#include "lmp_monitor.h"

#include <stddef.h>
#include <string.h>

#include "esp32_bt_rom.h"
#include "hci.h"

/* Which capture sources are live. Read in the LMP TX path (controller context)
 * and written from the command handler; a byte write is atomic on this core. */
static volatile uint8_t s_flags;

void lmp_monitor_set(uint8_t flags)
{
    s_flags = flags;
}

uint8_t lmp_monitor_get(void)
{
    return s_flags;
}

/* Emit one captured PDU as a vendor HCI event (see lmp_monitor.h for the layout).
 * The PDU is the concatenation head[0..head_len) + body[0..body_len), so an LL
 * capture can pass its reconstructed 2-byte header as head and the payload,
 * still in exchange memory, as body without staging a copy. Runs in controller
 * context, exactly like the ROM's own event emitters. */
static void monitor_emit(uint8_t direction, uint8_t link_id,
                         const uint8_t *head, uint8_t head_len,
                         const uint8_t *body, uint8_t body_len)
{
    uint16_t param_len = (uint16_t)(8 + head_len + body_len);
    uint8_t *p = r_modules_funcs_p->ke_msg_alloc(HCI_EVT_KE_ID, 0, HCI_EVT_VENDOR_SPECIFIC, param_len);
    if (p == NULL) {
        return;
    }
    uint32_t clock = r_ip_funcs_p->ld_read_clock();
    p[0] = LMP_MONITOR_EVT_SUBCODE;
    p[1] = direction;
    p[2] = link_id;
    p[3] = (uint8_t)clock;
    p[4] = (uint8_t)(clock >> 8);
    p[5] = (uint8_t)(clock >> 16);
    p[6] = (uint8_t)(clock >> 24);
    p[7] = (uint8_t)(head_len + body_len);
    memcpy(&p[8], head, head_len);
    if (body_len) {
        memcpy(&p[8 + head_len], body, body_len);
    }
    r_ip_funcs_p->hci_send_2_host_hack(p);
}

void lmp_monitor_on_lmp_tx(uint32_t link_id, const bt_em_lmp_buf_elt_t *buf_elt)
{
    if ((s_flags & LMP_MONITOR_LMP_TX) == 0 || buf_elt == NULL) {
        return;
    }
    const uint8_t *pdu = r_ip_funcs_p->em_buf_tx_buff_addr_get(buf_elt);
    monitor_emit(LMP_MONITOR_DIR_TX, (uint8_t)link_id, pdu, buf_elt->length, NULL, 0);
}

void lmp_monitor_on_lmp_rx(const uint8_t *pdu, uint8_t pdu_len)
{
    if ((s_flags & LMP_MONITOR_LMP_RX) == 0 || pdu == NULL || pdu_len == 0) {
        return;
    }
    /* lmp_unpack has no link id to give us, so RX reports the sentinel. */
    monitor_emit(LMP_MONITOR_DIR_RX, LMP_MONITOR_LINK_ID_UNKNOWN, pdu, pdu_len, NULL, 0);
}

/* A BLE LL data-channel PDU is a 2-byte header then payload. The header's low 2
 * bits are the LLID: 1 = data continuation/empty, 2 = data start, 3 = control.
 * LLID 0 is reserved and marks a descriptor we should not read as a PDU. */
#define LL_LLID_MASK 0x03

/* Emit a BLE LL PDU: rebuild its 2-byte header from the LLID/flags byte and the
 * length, then append the payload. Shared by the TX and RX taps. */
static void ll_emit(uint8_t direction, uint8_t hdr0, uint8_t length, const uint8_t *payload)
{
    if ((hdr0 & LL_LLID_MASK) == 0 || length == 0 || payload == NULL) {
        return;
    }
    const uint8_t header[2] = {hdr0, length};
    monitor_emit(direction, LMP_MONITOR_LINK_ID_UNKNOWN, header, sizeof(header), payload, length);
}

void lmp_monitor_on_ll_tx(int32_t tx_desc)
{
    if ((s_flags & LMP_MONITOR_LL_TX) == 0 || tx_desc == 0) {
        return;
    }
    const lld_tx_desc_t *desc = (const lld_tx_desc_t *)tx_desc;
    if (desc->length == 0) {
        return;
    }
    /* The payload sits in exchange memory at an offset the descriptor carries;
     * llid and length rebuild the 2-byte LL header the ROM will put on air. */
    const uint8_t *payload = (const uint8_t *)(LLD_EM_BASE + desc->buf_off);
    ll_emit(LMP_MONITOR_DIR_TX, desc->llid, desc->length, payload);
}

void lmp_monitor_on_ll_rx(uint8_t nb_rx)
{
    if ((s_flags & LMP_MONITOR_LL_RX) == 0) {
        return;
    }
    /* The ROM is about to drain nb_rx buffers starting at the current RX index,
     * advancing it (mod the ring size) per PDU. We read the same descriptors
     * first, before lld_pdu_rx_handler frees them. */
    uint8_t idx = LLD_RX_CURRENT_IDX;
    for (uint8_t i = 0; i < nb_rx; i++, idx = (idx + 1) & (LLD_RX_DESC_COUNT - 1)) {
        uint16_t hdr = lld_rx_desc[idx].hdr;
        uint8_t length = (uint8_t)(hdr >> 8);
        if (length == 0) {
            continue; /* empty PDU (keepalive); nothing to capture */
        }
        const uint8_t *payload = (const uint8_t *)r_ip_funcs_p->em_buf_rx_buff_addr_get(idx);
        ll_emit(LMP_MONITOR_DIR_RX, (uint8_t)hdr, length, payload);
    }
}

/* The event twin of s_vs_cmd_desc: no packer, params already laid out. */
static uint16_t evt_pack_in_place(uint8_t *out, uint8_t *in, uint16_t *out_len, uint16_t in_len)
{
    return 0;
}

static const hci_evt_desc_t s_monitor_evt_desc = {
    .code = HCI_EVT_VENDOR_SPECIFIC,
    .dest_field = 0,
    .special_pack = HCI_EVT_PK_SPE, /* par_fmt is a function, not a format string */
    .reserved = 0,
    .par_fmt = evt_pack_in_place,
};

hci_evt_desc_t *lmp_monitor_evt_desc(uint8_t evt_code)
{
    if (evt_code == HCI_EVT_VENDOR_SPECIFIC) {
        return (hci_evt_desc_t *)&s_monitor_evt_desc;
    }
    return NULL;
}
