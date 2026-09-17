/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Display subcomponent: owns the panel driver and hides esp_lcd.
 *
 * Upper layers never see the panel handle; they blit RGB565 pixel buffers to a
 * rectangle and query the panel size. Pixels are in the panel's native byte
 * order (see gfx.h GFX_SWAP).
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include "esp_err.h"

#include "font.h"

esp_err_t display_init(void);   /* bring up the panel; ESP_OK if ready */

int display_width(void);        /* panel width in pixels  */
int display_height(void);       /* panel height in pixels */

/* Blit a w*h RGB565 buffer to the rectangle at (x, y). Blocks until the transfer
 * completes, so the caller may reuse or free the buffer right after. */
void display_blit(int x, int y, int w, int h, const uint16_t *px);

/* Convenience drawing -- these own the DMA scratch buffer and strip loop.
 * Colours are RGB565 in the panel's byte order (see gfx.h GFX_SWAP). */
void display_fill_rect(int x, int y, int w, int h, uint16_t color);
void display_draw_image(int x, int y, int w, int h, const uint16_t *src);
void display_draw_text(int x, int y, const char *s, const font_t *font,
                       uint16_t fg, uint16_t bg);

#endif /* DISPLAY_H */
