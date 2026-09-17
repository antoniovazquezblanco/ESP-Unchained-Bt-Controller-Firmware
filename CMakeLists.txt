# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly.
cmake_minimum_required(VERSION 3.16)

# Board selection
include("${CMAKE_CURRENT_LIST_DIR}/cmake/board.cmake")

# Project
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp_unchained_bt_controller)

# Target helpers
include("${CMAKE_CURRENT_LIST_DIR}/cmake/espflash.cmake")
