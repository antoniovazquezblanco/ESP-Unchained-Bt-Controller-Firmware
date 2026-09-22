# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

# Board selection
include("${CMAKE_CURRENT_LIST_DIR}/cmake/board.cmake")

# Project
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
idf_build_set_property(MINIMAL_BUILD ON)
project(esp_unchained_bt_controller)

# Target helpers
include("${CMAKE_CURRENT_LIST_DIR}/cmake/espflash.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/release.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/logo.cmake")
