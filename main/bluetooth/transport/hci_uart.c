/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * HCI over a hardware UART transport.
 * This requires an external UART to USB on a board to connect to a PC.
 */

#include "hci_uart.h"

#include "sdkconfig.h"
#include "driver/uart.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "vhci_bridge.h"

/*
 * HCI owns the UART.
 * Anything else writing there causes faults and desynchronises the stream.
 * Check that the configuration is correct and nothing else writes to this
 * UART.
 */
#if CONFIG_ESP_CONSOLE_UART && (CONFIG_ESP_CONSOLE_UART_NUM == CONFIG_UNCHAINED_HCI_UART_NUM)
#error "HCI is bridged to a UART that also carries the console. Set the console elsewhere (CONFIG_ESP_CONSOLE_NONE), or use a different HCI transport."
#endif

static const char *TAG = "HCIUART";

#define HCI_UART_PORT   (CONFIG_UNCHAINED_HCI_UART_NUM)
#define UART_RX_BUF     1024

static int hci_uart_read(uint8_t *buf, size_t len)
{
    /* uart_read_bytes() waits for the full length, so ask for one byte to block
     * on and then drain whatever else is already buffered. That hands a packet
     * over with minimal latency and still satisfies the bridge contract. */
    int n = uart_read_bytes(HCI_UART_PORT, buf, 1, portMAX_DELAY);
    if (n <= 0) {
        return n;
    }
    int more = uart_read_bytes(HCI_UART_PORT, buf + n, len - n, 0);
    return (more > 0) ? n + more : n;
}

static int hci_uart_write(const uint8_t *buf, size_t len)
{
    return uart_write_bytes(HCI_UART_PORT, (const char *)buf, len);
}

static const vhci_bridge_io_t s_io = {
    .name  = "UART",
    .read  = hci_uart_read,
    .write = hci_uart_write,
};

esp_err_t hci_uart_init(void)
{
    uart_config_t uart_cfg = {
        .baud_rate  = CONFIG_UNCHAINED_HCI_UART_BAUD,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_RETURN_ON_ERROR(uart_driver_install(HCI_UART_PORT, UART_RX_BUF, 0, 0, NULL, 0),
                        TAG, "uart driver");
    ESP_RETURN_ON_ERROR(uart_param_config(HCI_UART_PORT, &uart_cfg), TAG, "uart config");
    ESP_RETURN_ON_ERROR(uart_set_pin(HCI_UART_PORT, CONFIG_UNCHAINED_HCI_UART_TX_GPIO,
                                     CONFIG_UNCHAINED_HCI_UART_RX_GPIO,
                                     UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE), TAG, "uart pins");

    ESP_LOGI(TAG, "UART%d @ %d baud", HCI_UART_PORT, CONFIG_UNCHAINED_HCI_UART_BAUD);
    return vhci_bridge_start(&s_io);
}
