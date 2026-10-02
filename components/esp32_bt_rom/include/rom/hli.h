/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * High Level Interrupts (HLI) API
 */
#ifndef ROM_HLI_H
#define ROM_HLI_H

extern int hli_intr_disable();
extern void hli_intr_restore(int);

#endif /* ROM_HLI_H */
