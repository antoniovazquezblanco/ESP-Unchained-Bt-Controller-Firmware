/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * The traffic-monitor wire protocol, shared by every target.
 *
 * SET_TRAFFIC_MONITOR (0xFC03) takes a bitmask of capture sources, and each
 * captured PDU is reported as a 0xFF vendor event in the layout below. One host
 * parser decodes every target, so the values live here once; each target's monitor
 * enables the subset of sources it has a hook for (its *_MONITOR_SUPPORTED mask).
 */

#ifndef CAPTURE_EVENT_H
#define CAPTURE_EVENT_H

/* SET_TRAFFIC_MONITOR flag bits (the 0xFC03 parameter byte). Combine with OR;
 * 0 disables every source. */
#define TRAFFIC_MONITOR_LMP_TX 0x01 /* outgoing BR/EDR LMP PDUs              */
#define TRAFFIC_MONITOR_LMP_RX 0x02 /* incoming BR/EDR LMP PDUs              */
#define TRAFFIC_MONITOR_LL_TX 0x04  /* outgoing BLE LL PDUs                  */
#define TRAFFIC_MONITOR_LL_RX 0x08  /* incoming BLE LL PDUs                  */

/*
 * Capture event, carried in the 0xFF vendor event (HCI_EVT_VENDOR_SPECIFIC) after
 * the event code and length:
 *
 *   [0]    subcode = TRAFFIC_MONITOR_EVT_SUBCODE, namespaces this under 0xFF
 *   [1]    direction: TRAFFIC_MONITOR_DIR_TX / _DIR_RX
 *   [2]    link id, or TRAFFIC_MONITOR_LINK_ID_UNKNOWN when the tap has none
 *   [3..6] controller clock at capture, little-endian uint32
 *   [7]    PDU length
 *   [8..]  the PDU: BR/EDR LMP is the raw bytes (opcode first); BLE LL is a
 *          reconstructed 2-byte header (LLID, length) then the payload.
 */
#define TRAFFIC_MONITOR_EVT_SUBCODE 0x01
#define TRAFFIC_MONITOR_DIR_TX 0x00
#define TRAFFIC_MONITOR_DIR_RX 0x01
#define TRAFFIC_MONITOR_LINK_ID_UNKNOWN 0xFF

#endif /* CAPTURE_EVENT_H */
