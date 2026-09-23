# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

idf_component_register(SRCS "esp32_bt_rom.c"
                       INCLUDE_DIRS "include"
                       REQUIRES bt_protocols
                       REQUIRED_IDF_TARGETS esp32)

target_linker_script(${COMPONENT_LIB} INTERFACE "esp32_bt_rom.ld")
