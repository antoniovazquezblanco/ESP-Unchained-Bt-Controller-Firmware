/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP Unchained Bluetooth Controller Firmware
 *
 * Entry file.
 */

#include <stdio.h>

#include "esp_err.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "bluetooth.h"
#if CONFIG_UNCHAINED_DISPLAY_ENABLED
#include "ui.h"
#endif

void app_main(void)
{
#if CONFIG_UNCHAINED_DISPLAY_ENABLED
    /* UI initialization */
    ui_init();
#endif

    /* NVS initialization */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Bluetooth initialization */
    if (bluetooth_init() != ESP_OK) {
        return;
    }
}
