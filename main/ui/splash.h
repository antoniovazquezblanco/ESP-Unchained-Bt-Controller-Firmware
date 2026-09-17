/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Boot splash: the logo image (optional) plus the firmware name.
 */
#ifndef UI_SPLASH_H
#define UI_SPLASH_H

#include <stdbool.h>

/* Paint the splash. Returns true if something was drawn (so the caller can hold
 * it on screen), false if there is nothing to show. */
bool splash_show(void);

#endif /* UI_SPLASH_H */
