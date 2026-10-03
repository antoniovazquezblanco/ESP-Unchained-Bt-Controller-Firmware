/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C5.
 */

#ifndef VSC_H
#define VSC_H

/*
 * We own the whole vendor group (OGF 0x3F). These opcodes match the classic
 * ESP32 build so one host tool drives every target; the C5 controller dispatches
 * vendor commands by OCF (opcode & 0x3ff) and packs the Command Complete itself,
 * so each handler just fills return parameters and returns a status.
 *
 * INFO                0xFC00  in: nothing; out: fw name, fw version, board name,
 *                             each a uint8 length followed by that many bytes.
 * SUPPORTED_CMDS      0xFC01  in: nothing; out: little-endian uint64, bit N set
 *                             when the command with OCF N is implemented.
 * SET_BDADDR          0xFC02  in: 6-byte BD_ADDR; out: nothing.
 * SET_TRAFFIC_MONITOR 0xFC03  in: 1 flag byte (TRAFFIC_MONITOR_*); out: nothing.
 * SET_SCAN_CHANNEL    0xFC04  in: 1 channel byte (0 = hop, else 37/38/39); out: none.
 */
#define UNCHAINED_VS_INFO_OCF 0x000                /* 0xFC00 */
#define UNCHAINED_VS_SUPPORTED_CMDS_OCF 0x001      /* 0xFC01 */
#define UNCHAINED_VS_SET_BDADDR_OCF 0x002          /* 0xFC02 */
#define UNCHAINED_VS_SET_TRAFFIC_MONITOR_OCF 0x003 /* 0xFC03 */
#define UNCHAINED_VS_SET_SCAN_CHANNEL_OCF 0x004    /* 0xFC04 */

/*
 * Install our vendor commands into the controller's VS dispatch list. Call once
 * after the controller is enabled (ble_ll_hci_env_p is valid by then).
 */
void vsc_register(void);

#endif /* VSC_H */
