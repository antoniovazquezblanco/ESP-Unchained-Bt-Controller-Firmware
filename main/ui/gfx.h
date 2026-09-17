/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Software rendering into an in-RAM RGB565 buffer.
 *
 * These helpers know nothing about the panel: they draw into a caller-owned
 * buffer, which the display layer then blits. Colours are 16-bit RGB565 in the
 * byte order the buffer is blitted with -- use GFX_SWAP() to match a panel that
 * expects big-endian pixels. Drawing is not clipped; callers pass coordinates
 * that fit the buffer.
 */
#ifndef UI_GFX_H
#define UI_GFX_H

#include <stdint.h>

#include "font.h"

/* Pack 8-8-8 into RGB565. */
#define GFX_RGB565(r, g, b) \
    ((uint16_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)))

/* Swap the two bytes of a 16-bit colour (RGB565 <-> panel big-endian). */
#define GFX_SWAP(c) \
    ((uint16_t)(((uint16_t)(c) >> 8) | ((uint16_t)(c) << 8)))

/* Fill a w*h buffer with a solid colour. */
void gfx_fill(uint16_t *buf, int w, int h, uint16_t color);

/* Draw one glyph at (x, y) into a buffer `stride` pixels wide. */
void gfx_glyph(uint16_t *buf, int stride, int x, int y, char ch,
               const font_t *font, uint16_t fg);

/* Draw a NUL-terminated string, advancing one cell width per character. */
void gfx_text(uint16_t *buf, int stride, int x, int y, const char *s,
              const font_t *font, uint16_t fg);

#endif /* UI_GFX_H */
