# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Get board name
if(BOARD)
    set(_bt_unchained_esp32c5_board "${BOARD}")
else()
    set(_bt_unchained_esp32c5_board "unknown")
endif()

idf_component_register(SRCS "bt_unchained_esp32c5.c" "vsc.c" "traffic_monitor.c" "scan_pin.c"
                       PRIV_REQUIRES bt_unchained bt bt_protocols bt_rom_esp32c5 esp_app_format
                       REQUIRED_IDF_TARGETS esp32c5)

target_compile_definitions(${COMPONENT_LIB} PRIVATE "BT_UNCHAINED_ESP32C5_BOARD=\"${_bt_unchained_esp32c5_board}\"")

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
