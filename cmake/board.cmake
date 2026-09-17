# Board selection helper
#
# If BOARD variable is defined, the helper looks for boards/<name>.defaults
# and layers it to the SDK defaults.
#
# `idf.py -DBOARD=<name> ...` layers boards/<name>.defaults

if(DEFINED BOARD)
    set(_board_defaults "${CMAKE_SOURCE_DIR}/boards/${BOARD}.defaults")
    if(NOT EXISTS "${_board_defaults}")
        message(FATAL_ERROR "Unknown BOARD '${BOARD}' (no ${_board_defaults})")
    endif()
    set(SDKCONFIG_DEFAULTS "${CMAKE_SOURCE_DIR}/sdkconfig.defaults;${_board_defaults}")
endif()
