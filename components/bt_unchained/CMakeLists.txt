# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Header-only: the target-agnostic unchained interface and wire definitions shared
# by every esp*_bt_unchained backend (and the app). REQUIRES bt_protocols so the
# shared headers can use the HCI opcode helpers.
idf_component_register(INCLUDE_DIRS "include"
                       REQUIRES bt_protocols)
