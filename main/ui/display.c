/*
 * Display support source file.
 */
#include "display.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_DISPLAY_ENABLED

#include "esp_log.h"
#include "esp_check.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

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

static esp_lcd_panel_handle_t s_panel;

esp_lcd_panel_handle_t display_panel(void)
{
    return s_panel;
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
