/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C3 controller "sdk config" private options.
 *
 * sdk_cfg_priv_opts is a controller-private configuration struct kept in the
 * ROM-owned BTDM DRAM as an absolute libbtdm symbol at 0x3fcdf96c. Its size is
 * 0x48 (72) bytes: it runs up to the next symbol, privacy_en @ 0x3fcdf9b4.
 */

#ifndef ESP32C3_SDK_CONFIG_H
#define ESP32C3_SDK_CONFIG_H

#include <stddef.h>
#include <stdint.h>

/*
 * Controller-private options struct (72 bytes).
 */
typedef struct
{
    uint8_t _reserved_00[0x34]; /* 0x00..0x33 controller-private config */
    uint16_t company_id;        /* 0x34 BT SIG Company Identifier (manufacturer) */
    uint8_t _reserved_36[0x12]; /* 0x36..0x47 controller-private config */
} sdk_cfg_priv_opts_t;

_Static_assert(sizeof(sdk_cfg_priv_opts_t) == 0x48, "sdk_cfg_priv_opts spans 0x48 bytes (up to privacy_en)");
_Static_assert(offsetof(sdk_cfg_priv_opts_t, company_id) == 0x34, "company_id must sit at offset 0x34");

/* The live controller config (absolute libbtdm symbol @ 0x3fcdf96c). */
extern sdk_cfg_priv_opts_t sdk_cfg_priv_opts;

/* libbtdm sdk-config helpers. */
extern sdk_cfg_priv_opts_t *r_sdk_config_get_priv_opts(void);
extern void sdk_config_overwrite_priv_opts(void); /* re-applies defaults, incl. company_id = 0x02E5 */

#endif /* ESP32C3_SDK_CONFIG_H */
