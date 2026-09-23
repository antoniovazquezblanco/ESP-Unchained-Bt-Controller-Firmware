/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Patches the ESP32 classic (BR/EDR) controller's RivieraWaves ROM dispatch
 * tables (exposed by esp32_bt_rom) to graft in our own functionality.
 */

#ifndef ESP32_BT_UNCHAINED_H
#define ESP32_BT_UNCHAINED_H

/*
 * Initialise the patch layer (ESP32 only). Call once, after
 * esp_bt_controller_enable().
 * This sets an identifiable company id and routes the whole vendor command
 * group (OGF 0x3F) to our own set, defined in vsc.h.
 */
void esp32_bt_unchained_init(void);

#endif /* ESP32_BT_UNCHAINED_H */
