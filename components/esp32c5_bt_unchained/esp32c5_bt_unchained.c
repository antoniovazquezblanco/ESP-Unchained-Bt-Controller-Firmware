/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Unlocks the ESP32-C5 controller: stock Espressif vendor-specific commands and
 * events.
 *
 * The precompiled controller (libble_app.a) registers its vendor-specific
 * commands only when the matching enable function is called. ESP-IDF wraps those
 * calls in `#ifdef CONFIG_BT_BLUEDROID_ENABLED / CONFIG_BT_NIMBLE_ENABLED`, so a
 * controller-only build leaves the whole VS set dormant (every 0x3F opcode
 * answers 0x01 "Unknown HCI Command"). We call them ourselves.
 *
 * Every symbol was verified with `nm` over the controller library, not from the
 * headers, which can be wrong about them. Requires ESP-IDF v6.0+ (or a VS-enable
 * backport); we feature-detect on esp_bt_vs.h to keep compiling on older IDF.
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

extern void advFilter_stack_enableDupExcListVsCmd(bool en);
extern void scan_stack_enableAdvFlowCtrlVsCmd(bool en);
extern void adv_stack_enableClearLegacyAdvVsCmd(bool en);
extern void arr_stack_enableMultiConnVsCmd(bool en);
extern void pcl_stack_enableSetRssiThreshVsCmd(bool en);
extern void chanSel_stack_enableSetCsaVsCmd(bool en);
extern void log_stack_enableLogsRelatedVsCmd(bool en);
extern void hci_stack_enableSetVsEvtMaskVsCmd(bool en);
extern void winWiden_stack_enableSetConstPeerScaVsCmd(bool en);
extern void esp_ble_internalTestFeaturesEnable(bool en);

extern void adv_stack_enableDisableAdvDelayVsCmd(bool en);
extern void extAdv_stack_setExtAdvConstantDidVsCmd(bool en);
extern void phy_stack_enableCCAEnVsCmd(bool en);
extern void phy_stack_enableCodedPhySwitchVsCmd(bool en);
extern void phy_stack_enableSetRxSensThresVsCmd(bool en);
extern void scan_stack_enableScanForeverVsCmd(bool en);
extern void scan_stack_enableSetScanADIOnlyFilterVsCmd(bool en);
extern void scan_stack_enableSetScanBackoffUpperLimitMaxVsCmd(bool en);
extern void txPower_stack_enableTxPowerVsCmd(bool en);

extern void adv_stack_enableScanReqRxdVsEvent(bool en);
extern void conn_stack_enableChanMapUpdCompVsEvent(bool en);
extern void sleep_stack_enableWakeupVsEvent(bool en);

void bt_unchained_init(void)
{
    /* Commands. */
    advFilter_stack_enableDupExcListVsCmd(true);     /* 0xFD08/0xFD0D/0xFD0E */
    scan_stack_enableAdvFlowCtrlVsCmd(true);         /* 0xFD09/0xFD0A (+ ADV_LOST 0xF0) */
    adv_stack_enableClearLegacyAdvVsCmd(true);       /* 0xFD0C */
    arr_stack_enableMultiConnVsCmd(true);            /* 0xFD0F/0xFD10 */
    pcl_stack_enableSetRssiThreshVsCmd(true);        /* 0xFD11 */
    chanSel_stack_enableSetCsaVsCmd(true);           /* 0xFD12 */
    log_stack_enableLogsRelatedVsCmd(true);          /* 0xFD14 */
    hci_stack_enableSetVsEvtMaskVsCmd(true);         /* 0xFD16 */
    winWiden_stack_enableSetConstPeerScaVsCmd(true); /* 0xFD18 */
    esp_ble_internalTestFeaturesEnable(true);        /* 0xFD13 CFG_TEST_RELATED */

    adv_stack_enableDisableAdvDelayVsCmd(true);
    extAdv_stack_setExtAdvConstantDidVsCmd(true);
    phy_stack_enableCCAEnVsCmd(true);
    phy_stack_enableCodedPhySwitchVsCmd(true);
    phy_stack_enableSetRxSensThresVsCmd(true);
    scan_stack_enableScanForeverVsCmd(true);
    scan_stack_enableSetScanADIOnlyFilterVsCmd(true);
    scan_stack_enableSetScanBackoffUpperLimitMaxVsCmd(true);
    txPower_stack_enableTxPowerVsCmd(true);
    ESP_LOGI(TAG, "vendor-specific HCI commands enabled");

    /* Events. */
    adv_stack_enableScanReqRxdVsEvent(true);      /* LE meta 0xC0 */
    conn_stack_enableChanMapUpdCompVsEvent(true); /* LE meta 0xC1 */
#if defined(CONFIG_BT_LE_SLEEP_ENABLE)
    /* SLEEP_WAKEUP stores its emitter into g_ble_lll_sleep_env_p, which the
     * controller only allocates when BLE sleep is enabled. Calling it without
     * sleep faults (store through a NULL env), so it is gated here. */
    sleep_stack_enableWakeupVsEvent(true); /* LE meta 0xC3 */
#endif
    ESP_LOGI(TAG, "vendor-specific HCI events enabled");
}

#else /* ESP-IDF too old to expose the VS enable API */

void bt_unchained_init(void)
{
    ESP_LOGW(TAG, "this ESP-IDF does not expose the VS enable API (v6.0+ needed); "
                  "vendor-specific HCI commands left disabled");
}

#endif
