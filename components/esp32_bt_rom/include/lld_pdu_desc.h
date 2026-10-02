/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 BLE lower-link-driver (LLD) PDU descriptors, the exchange-memory
 * structures r_lld_pdu_data_tx_push and r_lld_pdu_rx_handler walk.
 */

#ifndef LLD_PDU_DESC_H
#define LLD_PDU_DESC_H

#include <stddef.h>
#include <stdint.h>

#include "bt_em_buf.h"

/*
 * One entry of the BLE RX descriptor ring. r_lld_pdu_rx_handler reads `hdr` for
 * the LL header and routes the PDU; r_em_buf_rx_buff_addr_get(idx) resolves
 * `buf_ptr` to the payload's CPU address. Ring is LLD_RX_DESC_COUNT entries,
 * indexed by the controller's rolling RX buffer index.
 */
typedef struct
{
    uint16_t hdr;      /**< +0: LL data-channel header; bits 0-1 LLID, high byte = payload length. */
    uint16_t reserved; /**< +2: driver state. */
    uint16_t buf_ptr;  /**< +4: EM offset of the payload (what em_buf_rx_buff_addr_get reads). */
    uint16_t pad[3];   /**< +6: driver state. */
} lld_rx_desc_t;
_Static_assert(sizeof(lld_rx_desc_t) == 12, "the ROM walks these with a 12 byte stride");

/** RX ring length; r_lld_pdu_rx_handler wraps the buffer index with extui ...,0,3. */
#define LLD_RX_DESC_COUNT 8

/** The RX descriptor ring; provided as an absolute ROM-data symbol by esp32_bt_rom.ld. */
extern volatile lld_rx_desc_t lld_rx_desc[LLD_RX_DESC_COUNT];

/*
 * Controller exchange-memory environment, provided by ESP-IDF's esp32.rom.ld.
 * Only one field matters here: the index of the next RX buffer the controller
 * will fill, which r_lld_pdu_rx_handler reads at +0x5c8 and advances per PDU.
 */
extern uint8_t em_buf_env[];
#define LLD_RX_CURRENT_IDX (em_buf_env[0x5c8])

/* Base of the controller's exchange-memory window; an EM offset plus this is a
 * CPU address, as r_em_buf_rx_buff_addr_get / r_em_buf_tx_buff_addr_get apply. */
#define LLD_EM_BASE 0x3ffb0000u

/*
 * One outgoing BLE LL PDU descriptor, as handed to r_lld_pdu_data_tx_push and
 * built by r_lld_pdu_data_send. The payload is in EM at LLD_EM_BASE + buf_off;
 * llid and length are the on-air LL header. Offsets confirmed on-chip against a
 * live connection: a data PDU descriptor had buf_off pointing at the L2CAP
 * bytes, llid 2, and length matching them.
 */
typedef struct
{
    uint8_t pad0[4];  /**< +0: list chaining while queued for TX. */
    uint8_t buf_idx;  /**< +4: EM TX buffer index. */
    uint8_t pad5[3];  /**< +5: driver state. */
    uint16_t buf_off; /**< +8: EM offset of the payload. */
    uint8_t llid;     /**< +10: LL header byte 0; bits 0-1 LLID. */
    uint8_t length;   /**< +11: payload length in bytes. */
} lld_tx_desc_t;
_Static_assert(offsetof(lld_tx_desc_t, buf_off) == 8, "buf_off must be at +8");
_Static_assert(offsetof(lld_tx_desc_t, llid) == 10, "llid must be at +10");
_Static_assert(offsetof(lld_tx_desc_t, length) == 11, "length must be at +11");

#endif /* LLD_PDU_DESC_H */
