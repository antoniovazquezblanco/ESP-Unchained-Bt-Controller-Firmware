# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# HCI transport
set(srcs_hci_transport "")
if(CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG)
    list(APPEND srcs_hci_transport "bluetooth/transport/hci_usb_serial_jtag.c")
elseif(CONFIG_UNCHAINED_HCI_OVER_UART)
    list(APPEND srcs_hci_transport "bluetooth/transport/hci_uart.c")
endif()
if(srcs_hci_transport)
    list(APPEND srcs_hci_transport "bluetooth/transport/vhci_bridge.c")
endif()

# UI
set(srcs_ui "")
if(CONFIG_UNCHAINED_DISPLAY_ENABLED)
    set(srcs_ui "ui/ui.c" "ui/display.c" "ui/console.c" "ui/splash.c" "ui/gfx.c" "ui/font.c")
endif()

idf_component_register(SRCS "main.c"
                            "bluetooth/bluetooth.c" "bluetooth/vsc.c"
                            "bluetooth/transport/transport.c" ${srcs_hci_transport} ${srcs_ui}
                       PRIV_REQUIRES bt nvs_flash esp_driver_uart esp_driver_usb_serial_jtag
                                     esp_lcd esp_driver_spi esp_driver_gpio esp_app_format
                       INCLUDE_DIRS "." "ui" "bluetooth"
                       REQUIRED_IDF_TARGETS esp32c5 esp32c3 esp32)
