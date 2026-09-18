/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI transport setup.
 */

#include "transport.h"

#include "sdkconfig.h"
#include "esp_log.h"

#if CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG
#include "hci_usb.h"
#elif CONFIG_UNCHAINED_HCI_OVER_UART
#include "hci_uart.h"
#endif

static const char *TAG = "TL";

esp_err_t transport_init(void)
{
    ESP_LOGI(TAG, "Initializing Bluetooth transport");

#if CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG
    return hci_usb_init();
#elif CONFIG_UNCHAINED_HCI_OVER_UART
    return hci_uart_init();
#else
    #pragma message("No HCI transport selected!")
    ESP_LOGW(TAG, "No HCI transport selected!");
    return ESP_OK;
#endif
}
