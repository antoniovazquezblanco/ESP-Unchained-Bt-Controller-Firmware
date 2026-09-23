/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Unlocks the ESP32-C3 controller: stock Espressif vendor-specific commands.
 *
 * The precompiled controller (libbtdm_app.a) registers its vendor-specific
 * commands only when the matching enable function is called. ESP-IDF wraps those
 * calls in `#ifdef CONFIG_BT_BLUEDROID_ENABLED / CONFIG_BT_NIMBLE_ENABLED`, so a
 * controller-only build leaves the whole VS set dormant (every 0x3F opcode
 * answers 0x01 "Unknown HCI Command"). We call them ourselves.
 *
 * The C3 exports six enablers and no vendor-specific *event* enablers at all.
 * Every symbol was verified with `nm` over the controller library, not from the
 * headers: the C3 header documents its dup-exc enabler as
 * `advFilter_stack_eanbleDupExcListCmd`, which does not exist -- the library
 * exports `advFilter_stack_enableDupExcListVsCmd`.
 *
 * Requires ESP-IDF v6.0+ (or a VS-enable backport); we feature-detect on
 * esp_bt_vs.h to keep compiling on older IDF.
 */
#include "bt_unchained.h"

#include <stdbool.h>

#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "UNCHAINED";

#if defined(__has_include) && __has_include("esp_bt_vs.h")
#define HAVE_VS_ENABLE 1
#endif

#ifdef HAVE_VS_ENABLE

extern void bt_stack_enableEchoVsCmd(bool en);
extern void advFilter_stack_enableDupExcListVsCmd(bool en);
extern void scan_stack_enableAdvFlowCtrlVsCmd(bool en);
extern void adv_stack_enableClearLegacyAdvVsCmd(bool en);
extern void chanSel_stack_enableSetCsaVsCmd(bool en);
extern void esp_ble_internalTestFeaturesEnable(bool en);

void bt_unchained_init(void)
{
    bt_stack_enableEchoVsCmd(true);              /* 0xFC81 ECHO */
    advFilter_stack_enableDupExcListVsCmd(true); /* 0xFD08 CONFIG_DUP_EXC_LIST */
    scan_stack_enableAdvFlowCtrlVsCmd(true);     /* 0xFD09/0xFD0A ADV report flow ctrl */
    adv_stack_enableClearLegacyAdvVsCmd(true);   /* 0xFD0C CLR_LEGACY_ADV */
    chanSel_stack_enableSetCsaVsCmd(true);       /* 0xFD12 ENABLE_CSA2 */
    esp_ble_internalTestFeaturesEnable(true);    /* 0xFD13 CFG_TEST_RELATED */
    ESP_LOGI(TAG, "vendor-specific HCI commands enabled");
}

#else /* ESP-IDF too old to expose the VS enable API */

void bt_unchained_init(void)
{
    ESP_LOGW(TAG, "this ESP-IDF does not expose the VS enable API (v6.0+ needed); "
                  "vendor-specific HCI commands left disabled");
}

#endif
