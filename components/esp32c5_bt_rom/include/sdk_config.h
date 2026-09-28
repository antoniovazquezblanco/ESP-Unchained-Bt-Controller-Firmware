/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C5 controller "sdk config" private options.
 */

#ifndef ESP32C5_SDK_CONFIG_H
#define ESP32C5_SDK_CONFIG_H

#include <stddef.h>
#include <stdint.h>

/*
 * Controller-private options struct (72 bytes).
 */
typedef struct
{
    uint16_t company_id;        /* 0x00 BT SIG Company Identifier (manufacturer) */
    uint16_t subvers_nr;        /* 0x02 LL SubVersNr */
    uint8_t _reserved_04[0x14]; /* 0x04..0x17 controller-private config */
    uint8_t vers_nr;            /* 0x18 LL VersNr */
    uint8_t _reserved_19[0x2f]; /* 0x19..0x47 controller-private config */
} priv_config_opts_t;

_Static_assert(sizeof(priv_config_opts_t) == 0x48, "priv_config_opts spans 0x48 bytes");
_Static_assert(offsetof(priv_config_opts_t, company_id) == 0x00, "company_id must sit at offset 0x00");
_Static_assert(offsetof(priv_config_opts_t, vers_nr) == 0x18, "vers_nr must sit at offset 0x18");

/* The live controller config (libble_app pointer, NULL before controller init). */
extern priv_config_opts_t *priv_config_opts_ptr;

/* libble_app sdk-config helpers. */
extern void r_priv_sdk_config_options_init(void); /* allocates and applies defaults, incl. company_id = 0x02E5 */

#endif /* ESP32C5_SDK_CONFIG_H */
