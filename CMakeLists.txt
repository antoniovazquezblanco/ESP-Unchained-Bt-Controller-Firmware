# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly.
cmake_minimum_required(VERSION 3.16)

# Board selection
# `idf.py -DBOARD=<name> ...` layers boards/<name>.defaults
if(DEFINED BOARD)
    set(_board_defaults "${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD}.defaults")
    if(NOT EXISTS "${_board_defaults}")
        message(FATAL_ERROR "Unknown BOARD '${BOARD}' (no ${_board_defaults})")
    endif()
    set(SDKCONFIG_DEFAULTS "${CMAKE_CURRENT_LIST_DIR}/sdkconfig.defaults;${_board_defaults}")
endif()

# The chips this firmware supports are declared with REQUIRED_IDF_TARGETS on the
# main component (main/CMakeLists.txt).

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp_unchained_bt_controller)
