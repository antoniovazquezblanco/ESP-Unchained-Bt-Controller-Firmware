/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32 LMP descriptor tables.
 */

#ifndef LMP_DESC_TAB_H
#define LMP_DESC_TAB_H

#include <stdint.h>

/**
 * One LMP PDU descriptor, the per-opcode entry that drives lmp_pack/lmp_unpack.
 */
typedef struct
{
    uint8_t opcode;      /**< LMP opcode, or the extended opcode in lmp_ext_desc_tab. */
    uint8_t len;         /**< On-air PDU length, opcode byte(s) included. */
    uint8_t reserved[2]; /**< The lookup reads opcode as a byte and skips these. */
    const char *fmt;     /**< Field layout: B byte, H 16-bit, L 32-bit, a decimal
                              prefix turning B into an array of that many bytes. */
} lmp_desc_t;
_Static_assert(sizeof(lmp_desc_t) == 8, "the ROM walks these with an 8 byte stride");

/** Entries in each table. The ROM lookups hardcode these counts. */
#define LMP_DESC_TAB_SIZE 57
#define LMP_EXT_DESC_TAB_SIZE 26

/** Opcode that escapes to lmp_ext_desc_tab, which is then keyed on the next byte. */
#define LMP_OPCODE_ESCAPE 127

/** Descriptors for the plain opcodes, keyed on pdu[0] >> 1. */
extern const lmp_desc_t lmp_desc_tab[];

/** Descriptors for the escaped opcodes, keyed on pdu[1]. */
extern const lmp_desc_t lmp_ext_desc_tab[];

#endif /* LMP_DESC_TAB_H */
