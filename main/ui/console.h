/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * On-screen text console: a title bar plus a scrolling monospaced log area.
 *
 * Holds the text model and renders it to the display; it does not source its own
 * text. Feed it bytes with console_write() (newlines scroll, over-long lines
 * wrap, ANSI colour escapes are stripped) and call console_render() to paint.
 */
#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <stddef.h>

#include "esp_err.h"

/* Allocate buffers, clear the model and paint the title bar + empty log area.
 * Requires display_init() to have succeeded first. */
esp_err_t ui_console_init(void);

/* Set the title-bar text and repaint the bar (safe before or after init). */
void ui_console_set_title(const char *title);

/* Append bytes to the log (handles '\n', '\t', wrapping and ANSI escapes). */
void ui_console_write(const char *data, size_t len);

/* Paint the current text model to the display. */
void ui_console_render(void);

#endif /* UI_CONSOLE_H */
