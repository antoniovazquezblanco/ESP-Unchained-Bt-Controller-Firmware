# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# espflash helper
#
# Helpers to download and make espflash available to generate release files
# that include a flashing tool.

# Target: espflash downloader
add_custom_target(espflash-download
    COMMAND "${PYTHON}" "${CMAKE_CURRENT_SOURCE_DIR}/utils/download_espflash.py"
            --output-dir "${CMAKE_BINARY_DIR}/espflash"
    COMMENT "Downloading espflash binaries into ${CMAKE_BINARY_DIR}/espflash"
    USES_TERMINAL
    VERBATIM)
