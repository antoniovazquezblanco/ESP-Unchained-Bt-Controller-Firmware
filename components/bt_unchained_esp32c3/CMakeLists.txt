# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_bt_unchained_esp32c3_board "${BOARD}")
else()
    set(_bt_unchained_esp32c3_board "unknown")
endif()

idf_component_register(SRCS "bt_unchained_esp32c3.c" "vsc.c" "traffic_monitor.c" "scan_pin.c"
                       PRIV_REQUIRES bt_unchained bt bt_protocols bt_rom_esp32c3 esp_app_format
                       REQUIRED_IDF_TARGETS esp32c3)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "BT_UNCHAINED_ESP32C3_BOARD=\"${_bt_unchained_esp32c3_board}\"")
