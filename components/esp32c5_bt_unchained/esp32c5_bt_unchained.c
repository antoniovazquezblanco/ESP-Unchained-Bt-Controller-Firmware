/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Unlocks the ESP32-C5 controller with our own vendor-specific commands.
 *
 * The custom vendor group (Company 0xF00D: INFO, SUPPORTED_CMDS, SET_BDADDR,
 * SET_TRAFFIC_MONITOR, SET_SCAN_CHANNEL) is registered straight into the
 * controller's vendor-command list (vsc.c), which needs only libble_app symbols.
 * The traffic monitor and scan-channel pin are installed at link time
 * (traffic_monitor.c / scan_pin.c -- see the CMakeLists --wrap options and the
 * forced-channel byte), so there is nothing to wire up here beyond registering the
 * commands and re-branding the company id.
 */
#include "bt_unchained.h"

#include "esp_log.h"

#include "esp32c5_bt_rom.h"
#include "vsc.h"

static const char *TAG = "UNCHAINED";

/* Identifiable company id stamped over the controller default (0x02E5). */
#define UNCHAINED_COMPID 0xF00D

void bt_unchained_init(void)
{
    vsc_register();
    if (esp32c5_bt_set_compid(UNCHAINED_COMPID))
        ESP_LOGI(TAG, "unchained vendor commands registered, company id 0x%04X", UNCHAINED_COMPID);
    else
        ESP_LOGW(TAG, "unchained vendor commands registered; company id not applied (controller cfg not ready)");
}
