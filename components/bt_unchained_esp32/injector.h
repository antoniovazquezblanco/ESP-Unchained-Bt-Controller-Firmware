/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the classic ESP32: transmit a crafted BR/EDR LMP PDU or BLE
 * LL PDU (data or control) on an owned link, the write twin of the traffic
 * monitor. Driven by INJECT_PDU (0xFC05); the wire contract is in pdu_inject.h.
 */

#ifndef INJECTOR_H
#define INJECTOR_H

#include <stdint.h>

/*
 * Inject one PDU described by the INJECT_PDU command parameters (pdu_inject.h:
 * [kind][handle:2][pdu...]). Returns the HCI status: SUCCESS, INVALID_PARAMS
 * (short/garbled, or an LL payload over the single-PDU max), MEM_CAPACITY_EXCEEDED
 * (TX pool empty) or COMMAND_DISALLOWED (no such active link, or the controller
 * refused the submit).
 */
uint8_t inject_pdu(const uint8_t *params, uint8_t length);

#endif /* INJECTOR_H */
