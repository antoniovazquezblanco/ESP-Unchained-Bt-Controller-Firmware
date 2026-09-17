/*
 * Software rendering into an in-RAM RGB565 buffer.
 */
#include "gfx.h"

void gfx_fill(uint16_t *buf, int w, int h, uint16_t color)
{
    int n = w * h;
    for (int i = 0; i < n; i++) buf[i] = color;
}

void gfx_glyph(uint16_t *buf, int stride, int x, int y, char ch,
               const font_t *font, uint16_t fg)
{
    const uint8_t *g = font->glyph(ch);
    for (int ry = 0; ry < font->h; ry++) {
        uint8_t row = g[ry];
        uint16_t *p = &buf[(y + ry) * stride + x];
        for (int cx = 0; cx < font->w; cx++) {
            if (row & (1 << cx)) p[cx] = fg;
        }
    }
}

void gfx_text(uint16_t *buf, int stride, int x, int y, const char *s,
              const font_t *font, uint16_t fg)
{
    for (; *s; s++, x += font->w) {
        gfx_glyph(buf, stride, x, y, *s, font, fg);
    }
}
