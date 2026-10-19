/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the classic ESP32: transmit a crafted BR/EDR LMP PDU on an
 * owned link, the write twin of the LMP monitor. Driven by INJECT_PDU (0xFC05);
 * the wire contract is in pdu_inject.h.
 *
 * BLE LL injection is not yet implemented on the classic ESP32 (its LL TX path
 * needs an em_desc_node built for lld_pdu_data_tx_push, more reversing than the LMP
 * path); KIND_LL returns UNSUPPORTED_FEATURE for now. The C3/S3/C5 builds do LL.
 */

#ifndef INJECTOR_H
#define INJECTOR_H

#include <stdint.h>

/*
 * Inject one PDU described by the INJECT_PDU command parameters (pdu_inject.h:
 * [kind][handle:2][pdu...]). Returns the HCI status: SUCCESS, INVALID_PARAMS
 * (short/garbled), UNSUPPORTED_FEATURE (KIND_LL -- not yet on the classic),
 * MEM_CAPACITY_EXCEEDED (LMP pool empty) or COMMAND_DISALLOWED (no such active
 * link, or the controller refused the submit).
 */
uint8_t inject_pdu(const uint8_t *params, uint8_t length);

#endif /* INJECTOR_H */
