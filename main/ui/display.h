/*
 * Display support header file.
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "esp_err.h"
#include "esp_lcd_types.h"

esp_err_t display_init(void);              /* bring up the panel; ESP_OK if ready */
esp_lcd_panel_handle_t display_panel(void); /* panel handle, or NULL if not up */

#endif /* DISPLAY_H */
