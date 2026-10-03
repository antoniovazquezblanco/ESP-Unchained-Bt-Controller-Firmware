/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Interface to the target's unchained layer. One esp*_bt_unchained component
 * implements it per target and the build picks the one matching IDF_TARGET.
 */

#ifndef BT_UNCHAINED_H
#define BT_UNCHAINED_H

/*
 * Unlock whatever this target's controller has to offer: our own vendor command
 * set (Company 0xF00D -- see vsc.h), which on every target includes SET_BDADDR,
 * the link traffic monitor and the scan-channel pin. Call once, after
 * esp_bt_controller_enable().
 */
void bt_unchained_init(void);

#endif /* BT_UNCHAINED_H */
