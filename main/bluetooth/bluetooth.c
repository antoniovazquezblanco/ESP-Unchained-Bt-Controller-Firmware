/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Bluetooth controller / HCI transport bring-up.
 */
#include "bluetooth.h"

#include "esp_log.h"
#include "esp_bt.h"

#include "hci_usb.h"
#include "hci_uart.h"
#include "vsc.h"

static const char *TAG = "BT";

/* Controller mode: dual (BR/EDR + BLE) where the chip and its config ask for it
 * (the classic ESP32), BLE-only otherwise. The BTDM_CTRL_MODE_* symbols exist
 * only on the classic ESP32; BLE-only chips (C3/C5) fall through to the default.
 * Memory for the mode we do NOT run is released. */
#if defined(CONFIG_BTDM_CTRL_MODE_BTDM)
#  define BT_MODE          ESP_BT_MODE_BTDM
#  define BT_MODE_UNUSED   0
#elif defined(CONFIG_BTDM_CTRL_MODE_BR_EDR_ONLY)
#  define BT_MODE          ESP_BT_MODE_CLASSIC_BT
#  define BT_MODE_UNUSED   ESP_BT_MODE_BLE
#else
#  define BT_MODE          ESP_BT_MODE_BLE
#  define BT_MODE_UNUSED   ESP_BT_MODE_CLASSIC_BT
#endif

esp_err_t bluetooth_init(void)
{
#if BT_MODE_UNUSED
    (void)esp_bt_controller_mem_release(BT_MODE_UNUSED);   /* free the unused mode's RAM */
#endif

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_bt_controller_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Vendor-specific (OGF 0x3F) commands that must be registered before enable. */
    vsc_enable_pre();

    ret = esp_bt_controller_enable(BT_MODE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_bt_controller_enable failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "HCI controller enabled (mode %d)", (int)BT_MODE);

    /* With VHCI the transport is ours to provide; each bridge is a no-op unless
     * its board selected it. */
    ret = hci_usb_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "hci_usb_init failed: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = hci_uart_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "hci_uart_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* ...and the vendor-specific commands that must be registered after enable. */
    vsc_enable_post();

    return ESP_OK;
}
