/*
 * Bitmap fonts.
 *
 * A font is a fixed-cell bitmap: glyph(c) returns `h` bytes, one per row, with
 * bit 0 = leftmost pixel. Out-of-range characters fall back to a blank cell, so
 * callers never need to range-check.
 */
#ifndef UI_FONT_H
#define UI_FONT_H

#include <stdint.h>

typedef struct {
    uint8_t w;                          /* cell width in pixels  */
    uint8_t h;                          /* cell height in pixels */
    const uint8_t *(*glyph)(char c);    /* h bytes, bit0 = leftmost pixel */
} font_t;

/* 8x8 ASCII font (0x20-0x7F). */
extern const font_t font8x8;

#endif /* UI_FONT_H */
