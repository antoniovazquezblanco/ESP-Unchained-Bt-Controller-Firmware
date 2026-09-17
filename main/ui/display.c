/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Display support source file.
 */
#include "display.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_DISPLAY_ENABLED

#include <string.h>

#include "esp_log.h"
#include "esp_check.h"
#include "esp_attr.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "gfx.h"

/* The bus pins are wiring, so they have no default worth inheriting: a board
 * that enables the display must state all four. Catching it here beats
 * debugging it on the bench -- esp_lcd cannot read back from the panel (MISO is
 * not wired), so a mis-pinned display reports success on every call and simply
 * shows nothing. */
#if CONFIG_UNCHAINED_DISPLAY_SCLK_GPIO < 0 || CONFIG_UNCHAINED_DISPLAY_MOSI_GPIO < 0 ||     CONFIG_UNCHAINED_DISPLAY_CS_GPIO < 0   || CONFIG_UNCHAINED_DISPLAY_DC_GPIO < 0
#error "UNCHAINED_DISPLAY_ENABLED but the display bus pins are unset. Set SCLK/MOSI/CS/DC (and the panel's RST and backlight, if it wires them) in boards/<name>.defaults."
#endif

static const char *TAG = "DISP";

#define DISP_W    CONFIG_UNCHAINED_DISPLAY_WIDTH
#define DISP_H    CONFIG_UNCHAINED_DISPLAY_HEIGHT
#define DISP_HOST CONFIG_UNCHAINED_DISPLAY_SPI_HOST

#define DRAW_STRIP 32                       /* rows per DMA blit in the draw helpers */

static esp_lcd_panel_handle_t s_panel;
static SemaphoreHandle_t s_blit_done;       /* given when a blit's DMA completes */

int display_width(void)  { return DISP_W; }
int display_height(void) { return DISP_H; }

/* esp_lcd queues colour transfers (async); this fires when one finishes. */
static bool IRAM_ATTR on_blit_done(esp_lcd_panel_io_handle_t io,
                                   esp_lcd_panel_io_event_data_t *edata, void *ctx)
{
    BaseType_t hp = pdFALSE;
    xSemaphoreGiveFromISR(s_blit_done, &hp);
    return hp == pdTRUE;
}

void display_blit(int x, int y, int w, int h, const uint16_t *px)
{
    if (!s_panel) return;
    esp_lcd_panel_draw_bitmap(s_panel, x, y, x + w, y + h, px);
    /* The transfer is async and reads `px` by DMA; wait so the caller may reuse
     * or free the buffer immediately. */
    xSemaphoreTake(s_blit_done, portMAX_DELAY);
}

/* ---- Higher-level draw helpers (own the DMA scratch + strip loop) -------- */

void display_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0) return;
    uint16_t *buf = heap_caps_malloc(w * DRAW_STRIP * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!buf) return;
    gfx_fill(buf, w, DRAW_STRIP, color);
    for (int yy = 0; yy < h; yy += DRAW_STRIP) {
        int rows = (h - yy < DRAW_STRIP) ? (h - yy) : DRAW_STRIP;
        display_blit(x, y + yy, w, rows, buf);
    }
    heap_caps_free(buf);
}

/* Draw a w*h RGB565 image (panel byte order). `src` may live in flash: it is
 * copied into a DMA scratch buffer a strip at a time. */
void display_draw_image(int x, int y, int w, int h, const uint16_t *src)
{
    if (w <= 0 || h <= 0) return;
    uint16_t *buf = heap_caps_malloc(w * DRAW_STRIP * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!buf) return;
    for (int yy = 0; yy < h; yy += DRAW_STRIP) {
        int rows = (h - yy < DRAW_STRIP) ? (h - yy) : DRAW_STRIP;
        memcpy(buf, &src[(size_t)yy * w], (size_t)rows * w * sizeof(uint16_t));
        display_blit(x, y + yy, w, rows, buf);
    }
    heap_caps_free(buf);
}

/* Draw a NUL-terminated string at (x, y): glyphs in `fg` on a `bg` box. */
void display_draw_text(int x, int y, const char *s, const font_t *font,
                       uint16_t fg, uint16_t bg)
{
    int tw = (int)strlen(s) * font->w;
    if (tw <= 0) return;
    uint16_t *buf = heap_caps_malloc((size_t)tw * font->h * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!buf) return;
    gfx_fill(buf, tw, font->h, bg);
    gfx_text(buf, tw, 0, 0, s, font, fg);
    display_blit(x, y, tw, font->h, buf);
    heap_caps_free(buf);
}

static void backlight_on(void)
{
#if CONFIG_UNCHAINED_DISPLAY_BL_GPIO >= 0
    gpio_config_t bl = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << CONFIG_UNCHAINED_DISPLAY_BL_GPIO,
    };
    gpio_config(&bl);
    gpio_set_level(CONFIG_UNCHAINED_DISPLAY_BL_GPIO,
                   CONFIG_UNCHAINED_DISPLAY_BL_ACTIVE_HIGH ? 1 : 0);
#endif
}

esp_err_t display_init(void)
{
    s_blit_done = xSemaphoreCreateBinary();
    ESP_RETURN_ON_FALSE(s_blit_done, ESP_ERR_NO_MEM, TAG, "blit semaphore");

    spi_bus_config_t bus = {
        .sclk_io_num = CONFIG_UNCHAINED_DISPLAY_SCLK_GPIO,
        .mosi_io_num = CONFIG_UNCHAINED_DISPLAY_MOSI_GPIO,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISP_W * 40 * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(DISP_HOST, &bus, SPI_DMA_CH_AUTO), TAG, "spi bus");

    esp_lcd_panel_io_handle_t io = NULL;
    esp_lcd_panel_io_spi_config_t io_cfg = {
        .cs_gpio_num = CONFIG_UNCHAINED_DISPLAY_CS_GPIO,
        .dc_gpio_num = CONFIG_UNCHAINED_DISPLAY_DC_GPIO,
        .pclk_hz = CONFIG_UNCHAINED_DISPLAY_PCLK_HZ,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = on_blit_done,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)DISP_HOST,
                                                 &io_cfg, &io), TAG, "panel io");

    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = CONFIG_UNCHAINED_DISPLAY_RST_GPIO,
#if CONFIG_UNCHAINED_DISPLAY_BGR_ORDER
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
#else
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
#endif
        .bits_per_pixel = 16,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_st7789(io, &panel_cfg, &s_panel), TAG, "st7789");

    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "reset");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "init");
    /* Orientation / colour as observed in the badge firmware. */
    esp_lcd_panel_invert_color(s_panel, true);
    esp_lcd_panel_swap_xy(s_panel, true);
    esp_lcd_panel_mirror(s_panel, true, false);
    esp_lcd_panel_disp_on_off(s_panel, true);

    backlight_on();

    ESP_LOGI(TAG, "ST7789 ready (%dx%d)", DISP_W, DISP_H);
    return ESP_OK;
}

#else /* !CONFIG_UNCHAINED_DISPLAY_ENABLED */

esp_err_t display_init(void) { return ESP_OK; }

#endif
