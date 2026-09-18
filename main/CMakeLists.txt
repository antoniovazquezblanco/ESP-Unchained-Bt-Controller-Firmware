# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# HCI transports
set(hci_transport_srcs "")
if(CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG)
    list(APPEND hci_transport_srcs "bluetooth/transport/hci_usb_serial_jtag.c")
elseif(CONFIG_UNCHAINED_HCI_OVER_UART)
    list(APPEND hci_transport_srcs "bluetooth/transport/hci_uart.c")
endif()
if(hci_transport_srcs)
    list(APPEND hci_transport_srcs "bluetooth/transport/vhci_bridge.c")
endif()

idf_component_register(SRCS "main.c"
                            "ui/display.c" "ui/font.c" "ui/gfx.c" "ui/console.c" "ui/splash.c" "ui/ui.c"
                            "bluetooth/bluetooth.c" "bluetooth/vsc.c"
                            "bluetooth/transport/transport.c" ${hci_transport_srcs}
                       PRIV_REQUIRES bt nvs_flash esp_driver_uart esp_driver_usb_serial_jtag
                                     esp_lcd esp_driver_spi esp_driver_gpio esp_app_format
                       INCLUDE_DIRS "." "ui" "bluetooth"
                       REQUIRED_IDF_TARGETS esp32c5 esp32c3 esp32)
