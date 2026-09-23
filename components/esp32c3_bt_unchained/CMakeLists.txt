# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

idf_component_register(SRCS "esp32c3_bt_unchained.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES bt esp32c3_bt_rom
                       REQUIRED_IDF_TARGETS esp32c3)
