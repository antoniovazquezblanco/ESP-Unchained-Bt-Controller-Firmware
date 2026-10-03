# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

idf_component_register(SRCS "bt_rom_esp32c5.c"
                       INCLUDE_DIRS "include"
                       REQUIRED_IDF_TARGETS esp32c5)
