# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_bt_unchained_esp32_board "${BOARD}")
else()
    set(_bt_unchained_esp32_board "unknown")
endif()

idf_component_register(SRCS "bt_unchained_esp32.c" "vsc.c" "lmp_monitor.c" "scan_pin.c"
                       PRIV_REQUIRES bt_unchained bt_protocols bt_rom_esp32 esp_app_format
                       REQUIRED_IDF_TARGETS esp32)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "BT_UNCHAINED_ESP32_BOARD=\"${_bt_unchained_esp32_board}\"")
