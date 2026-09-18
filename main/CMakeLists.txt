# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# HCI transports
set(hci_transport_srcs "")
if(CONFIG_UNCHAINED_HCI_OVER_USB_SERIAL_JTAG)
    list(APPEND hci_transport_srcs "bluetooth/transport/hci_usb.c")
elseif(CONFIG_UNCHAINED_HCI_OVER_UART)
    list(APPEND hci_transport_srcs "bluetooth/transport/hci_uart.c")
endif()

idf_component_register(SRCS "main.c"
                            "ui/display.c" "ui/font.c" "ui/gfx.c" "ui/console.c" "ui/splash.c" "ui/ui.c"
                            "bluetooth/bluetooth.c" "bluetooth/vsc.c"
                            "bluetooth/transport/transport.c" ${hci_transport_srcs}
                       PRIV_REQUIRES bt nvs_flash esp_driver_uart esp_driver_usb_serial_jtag
                                     esp_lcd esp_driver_spi esp_driver_gpio esp_app_format
                       INCLUDE_DIRS "." "ui" "bluetooth"
                       REQUIRED_IDF_TARGETS esp32c5 esp32c3 esp32)

# Boot logo image: when enabled, generate the RGB565 header from the sprites in
# utils/logo at build time (needs Pillow in the build Python). It lands in the
# build tree -- never checked in -- and splash.c picks it up from there.
if(CONFIG_UNCHAINED_DISPLAY_LOGO)
    set(logo_dir "${CMAKE_CURRENT_SOURCE_DIR}/../utils/logo")
    set(logo_hdr "${CMAKE_CURRENT_BINARY_DIR}/logo_img.h")
    add_custom_command(
        OUTPUT "${logo_hdr}"
        COMMAND "${PYTHON}" "${logo_dir}/render_logo.py"
                320 240 --links 2 --format c -o "${logo_hdr}"
        DEPENDS "${logo_dir}/render_logo.py"
                "${logo_dir}/sprites/part1_left.png"
                "${logo_dir}/sprites/part2_mid.png"
                "${logo_dir}/sprites/part3_right.png"
                "${logo_dir}/sprites/part1_left_broken.png"
                "${logo_dir}/sprites/part3_right_broken.png"
                "${logo_dir}/sprites/rune.png"
        COMMENT "Generating boot logo header (logo_img.h)"
        VERBATIM)
    add_custom_target(unchained_logo_img DEPENDS "${logo_hdr}")
    add_dependencies(${COMPONENT_LIB} unchained_logo_img)
    target_include_directories(${COMPONENT_LIB} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
endif()
