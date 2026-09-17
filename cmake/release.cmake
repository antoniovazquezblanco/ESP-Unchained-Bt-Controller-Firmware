# Release tooling

## Variables

# Version
idf_build_get_property(_dist_ver PROJECT_VER)
if(NOT _dist_ver)
    set(_dist_ver "unknown")
endif()

# Board
if(DEFINED BOARD)
    set(_dist_board "${BOARD}")
else()
    set(_dist_board "${IDF_TARGET}")
endif()

# Name
set(_dist_name "esp-unchained-${_dist_board}-${_dist_ver}")

# Distribution directory and staging folder
set(_dist_dir "${CMAKE_BINARY_DIR}/dist")
set(_dist_stage "${_dist_dir}/${_dist_name}")

# Source flasher dir
set(_dist_flasher_src "${CMAKE_CURRENT_SOURCE_DIR}/utils/flasher")


## Configuration

# Flasher files
configure_file("${_dist_flasher_src}/flash.bat"  "${CMAKE_BINARY_DIR}/flash.bat"  @ONLY NEWLINE_STYLE CRLF)
configure_file("${_dist_flasher_src}/README.txt" "${CMAKE_BINARY_DIR}/README.txt" @ONLY NEWLINE_STYLE CRLF)
configure_file("${_dist_flasher_src}/flash.sh"   "${CMAKE_BINARY_DIR}/flash.sh"   @ONLY NEWLINE_STYLE UNIX)
if(NOT CMAKE_HOST_WIN32)
    file(CHMOD "${CMAKE_BINARY_DIR}/flash.sh"
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                         GROUP_READ GROUP_EXECUTE
                         WORLD_READ WORLD_EXECUTE)
endif()

# Distribution working directory
file(MAKE_DIRECTORY "${_dist_dir}")


## Targets

# Distribution zip
add_custom_target(distribution
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${_dist_stage}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${_dist_stage}/tools"
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_BINARY_DIR}/merged-binary.bin" "${_dist_stage}/${_dist_name}.bin"
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_BINARY_DIR}/flash.bat" "${CMAKE_BINARY_DIR}/flash.sh" "${CMAKE_BINARY_DIR}/README.txt"
            "${_dist_stage}"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_BINARY_DIR}/espflash" "${_dist_stage}/tools"
    COMMAND "${CMAKE_COMMAND}" -E tar cf "${_dist_name}.zip" --format=zip "${_dist_name}"
    WORKING_DIRECTORY "${_dist_dir}"
    COMMENT "Packaging ${_dist_name}.zip"
    USES_TERMINAL
    VERBATIM)
add_dependencies(distribution merge-bin espflash-download)
