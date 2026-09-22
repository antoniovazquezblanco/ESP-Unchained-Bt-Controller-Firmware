/* SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Absolute ESP32 BR/EDR controller ROM-data symbols not provided by ESP-IDF's
 * rom.ld. PROVIDE so a consumer that never references one costs nothing.
 */

/* Local BT SIG Company Identifier (CompId); see co_default_compid in the header. */
PROVIDE ( co_default_compid = 0x3ffae6fa );
