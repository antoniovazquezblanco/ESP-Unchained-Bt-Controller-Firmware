/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Our vendor-specific HCI command set for the ESP32-C5. The opcodes and their
 * contracts are shared across targets (unchained_vsc.h); this header adds only the
 * C5 dispatch glue. The NimBLE-lineage controller dispatches vendor commands by OCF
 * (opcode & 0x3ff) from a linked list and packs the Command Complete itself, so
 * each handler just fills return parameters and returns a status.
 */

#ifndef VSC_H
#define VSC_H

#include "unchained_vsc.h"

/*
 * Install our vendor commands into the controller's VS dispatch list. Call once
 * after the controller is enabled (ble_ll_hci_env_p is valid by then).
 */
void vsc_register(void);

#endif /* VSC_H */
