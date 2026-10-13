/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Addressable status LED(s) over RMT (WS2812 / SK6812 / SK6805-compatible).
 */
#include "status_led.h"

#include "sdkconfig.h"

#if CONFIG_UNCHAINED_LED_ENABLED

#if CONFIG_UNCHAINED_LED_GPIO < 0
#error "UNCHAINED_LED_ENABLED but UNCHAINED_LED_GPIO is unset. Set the LED data GPIO (and count) in boards/<name>.defaults."
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "LED";

#define LED_GPIO CONFIG_UNCHAINED_LED_GPIO
#define LED_COUNT CONFIG_UNCHAINED_LED_COUNT
#define LED_MAX CONFIG_UNCHAINED_LED_BRIGHTNESS /* per-channel brightness ceiling */

#define FRAME_MS 33      /* ~30 Hz render loop                         */
#define BREATH_FRAMES 60 /* heartbeat period (~2 s), triangle wave     */
#define BLINK_FRAMES 2   /* host TX/RX blink hold (~66 ms)             */
#define CAPTURE_FRAMES 3 /* capture flash hold (~100 ms)              */
#define DROP_FRAMES 24   /* drop stays visible (~0.8 s)               */

typedef struct
{
    uint8_t r, g, b;
} rgb_t;

static const rgb_t OFF = {0, 0, 0};
static const rgb_t AMBER = {255, 160, 0};   /* booting (pre-ready)              */
static const rgb_t WHITE = {255, 255, 255}; /* ready / resting heartbeat (dim)  */
static const rgb_t GREEN = {0, 255, 0};     /* host -> controller (TX)          */
static const rgb_t BLUE = {0, 0, 255};      /* controller -> host (RX)          */
static const rgb_t CYAN = {0, 255, 255};    /* TX + RX in the same frame        */
static const rgb_t RED = {255, 0, 0};       /* dropped packet                   */

/* Static colour held by the accent range (0 = off). */
static const rgb_t ACCENT = {
    (CONFIG_UNCHAINED_LED_ACCENT_RGB >> 16) & 0xFF,
    (CONFIG_UNCHAINED_LED_ACCENT_RGB >> 8) & 0xFF,
    CONFIG_UNCHAINED_LED_ACCENT_RGB & 0xFF,
};

static led_strip_handle_t s_strip;

/* Bitmask of events seen since the last frame; OR-ed from any context (incl. the
 * controller callback), drained by the render task. */
static volatile uint32_t s_pending;

void status_led_notify(status_led_event_t ev)
{
    __atomic_or_fetch(&s_pending, 1u << (unsigned)ev, __ATOMIC_RELAXED);
}

/* Write one pixel, scaled to the configured brightness ceiling. */
static void put(int i, rgb_t c)
{
    led_strip_set_pixel(s_strip, i,
                        (uint32_t)c.r * LED_MAX / 255,
                        (uint32_t)c.g * LED_MAX / 255,
                        (uint32_t)c.b * LED_MAX / 255);
}

static rgb_t dim(rgb_t c, uint32_t num, uint32_t den)
{
    return (rgb_t){c.r * num / den, c.g * num / den, c.b * num / den};
}

/* Idle colour: amber until the controller is up, then a dim breathing white,
 * kept well below the activity flashes so they clearly outshine it. */
static rgb_t base_color(bool ready, uint32_t phase)
{
    if (!ready) {
        return AMBER;
    }
    uint32_t half = BREATH_FRAMES / 2;
    uint32_t t = phase % BREATH_FRAMES;
    uint32_t level = t < half ? t : BREATH_FRAMES - t; /* triangle 0..half */
    return dim(WHITE, level, BREATH_FRAMES);           /* peak ~50% of full white */
}

static void render(bool ready, uint32_t phase, int tx, int rx, int cap, int drop)
{
    /* One status colour, priority mux: worst/most-urgent state wins. It shows on
     * the status pixels [STATUS_FIRST, +STATUS_COUNT); the accent range holds its
     * static colour; pixels in neither range are left off. One status pixel is the
     * common case (STATUS_COUNT defaults to 1); a board can widen the range to
     * cover a multi-LED logo. */
    /* Capture is controller->host traffic too, so it shares RX's colour -- the
     * only red-free hues (green, blue, cyan) are already taken by TX/RX/both. */
    bool rxc = rx || cap;
    rgb_t s;
    if (drop) {
        s = RED;
    } else if (tx && rxc) {
        s = CYAN; /* both directions in one frame -> clean primary blend */
    } else if (rxc) {
        s = BLUE;
    } else if (tx) {
        s = GREEN;
    } else {
        s = base_color(ready, phase);
    }
    int sfirst = CONFIG_UNCHAINED_LED_STATUS_FIRST;
    int slast = sfirst + CONFIG_UNCHAINED_LED_STATUS_COUNT;
    int afirst = CONFIG_UNCHAINED_LED_ACCENT_FIRST;
    int alast = afirst + CONFIG_UNCHAINED_LED_ACCENT_COUNT;
    for (int i = 0; i < LED_COUNT; i++) {
        put(i, (i >= sfirst && i < slast) ? s : (i >= afirst && i < alast) ? ACCENT
                                                                           : OFF);
    }
    led_strip_refresh(s_strip); /* blocking RMT flush -- fine in task context */
}

static void status_led_task(void *arg)
{
    bool ready = false;
    uint32_t phase = 0;
    int tx = 0, rx = 0, cap = 0, drop = 0;

    for (;; phase++) {
        uint32_t ev = __atomic_exchange_n(&s_pending, 0, __ATOMIC_RELAXED);
        if (ev & (1u << STATUS_LED_READY)) {
            ready = true;
        }
        if (ev & (1u << STATUS_LED_HOST_TX)) {
            tx = BLINK_FRAMES;
        }
        if (ev & (1u << STATUS_LED_HOST_RX)) {
            rx = BLINK_FRAMES;
        }
        if (ev & (1u << STATUS_LED_CAPTURE)) {
            cap = CAPTURE_FRAMES;
        }
        if (ev & (1u << STATUS_LED_DROP)) {
            drop = DROP_FRAMES;
        }

        render(ready, phase, tx, rx, cap, drop);

        if (tx)
            tx--;
        if (rx)
            rx--;
        if (cap)
            cap--;
        if (drop)
            drop--;
        vTaskDelay(pdMS_TO_TICKS(FRAME_MS));
    }
}

void status_led_init(void)
{
    if (s_strip) {
        return; /* already up */
    }

    led_strip_config_t strip_cfg = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = LED_COUNT,
        .led_model = LED_MODEL_WS2812, /* SK6805/SK6812 share the WS2812 protocol */
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
    };
    led_strip_rmt_config_t rmt_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
    };
    esp_err_t ret = led_strip_new_rmt_device(&strip_cfg, &rmt_cfg, &s_strip);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "led_strip init failed: %s", esp_err_to_name(ret));
        return;
    }
    led_strip_clear(s_strip);

    if (xTaskCreate(status_led_task, "status_led", 3072, NULL, 4, NULL) != pdPASS) {
        ESP_LOGW(TAG, "task create failed");
        return;
    }
    ESP_LOGI(TAG, "%d LED(s) on GPIO%d", LED_COUNT, LED_GPIO);
}

#endif /* CONFIG_UNCHAINED_LED_ENABLED */
