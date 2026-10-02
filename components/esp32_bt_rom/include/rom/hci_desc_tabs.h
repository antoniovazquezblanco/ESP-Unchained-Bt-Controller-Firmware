/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 HCI descriptor tables.
 */

#ifndef HCI_DESC_TABS_H
#define HCI_DESC_TABS_H

#include <stdint.h>

/**
 * HCI pack/unpack function type.
 * Used where a format string cannot describe the layout.
 */
typedef uint16_t (*hci_pkupk_func_t)(uint8_t *out, uint8_t *in, uint16_t *out_len, uint16_t in_len);

/*
 * Command destination field structure:
 * | Bits | Field                             |
 * |------|-----------------------------------|
 * | 7    | Special return parameter packing  |
 * | 6    | Special command parameter packing |
 * | 5 4  | Host Layer Task ID                |
 * | 3..0 | Link Layer Task ID                |
 *
 * The low nibble picks the link-layer task the command is routed to.
 * Bits * 5..4 the host-layer one.
 * The top two say whether the parameters and the return data need a
 * function instead of a format string.
 */
#define HCI_CMD_DEST_LL_MASK 0x0F
#define HCI_CMD_DEST_HL_MASK 0x30
#define HCI_CMD_DEST_SPEC_PAR_PK_MSK 0x40 /**< par_fmt is a function     */
#define HCI_CMD_DEST_SPEC_RET_PK_MSK 0x80 /**< ret_fmt is a function */

/**
 * One HCI command descriptor.
 */
typedef struct
{
    uint16_t opcode;         /**< Full HCI opcode, though the lookup matches the OCF only. */
    uint8_t dest_field;      /**< Routing plus the two special-packing bits above. */
    uint8_t par_size_max;    /**< Largest command parameter block accepted. A missing
                                  descriptor is reported as 0xFF instead. */
    const void *par_fmt;     /**< Command parameter format, or an hci_pkupk_func_t
                                  when dest_field has HCI_CMD_DEST_SPEC_PAR_PK_MSK. NULL when the
                                  command takes no parameters. */
    const void *ret_par_fmt; /**< Return parameter format, or an hci_pkupk_func_t
                                  when dest_field has HCI_CMD_DEST_SPEC_RET_PK_MSK. NULL when
                                  the command answers with a Command Status. */
} hci_cmd_desc_t;
_Static_assert(sizeof(hci_cmd_desc_t) == 12, "the ROM walks these with a 12 byte stride");

/**
 * Root table entry type.
 * One opcode group and the descriptors that belong to it.
 */
typedef struct
{
    uint8_t ogf;                        /**< Opcode group this table covers. */
    uint8_t reserved;                   /**< The lookup reads ogf as a byte and skips this. */
    uint16_t nb_cmds;                   /**< Entries in cmd_desc_tab[]. */
    const hci_cmd_desc_t *cmd_desc_tab; /**< Pointer to the array of command descriptors. */
} hci_cmd_desc_tab_ref_t;
_Static_assert(sizeof(hci_cmd_desc_tab_ref_t) == 8, "the ROM walks these with an 8 byte stride");

/**
 * Entries in hci_cmd_desc_root_tab.
 */
#define HCI_CMD_DESC_ROOT_TAB_SIZE 8

/** Root HCI command descriptor table. */
extern const hci_cmd_desc_tab_ref_t hci_cmd_desc_root_tab[];

/*
 * The group tables the root table points at.
 */
extern const hci_cmd_desc_t hci_cmd_desc_tab_lk_ctrl[];  /**< OGF 0x01, 45 commands */
extern const hci_cmd_desc_t hci_cmd_desc_tab_lk_pol[];   /**< OGF 0x02, 11 commands */
extern const hci_cmd_desc_t hci_cmd_desc_tab_ctrl_bb[];  /**< OGF 0x03, 81 commands */
extern const hci_cmd_desc_t hci_cmd_desc_tab_info_par[]; /**< OGF 0x04, 7 commands  */
extern const hci_cmd_desc_t hci_cmd_desc_tab_stat_par[]; /**< OGF 0x05, 7 commands  */
extern const hci_cmd_desc_t hci_cmd_desc_tab_testing[];  /**< OGF 0x06, 4 commands  */
extern const hci_cmd_desc_t hci_cmd_desc_tab_le[];       /**< OGF 0x08, 46 commands */
extern const hci_cmd_desc_t hci_cmd_desc_tab_vs[];       /**< OGF 0x3F, 29 commands */

/**
 * Event destination field structure:
 * | Bits | Field                             |
 * |------|-----------------------------------|
 * | 7..2 | Reserved                          |
 * | 1..0 | Host Layer Task ID                |
 */
#define HCI_EVT_DEST_HL_MASK 0x03

/** hci_evt_desc_t.special_pack: par_fmt is a function rather than a format. */
#define HCI_EVT_PK_SPE 0x01

/**
 * One HCI event descriptor, used to pack an event on its way to the host.
 */
typedef struct
{
    uint8_t code;
    uint8_t dest_field;   /**< Host-layer destination, HCI_EVT_DEST_HL_MASK */
    uint8_t special_pack; /**< HCI_EVT_PK_SPE: par_fmt is a function */
    uint8_t reserved;
    const void *par_fmt;
} hci_evt_desc_t;
_Static_assert(sizeof(hci_evt_desc_t) == 8, "the ROM walks these with an 8 byte stride");

/** Entries in hci_evt_desc_tab. The ROM lookup hardcodes this count. */
#define HCI_EVT_DESC_TAB_SIZE 55

extern const hci_evt_desc_t hci_evt_desc_tab[];

#endif /* HCI_DESC_TABS_H */
