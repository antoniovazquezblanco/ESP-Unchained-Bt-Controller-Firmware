/*
 * BLE controller / HCI transport bring-up.
 */
#include "bluetooth.h"

#include "esp_log.h"
#include "esp_bt.h"

#include "hci_usb.h"
#include "vsc.h"

static const char *TAG = "BT";

esp_err_t bluetooth_init(void)
{
    // No classic BT on this chip. release unused memory...
    (void)esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_bt_controller_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_bt_controller_enable failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "BLE HCI controller enabled");

    /* With VHCI the transport is ours to provide; bridge it to USB. A no-op
       when the controller drives its own HCI UART instead. */
    ret = hci_usb_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "hci_usb_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Unlock the vendor-specific (OGF 0x3F). */
    vsc_enable();

    return ESP_OK;
}
