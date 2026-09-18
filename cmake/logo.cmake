# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Boot logo helper
#
# When enabled, generate the RGB565 boot-logo header from the sprites in
# utils/logo at build time (needs Pillow in the build Python). The header lands
# in the build tree and splash.c picks it up from there.

if(CONFIG_UNCHAINED_DISPLAY_LOGO)
    set(logo_dir "${CMAKE_SOURCE_DIR}/utils/logo")
    set(logo_hdr "${CMAKE_BINARY_DIR}/logo_img.h")
    add_custom_command(
        OUTPUT "${logo_hdr}"
        COMMAND "${PYTHON}" "${logo_dir}/render_logo.py"
                320 240 --links 2 --format c -o "${logo_hdr}"
        DEPENDS "${logo_dir}/render_logo.py"
                "${logo_dir}/sprites/part1_left.png"
                "${logo_dir}/sprites/part2_mid.png"
                "${logo_dir}/sprites/part3_right.png"
                "${logo_dir}/sprites/part1_left_broken.png"
                "${logo_dir}/sprites/part3_right_broken.png"
                "${logo_dir}/sprites/rune.png"
        COMMENT "Generating boot logo header (logo_img.h)"
        VERBATIM)
    add_custom_target(unchained_logo_img DEPENDS "${logo_hdr}")

    # main is registered by project(); wire the generation into its library.
    idf_component_get_property(logo_main_lib main COMPONENT_LIB)
    add_dependencies(${logo_main_lib} unchained_logo_img)
    target_include_directories(${logo_main_lib} PRIVATE "${CMAKE_BINARY_DIR}")
endif()
