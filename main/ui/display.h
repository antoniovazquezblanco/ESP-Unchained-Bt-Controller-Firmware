/*
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

esp_err_t display_init(void);   /* bring up the panel; ESP_OK if ready */

int display_width(void);        /* panel width in pixels  */
int display_height(void);       /* panel height in pixels */

/* Blit a w*h RGB565 buffer to the rectangle at (x, y). */
void display_blit(int x, int y, int w, int h, const uint16_t *px);

#endif /* DISPLAY_H */
