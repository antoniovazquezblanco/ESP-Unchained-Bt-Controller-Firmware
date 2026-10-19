/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * PDU injection for the ESP32-C5: transmit a crafted LL PDU on an owned link, the
 * write twin of the traffic monitor. Driven by INJECT_PDU (0xFC05); the wire
 * contract is in pdu_inject.h.
 */

#ifndef INJECTOR_H
#define INJECTOR_H

#include <stdint.h>

/*
 * Inject one PDU described by the INJECT_PDU command parameters (pdu_inject.h:
 * [kind][handle:2][pdu...]). Returns the HCI status: SUCCESS, INVALID_PARAMS
 * (short/garbled), UNSUPPORTED_FEATURE (LMP -- the C5 is BLE-only),
 * MEM_CAPACITY_EXCEEDED (mbuf pool empty) or COMMAND_DISALLOWED (no such active
 * connection, or the controller refused the submit).
 */
uint8_t inject_pdu(const uint8_t *params, uint8_t length);

#endif /* INJECTOR_H */
