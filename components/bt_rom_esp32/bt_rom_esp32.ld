/* SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Absolute ESP32 controller ROM-data symbols not provided by ESP-IDF's
 * rom.ld. PROVIDE so a consumer that never references one costs nothing.
 */

/* Local BT SIG Company Identifier (CompId); see co_default_compid in the header. */
PROVIDE ( co_default_compid = 0x3ffae6fa );

/* BLE RX descriptor ring; see lld_rx_desc in lld_pdu_desc.h. The base is the
 * l32r constant r_lld_pdu_rx_handler loads for the LL header word, 4 below the
 * buffer-pointer base r_em_buf_rx_buff_addr_get keys off (0x3ffb0950). */
PROVIDE ( lld_rx_desc = 0x3ffb094c );
