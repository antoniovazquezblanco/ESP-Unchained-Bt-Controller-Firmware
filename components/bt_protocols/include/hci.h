/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Core specification protocol definitions.
 */

#ifndef HCI_H
#define HCI_H

/* HCI packet type: the first byte of every frame (Vol 4, Part A). */
#define HCI_PKT_CMD 0x01
#define HCI_PKT_ACL 0x02
#define HCI_PKT_SCO 0x03
#define HCI_PKT_ISO 0x05

/* Header sizes, not counting the packet type indicator. */
#define HCI_CMD_HDR_SIZE 3 /* opcode(2) + plen(1)        */
#define HCI_ACL_HDR_SIZE 4 /* handle(2) + len(2)         */
#define HCI_SCO_HDR_SIZE 3 /* handle(2) + len(1)         */
#define HCI_ISO_HDR_SIZE 4 /* handle(2) + len(2, 14 bit) */

/* The top two bits of an ISO length are reserved. */
#define HCI_ISO_LEN_MASK 0x3FFF

/* Command status / error codes (Vol 1, Part F). */
#define HCI_SUCCESS 0x00
#define HCI_ERR_INVALID_PARAMS 0x12

#endif /* HCI_H */
