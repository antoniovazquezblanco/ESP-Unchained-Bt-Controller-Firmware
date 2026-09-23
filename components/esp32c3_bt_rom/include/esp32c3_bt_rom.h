/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 BLE controller ROM glue.
 *
 * The C3 ships a large slice of the RivieraWaves BLE stack in mask ROM. Those
 * functions are exported as absolute symbols by ESP-IDF's
 * components/esp_rom/esp32c3/ld/esp32c3.rom.bt_funcs.ld (linked whenever the BT
 * controller runs from IRAM, i.e. not CONFIG_BT_CTRL_RUN_IN_FLASH_ONLY), so this
 * header just declares typed prototypes for the unchained-relevant subset.
 */

#ifndef ESP32C3_BT_ROM_H
#define ESP32C3_BT_ROM_H

#include <stdbool.h>
#include <stdint.h>

/*
 * HCI command descriptor entry (12 bytes). r_hci_look_for_cmd_desc() returns one
 * of these; the lookup matches on the OCF (opcode & 0x3ff).
 */
typedef struct
{
    uint16_t opcode; /* OGF<<10 | OCF */
    uint16_t flags;  /* low nibble: CC/CS dest & return-param format; 0x40: handler self-unpacks */
    void *fn;        /* +4: param-unpack format (std cmds) or inline handler (vendor cmds) */
    void *ret_fmt;   /* +8: return-parameter format / handler */
} esp32c3_hci_cmd_desc_t;

/*
 * ESP vendor sub-dispatch entry (8 bytes), walked by
 * r_esp_vendor_hci_command_handler(). Matched on the full opcode; the handler
 * receives the opcode in a3.
 */
typedef struct
{
    uint16_t opcode;
    uint16_t _reserved;
    void *handler; /* void handler(uint16_t opcode) */
} esp32c3_esp_vendor_cmd_t;

/* ---- ROM functions (absolute symbols from esp32c3.rom.bt_funcs.ld) ---- */

/* HCI command entry: resolves the descriptor, unpacks params, routes the cmd. */
void r_hci_cmd_received(uint16_t opcode, uint16_t param_len, const void *params);

/*
 * Descriptor lookup: OGF -> per-group table, match on OCF. Returns an
 * esp32c3_hci_cmd_desc_t* or NULL. For opcodes > 0xFC80 it also scans the
 * RAM-registered vendor table (the custom-VSC hook, see the reversing doc).
 */
void *r_hci_look_for_cmd_desc(uint16_t opcode);

/* Install the ROM-default vendor command tables. */
void r_hci_register_vendor_desc_tab(void);
void r_register_esp_vendor_cmd_handler(void);

/* Message/command handler-table getters (return the dispatch table base). */
void *r_llc_hci_cmd_handler_tab_p_get(void);
void *r_llm_msg_handler_tab_p_get(void);
void *r_misc_msg_handler_tab_p_get(void);

/**
 * Self-test: confirm the ROM BT symbols resolved into the C3 ROM address window.
 */
bool esp32c3_bt_rom_selftest(void);

#endif /* ESP32C3_BT_ROM_H */
