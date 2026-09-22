/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Typed declarations for the ESP32 classic (BR/EDR) Bluetooth controller's ROM
 * functions.
 */

#ifndef ESP32_BT_ROM_H
#define ESP32_BT_ROM_H

#include <stdbool.h>

/**
 * Signature of r_bt_util_buf_init, slot 0 of the IP functions table.
 * It (re)initialises the controller's exchange-memory buffer pools, tearing
 * them  down first when reset is non-zero. Recovered from the loaded
 * controller.
 */
typedef void (*r_bt_util_buf_init_fn_t)(bool reset);

/**
 * RivieraWaves "IP functions" dispatch table type definition.
 *
 * This is a function pointer table used by RivieraWaves' Bluetooth stack.
 * Because the base RivieraWaves stack is provided in ROM (read-only memory),
 * it cannot be modified directly. For this reason, the stack calls the functions
 * via this pointer table indirectly because it  allows for dynamic replacement
 * or patching of the functions at runtime.
 */
struct r_ip_funcs {
    r_bt_util_buf_init_fn_t bt_util_buf_init; /* [0] */
    void *fn[];                               /* [1..] not yet typed */
};

/**
 * Pointer to the RivieraWaves IP functions table.
 * 
 * The linker resolves the symbol location against libbtdm_app.a with a fallback
 * on esp32.rom.ld to the internal ROM position.
 */
extern struct r_ip_funcs *r_ip_funcs_p;

/**
 * A self-test function for the ESP32 Bluetooth ROM functions.
 */
bool esp32_bt_rom_selftest(void);

#endif /* ESP32_BT_ROM_H */
