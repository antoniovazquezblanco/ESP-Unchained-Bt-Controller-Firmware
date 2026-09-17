/*
 * On-screen UI.
 *
 * Facade over the display / console subcomponents. It wires them together and
 * owns the one policy this firmware wants: mirror the ESP log onto the panel.
 */

#include "ui.h"
#include "display.h"
#include "console.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_DISPLAY_ENABLED

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"

#include "esp_log.h"

static const char *TAG = "UI";

static bool s_ready;
static StreamBufferHandle_t s_sb;
static vprintf_like_t s_prev;

/* esp_log hook: keep the normal console, and tee the text to the UI task. */
static int log_vprintf(const char *fmt, va_list ap)
{
    char tmp[200];
    va_list ap2;
    va_copy(ap2, ap);
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    int r = s_prev ? s_prev(fmt, ap2) : n;  /* keep the normal console */
    va_end(ap2);
    if (s_sb && n > 0) {
        size_t len = (n < (int)sizeof(tmp)) ? (size_t)n : sizeof(tmp) - 1;
        xStreamBufferSend(s_sb, tmp, len, 0);   /* non-blocking; drop if full */
    }
    return r;
}

static void ui_task(void *arg)
{
    char rx[128];
    for (;;) {
        size_t got = xStreamBufferReceive(s_sb, rx, sizeof(rx), pdMS_TO_TICKS(100));
        if (got) {
            ui_console_write(rx, got);
            ui_console_render();
        }
    }
}

void ui_init(void)
{
    if (s_ready) return;                    /* already initialised */

    /* The panel is ours to bring up: callers only ever talk to ui.h. */
    esp_err_t ret = display_init();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "display init failed: %s", esp_err_to_name(ret));
        return;
    }
    if (ui_console_init() != ESP_OK) {
        ESP_LOGW(TAG, "console init failed");
        return;
    }

    s_sb = xStreamBufferCreate(2048, 1);
    if (!s_sb) return;
    s_ready = true;

    xTaskCreate(ui_task, "ui", 4096, NULL, 5, NULL);
    s_prev = esp_log_set_vprintf(log_vprintf);
}

#else /* !CONFIG_UNCHAINED_DISPLAY_ENABLED */

void ui_init(void) { }

#endif
