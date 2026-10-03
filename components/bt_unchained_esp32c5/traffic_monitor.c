/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Low-level link traffic monitor for the ESP32-C5.
 *
 * The NimBLE-lineage controller (libble_app) is statically linked and has no
 * writable dispatch table, so the taps are installed at link time with
 * --wrap (see CMakeLists.txt): every controller call to the wrapped function
 * lands in our __wrap_ version first, which captures the PDU and then tail-calls
 * the real __real_ one. Captures are emitted as 0xFF vendor HCI events in the
 * same layout as the classic ESP32 and C3 monitors.
 *
 *   RX: r_ble_ll_conn_rx_data_pdu(rxpdu, hdr) sees every received connection PDU
 *       (data and control); the on-air 2-byte header sits at the mbuf start.
 *   TX: r_ble_lll_conn_append_tx_buffer(conn, om, off, len, llid) is called once
 *       per on-air fragment with the LLID and length as explicit arguments.
 *
 * Both run where the controller already allocates event buffers and talks to the
 * host transport (the LL task / lower-layer context), so emitting from here is
 * safe -- the controller does the same from these very functions.
 */
#include "traffic_monitor.h"

#include <stddef.h>

#include "bt_rom_esp32c5.h"

/* Vendor-specific HCI event code. */
#define TM_EVT_CODE 0xFF

/* Cap on captured payload bytes. The controller's HCI event buffer holds a full
 * extended advertising report, so this stays well within the block; larger data
 * PDUs are truncated (control PDUs, the usual interest, are tiny). */
#define TM_MAX_PAYLOAD 240

/* Fixed capture-event overhead before the PDU: 8 header bytes + the 2-byte
 * reconstructed LL header. */
#define TM_PDU_OFFSET 10

/* Which capture sources are live. Written from the command handler, read in the
 * controller taps; a byte access is atomic on this core. */
static volatile uint8_t s_flags;

void traffic_monitor_set(uint8_t flags)
{
    s_flags = flags;
}

uint8_t traffic_monitor_get(void)
{
    return s_flags;
}

/*
 * Emit one captured PDU as a 0xFF vendor event. The payload is copied straight
 * out of the mbuf chain into the event buffer, so no staging buffer is needed.
 */
static void monitor_emit(uint8_t direction, uint8_t link_id, uint8_t llid,
                         const struct os_mbuf *om, int off, uint16_t pdu_len)
{
    uint8_t cap = pdu_len > TM_MAX_PAYLOAD ? TM_MAX_PAYLOAD : (uint8_t)pdu_len;
    uint8_t *p = r_ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT);
    if (p == NULL) {
        return;
    }
    if (cap != 0 && r_os_mbuf_copydata(om, off, cap, &p[2 + TM_PDU_OFFSET]) != 0) {
        r_ble_hci_trans_buf_free(p); /* chain shorter than claimed: drop cleanly */
        return;
    }
    uint32_t clock = r_ble_lll_timer_current_tick_get();
    p[0] = TM_EVT_CODE;
    p[1] = (uint8_t)(TM_PDU_OFFSET + cap);
    p[2] = TRAFFIC_MONITOR_EVT_SUBCODE;
    p[3] = direction;
    p[4] = link_id;
    p[5] = (uint8_t)clock;
    p[6] = (uint8_t)(clock >> 8);
    p[7] = (uint8_t)(clock >> 16);
    p[8] = (uint8_t)(clock >> 24);
    p[9] = (uint8_t)(2 + cap); /* PDU length: 2-byte header + payload */
    p[10] = llid;              /* reconstructed LL header byte 0 (LLID) */
    p[11] = cap;               /* reconstructed LL header byte 1 (length) */
    r_ble_ll_hci_event_send(p);
}

/*
 * RX tap: wraps the upper-LL receive path, which every incoming connection PDU
 * passes through. The mbuf starts with the on-air 2-byte header (LLID in the low
 * two bits of byte 0, length in byte 1) followed by the payload.
 */
extern void __real_r_ble_ll_conn_rx_data_pdu(struct os_mbuf *rxpdu, void *hdr);

void __wrap_r_ble_ll_conn_rx_data_pdu(struct os_mbuf *rxpdu, void *hdr)
{
    if ((s_flags & TRAFFIC_MONITOR_LL_RX) != 0 && rxpdu != NULL) {
        uint8_t h[2];
        if (r_os_mbuf_copydata(rxpdu, 0, 2, h) == 0 && h[1] != 0) {
            monitor_emit(TRAFFIC_MONITOR_DIR_RX, TRAFFIC_MONITOR_LINK_ID_UNKNOWN,
                         (uint8_t)(h[0] & 3), rxpdu, 2, h[1]);
        }
    }
    __real_r_ble_ll_conn_rx_data_pdu(rxpdu, hdr);
}

/*
 * TX tap: wraps the lower-LL function that appends each on-air fragment to the
 * transmit buffer. llid and len arrive as explicit arguments; the fragment bytes
 * live at offset `off` in the mbuf chain. Empty PDUs (keepalives) are skipped.
 */
extern int __real_r_ble_lll_conn_append_tx_buffer(void *conn, struct os_mbuf *om,
                                                  int off, unsigned int len, unsigned int llid);

int __wrap_r_ble_lll_conn_append_tx_buffer(void *conn, struct os_mbuf *om,
                                           int off, unsigned int len, unsigned int llid)
{
    if ((s_flags & TRAFFIC_MONITOR_LL_TX) != 0 && om != NULL && len != 0) {
        /* The connection handle sits at conn+8 (low byte is enough for a link id). */
        uint8_t link_id = conn != NULL ? *(const volatile uint8_t *)((const uint8_t *)conn + 8)
                                       : TRAFFIC_MONITOR_LINK_ID_UNKNOWN;
        monitor_emit(TRAFFIC_MONITOR_DIR_TX, link_id, (uint8_t)(llid & 3), om, off, (uint16_t)len);
    }
    return __real_r_ble_lll_conn_append_tx_buffer(conn, om, off, len, llid);
}
