# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_esp32c3_bt_unchained_board "${BOARD}")
else()
    set(_esp32c3_bt_unchained_board "unknown")
endif()

idf_component_register(SRCS "esp32c3_bt_unchained.c" "vsc.c" "traffic_monitor.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES bt bt_protocols esp32c3_bt_rom esp_app_format
                       REQUIRED_IDF_TARGETS esp32c3)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "ESP32C3_BT_UNCHAINED_BOARD=\"${_esp32c3_bt_unchained_board}\"")
