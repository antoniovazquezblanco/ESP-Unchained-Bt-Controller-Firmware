/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * ESP32-C5 BLE controller glue.
 */

#ifndef ESP32C5_BT_ROM_H
#define ESP32C5_BT_ROM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sdk_config.h"

/* ---- Company Identifier (manufacturer id) ---- */

/*
 * Both the HCI Read_Local_Version_Information reply and the over-the-air
 * LL_VERSION_IND read priv_config_opts_ptr->company_id, so changing it re-brands
 * the controller to the host and over the air. The pointer is NULL until the
 * controller is initialised, so change it AFTER controller init. See
 * reversing/esp32c5-bt-rom.md section 5.
 */

#define ESP32C5_BT_COMPID_DEFAULT 0x02E5 /* Espressif */

/* Returns false if the controller config struct is not yet allocated. */
static inline bool esp32c5_bt_set_compid(uint16_t compid)
{
    if (priv_config_opts_ptr == NULL) {
        return false;
    }
    priv_config_opts_ptr->company_id = compid;
    return true;
}

static inline uint16_t esp32c5_bt_get_compid(void)
{
    if (priv_config_opts_ptr == NULL) {
        return 0;
    }
    return priv_config_opts_ptr->company_id;
}

#endif /* ESP32C5_BT_ROM_H */
