/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Espressif vendor-specific (OGF 0x3F) HCI command enablement.
 */
#ifndef VSC_H
#define VSC_H

/*
 * Register Espressif's vendor-specific HCI commands/events with the controller.
 * Call after esp_bt_controller_enable(): the envs the enablers write into are
 * allocated by init()/enable().
 *
 * Not for the classic ESP32, which runs esp32_bt_unchained and serves its own
 * vendor command set instead. No-op with a warning on a chip without a verified
 * enable API, or on an ESP-IDF too old to expose it.
 */
void vsc_enable(void);

#endif /* VSC_H */
