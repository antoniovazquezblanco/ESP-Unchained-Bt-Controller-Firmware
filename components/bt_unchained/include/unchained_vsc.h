/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set (Company 0xF00D, OGF 0x3F).
 *
 * These opcodes and their wire contracts are shared by every target, so one host
 * tool drives them all. Each target component implements the dispatch in its own
 * vsc.c -- the mechanism differs (the RivieraWaves ROM on the ESP32/C3, the NimBLE
 * command list on the C5) -- but the opcode numbers and semantics are defined here
 * once. Every command replies with a Command Complete whose first return byte is a
 * status; both the OCF (vendor-group index) and the full opcode are provided.
 */

#ifndef UNCHAINED_VSC_H
#define UNCHAINED_VSC_H

#include "hci.h"

/*
 * INFO: identify this build.
 *
 * in:  nothing
 * out: status, then firmware name, firmware version and board name, each a
 *      uint8 length followed by that many bytes.
 */
#define UNCHAINED_VS_INFO_OCF 0x000
#define UNCHAINED_VS_INFO_OPCODE HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_INFO_OCF) /* 0xFC00 */

/*
 * SUPPORTED_CMDS: report which vendor commands this build implements.
 *
 * in:  nothing
 * out: status, then a little-endian uint64 bitfield: bit N is set when the command
 *      with OCF N is implemented, so INFO is bit 0. Commands past OCF 63 are not
 *      representable and are reported as absent.
 */
#define UNCHAINED_VS_SUPPORTED_CMDS_OCF 0x001
#define UNCHAINED_VS_SUPPORTED_CMDS_OPCODE \
    HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SUPPORTED_CMDS_OCF) /* 0xFC01 */

/*
 * SET_BDADDR: override the controller public address.
 *
 * in:  6-byte BD_ADDR, least-significant byte first
 * out: status; INVALID_PARAMS if fewer than 6 bytes were given.
 */
#define UNCHAINED_VS_SET_BDADDR_OCF 0x002
#define UNCHAINED_VS_SET_BDADDR_OPCODE HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_BDADDR_OCF) /* 0xFC02 */

/*
 * SET_TRAFFIC_MONITOR: report low-level link traffic to the host as vendor events
 * (0xFF), see capture_event.h.
 *
 * in:  1-byte flag bitmask (TRAFFIC_MONITOR_* in capture_event.h); 0 disables all.
 * out: status; INVALID_PARAMS if no byte was given, UNSUPPORTED_FEATURE if a flag
 *      this target has no hook for is set.
 */
#define UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF 0x003
#define UNCHAINED_VS_SET_TRAFFIC_MONITOR_OPCODE \
    HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF) /* 0xFC03 */

/*
 * SET_SCAN_CHANNEL: pin advertising reception (scanning) to one primary channel
 * instead of hopping 37/38/39.
 *
 * in:  1-byte channel: 37, 38 or 39 to pin, 0 to restore the three-channel hop.
 * out: status; INVALID_PARAMS if no byte was given or it is not 0/37/38/39.
 */
#define UNCHAINED_VS_SET_SCAN_CHANNEL_OCF 0x004
#define UNCHAINED_VS_SET_SCAN_CHANNEL_OPCODE \
    HCI_OPCODE(HCI_OGF_VENDOR, UNCHAINED_VS_SET_SCAN_CHANNEL_OCF) /* 0xFC04 */

#endif /* UNCHAINED_VSC_H */
