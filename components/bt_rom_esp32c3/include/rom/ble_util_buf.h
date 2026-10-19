/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 controller TX buffer pools (r_ble_util_buf_*, absolute symbols from
 * esp32c3.rom.bt_funcs.ld). These allocate the exchange-memory buffer a crafted
 * PDU is written into before it is queued with lld_con_llcp_tx / lld_con_data_tx
 * (rom/ip_funcs.h). The free twins (acl_tx_free_in_isr / llcp_tx_free) are what
 * the ROM's own TX-completion path calls, so a buffer we allocate here is freed by
 * the controller after air time -- we only free it ourselves if we abort before a
 * successful submit.
 *
 * Signatures and the LLCP/ACL asymmetry below are from the rev3 ROM disassembly
 * (r_ble_util_buf_llcp_tx_alloc @ 0x40002f88, r_ble_util_buf_acl_tx_alloc @
 * 0x40003322, their free/elt_get twins). The TX element they yield is the
 * lld_tx_elem_t of rom/lld.h: buf_handle @ +4, length_flags @ +6, the same fields
 * the traffic monitor already reads (hardware-validated) from the queued element.
 */

#ifndef ESP32C3_ROM_BLE_UTIL_BUF_H
#define ESP32C3_ROM_BLE_UTIL_BUF_H

#include <stdint.h>

#include "rom/lld.h"

/* Largest LLCP payload an llcp-pool buffer holds (its EM buffer stride is 27). */
#define BLE_UTIL_BUF_LLCP_MAX 27
/* Largest data payload we inject: the max LL data PDU payload (DLE). The ROM
 * acl_tx_alloc itself rejects len > 256. */
#define BLE_UTIL_BUF_ACL_MAX 251

/*
 * Allocate an LLCP (control) TX element. Returns the lld_tx_elem_t directly, with
 * buf_handle already naming a fresh 27-byte EM payload buffer, or NULL when the
 * LLCP pool is empty. Free with its buf_handle via r_ble_util_buf_llcp_tx_free.
 */
lld_tx_elem_t *r_ble_util_buf_llcp_tx_alloc(void);

/* Free an LLCP TX buffer by its buf_handle (lld_tx_elem_t.buf_handle). */
void r_ble_util_buf_llcp_tx_free(uint16_t buf_handle);

/*
 * Allocate a data (ACL) TX buffer for a payload of len bytes (len <= 256). Returns
 * a buffer handle (0 when the pool is empty), not the element: map it to its
 * lld_tx_elem_t with r_ble_util_buf_acl_tx_elt_get and to the EM payload with
 * em_buf_get. Free the handle with r_ble_util_buf_acl_tx_free.
 */
uint16_t r_ble_util_buf_acl_tx_alloc(uint16_t len);

/* Map a data-TX buffer handle to its lld_tx_elem_t. */
lld_tx_elem_t *r_ble_util_buf_acl_tx_elt_get(uint16_t buf_handle);

/* Free a data (ACL) TX buffer by its handle. */
void r_ble_util_buf_acl_tx_free(uint16_t buf_handle);

/*
 * Per-link connection-environment pointers (lld_con_env @ 0x3fcdfc9c, .bss). The
 * slot for an inactive link id is NULL; lld_con_llcp_tx / lld_con_data_tx look the
 * target link up here and return an error when it is NULL. The array holds up to
 * ESP32C3_LLD_CON_MAX links.
 */
#define ESP32C3_LLD_CON_MAX 10
extern void *lld_con_env[ESP32C3_LLD_CON_MAX];

#endif /* ESP32C3_ROM_BLE_UTIL_BUF_H */
