/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 classic (BR/EDR) Bluetooth controller ROM glue.
 */

#ifndef ESP32_BT_ROM_H
#define ESP32_BT_ROM_H

#include <stdbool.h>
#include <stdint.h>

#include "r_ip_funcs.h"
#include "r_modules_funcs.h"

/*
 * ke_msg id the ROM uses to allocate an HCI Command Complete event. Pass it to
 * ke_msg_alloc() with src_id = the command opcode, fill byte [0] = status followed
 * by the return parameters, then hand the buffer to hci_send_2_host(). Seen in
 * hci_rd_local_ver_info_cmd_handler and hci_dbg_set_bd_addr_cmd_handler.
 */
#define HCI_CC_EVT_KE_ID 0x801

/**
 * Local controller Bluetooth SIG Company Identifier (CompId), default 0x0060.
 *
 * A ROM-data constant (never written by the controller) reported in the HCI
 * Read_Local_Version_Information event and in the LMP/LL version exchange, so
 * writing it re-brands the controller both to the host and over the air. Provided
 * as an absolute symbol by esp32_bt_rom.ld; assign a new value once after
 * esp_bt_controller_enable() to fingerprint a ROM-modified controller.
 */
extern volatile uint16_t co_default_compid;

/**
 * A self-test function for the ESP32 Bluetooth ROM functions.
 */
bool esp32_bt_rom_selftest(void);

#endif /* ESP32_BT_ROM_H */
