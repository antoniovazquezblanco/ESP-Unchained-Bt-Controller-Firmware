/*
 * On-screen text console: a title bar plus a scrolling monospaced log area.
 */
#include "console.h"
#include "sdkconfig.h"

#if CONFIG_UNCHAINED_DISPLAY_ENABLED

#include <stdbool.h>
#include <string.h>

#include "esp_heap_caps.h"

#include "display.h"
#include "font.h"
#include "gfx.h"

/* ---- Layout ------------------------------------------------------------- */
/* The text grid is fixed at compile time (it sizes the model below), so it is
 * derived from the configured panel size rather than display_width/height(). */
#define SCR_W   CONFIG_UNCHAINED_DISPLAY_WIDTH
#define SCR_H   CONFIG_UNCHAINED_DISPLAY_HEIGHT
#define GLYPH   8                           /* font8x8 cell size */
#define BAR_H   (2 * GLYPH)                 /* title bar height */
#define COLS    (SCR_W / GLYPH)
#define ROWS    ((SCR_H - BAR_H) / GLYPH)

/* Colours are RGB565, stored byte-swapped for the panel (big-endian). */
#define C_BAR_BG GFX_SWAP(0x001F)   /* blue  */
#define C_BAR_FG GFX_SWAP(0xFFFF)   /* white */
#define C_LOG_BG GFX_SWAP(0x0000)   /* black */
#define C_LOG_FG GFX_SWAP(0x07E0)   /* green */

/* ---- State -------------------------------------------------------------- */
static uint16_t *s_line;                    /* one text row: SCR_W x GLYPH */
static char s_buf[ROWS][COLS];              /* console text model */
static int s_col;                           /* cursor column on bottom row */
static bool s_in_esc;                       /* stripping an ANSI escape */
static const char *s_title = "Bluetooth Unchained Fw";

/* ---- Drawing ------------------------------------------------------------ */
static void draw_topbar(void)
{
    uint16_t *bar = heap_caps_malloc(SCR_W * BAR_H * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!bar) return;
    gfx_fill(bar, SCR_W, BAR_H, C_BAR_BG);
    int len = (int)strlen(s_title);
    int x0 = (SCR_W - len * GLYPH) / 2;
    if (x0 < 0) x0 = 0;
    int y0 = (BAR_H - GLYPH) / 2;
    for (int i = 0; i < len && (x0 + i * GLYPH + GLYPH) <= SCR_W; i++) {
        gfx_glyph(bar, SCR_W, x0 + i * GLYPH, y0, s_title[i], &font8x8, C_BAR_FG);
    }
    display_blit(0, 0, SCR_W, BAR_H, bar);
    heap_caps_free(bar);
}

/* ---- Console model ------------------------------------------------------ */
static void console_newline(void)
{
    memmove(s_buf[0], s_buf[1], (ROWS - 1) * COLS);
    memset(s_buf[ROWS - 1], ' ', COLS);
    s_col = 0;
}

static void console_putc(char c)
{
    if (s_in_esc) {                         /* strip ANSI colour escapes */
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) s_in_esc = false;
        return;
    }
    if (c == 0x1B) { s_in_esc = true; return; }
    if (c == '\r') return;
    if (c == '\n') { console_newline(); return; }
    if (c == '\t') c = ' ';
    if (c < 0x20 || c > 0x7E) return;
    if (s_col >= COLS) console_newline();
    s_buf[ROWS - 1][s_col++] = c;
}

/* ---- Public API --------------------------------------------------------- */
void ui_console_set_title(const char *title)
{
    s_title = title;
    if (s_line) draw_topbar();              /* repaint only once initialised */
}

void ui_console_write(const char *data, size_t len)
{
    for (size_t i = 0; i < len; i++) console_putc(data[i]);
}

void ui_console_render(void)
{
    if (!s_line) return;
    for (int r = 0; r < ROWS; r++) {
        gfx_fill(s_line, SCR_W, GLYPH, C_LOG_BG);
        for (int c = 0; c < COLS; c++) {
            char ch = s_buf[r][c];
            if (ch != ' ') gfx_glyph(s_line, SCR_W, c * GLYPH, 0, ch, &font8x8, C_LOG_FG);
        }
        display_blit(0, BAR_H + r * GLYPH, SCR_W, GLYPH, s_line);
    }
}

esp_err_t ui_console_init(void)
{
    if (s_line) return ESP_OK;              /* already initialised */

    s_line = heap_caps_malloc(SCR_W * GLYPH * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!s_line) return ESP_ERR_NO_MEM;

    for (int r = 0; r < ROWS; r++) memset(s_buf[r], ' ', COLS);
    s_col = 0;

    draw_topbar();
    ui_console_render();                     /* clears the log area */
    return ESP_OK;
}

#endif /* CONFIG_UNCHAINED_DISPLAY_ENABLED */
