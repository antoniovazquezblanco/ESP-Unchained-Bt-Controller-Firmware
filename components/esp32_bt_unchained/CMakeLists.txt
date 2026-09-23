# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_esp32_bt_unchained_board "${BOARD}")
else()
    set(_esp32_bt_unchained_board "unknown")
endif()

idf_component_register(SRCS "esp32_bt_unchained.c" "vsc.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES bt_protocols esp32_bt_rom esp_app_format
                       REQUIRED_IDF_TARGETS esp32)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "ESP32_BT_UNCHAINED_BOARD=\"${_esp32_bt_unchained_board}\"")
