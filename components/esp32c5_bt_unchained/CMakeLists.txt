# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_esp32c5_bt_unchained_board "${BOARD}")
else()
    set(_esp32c5_bt_unchained_board "unknown")
endif()

idf_component_register(SRCS "esp32c5_bt_unchained.c" "vsc.c" "traffic_monitor.c" "scan_pin.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES bt bt_protocols esp32c5_bt_rom esp_app_format
                       REQUIRED_IDF_TARGETS esp32c5)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "ESP32C5_BT_UNCHAINED_BOARD=\"${_esp32c5_bt_unchained_board}\"")

# The controller (libble_app.a) is statically linked with no writable dispatch
# table, so the traffic monitor taps it at link time: --wrap routes every call to
# these controller functions through our __wrap_ versions, which capture and then
# tail-call the real __real_ ones (traffic_monitor.c). Both are called across
# object boundaries inside the library, which is what --wrap needs; the scan pin
# instead writes a forced-channel byte the controller already honours, so it needs
# no wrap. The symbols were verified with nm over libble_app.a.
target_link_libraries(${COMPONENT_LIB} INTERFACE
                      "-Wl,--wrap=r_ble_ll_conn_rx_data_pdu"
                      "-Wl,--wrap=r_ble_lll_conn_append_tx_buffer")
