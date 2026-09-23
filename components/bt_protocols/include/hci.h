/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth Core specification protocol definitions.
 */

#ifndef HCI_H
#define HCI_H

#include <stdint.h>

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

/* A command opcode packs a 6 bit group (OGF) and a 10 bit command (OCF). */
#define HCI_OPCODE(ogf, ocf) ((uint16_t)(((ogf) << 10) | ((ocf) & 0x03FF)))
#define HCI_OPCODE_OGF(opcode) (((opcode) >> 10) & 0x003F)
#define HCI_OPCODE_OCF(opcode) ((opcode) & 0x03FF)

/* Opcode groups (Vol 4, Part E 5.4.1). */
#define HCI_OGF_LINK_CTRL 0x01
#define HCI_OGF_LINK_POLICY 0x02
#define HCI_OGF_CTRL_BASEBAND 0x03
#define HCI_OGF_INFO_PARAMS 0x04
#define HCI_OGF_STATUS_PARAMS 0x05
#define HCI_OGF_TESTING 0x06
#define HCI_OGF_LE_CTRL 0x08
#define HCI_OGF_VENDOR 0x3F

/* Command status / error codes (Vol 1, Part F). */
#define HCI_SUCCESS 0x00
#define HCI_ERR_UNKNOWN_HCI_CMD 0x01
#define HCI_ERR_INVALID_PARAMS 0x12

#endif /* HCI_H */
