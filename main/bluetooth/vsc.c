/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Espressif vendor-specific (OGF 0x3F) HCI command enablement.
 *
 * The precompiled controller (libble_app.a / libbtdm_app.a) registers its
 * vendor-specific commands only when the matching enable function is called.
 * ESP-IDF wraps those calls in `#ifdef CONFIG_BT_BLUEDROID_ENABLED /
 * CONFIG_BT_NIMBLE_ENABLED`, so a controller-only build leaves the whole VS set
 * dormant (every 0x3F opcode answers 0x01 "Unknown HCI Command"). We call them
 * ourselves. Every symbol was verified with `nm` over each chip's controller
 * library, not from the headers. Requires ESP-IDF v6.0+ (or a VS-enable
 * backport); we feature-detect on esp_bt_vs.h to keep compiling on older IDF.
 *
 * The enablers run in one of two phases -- vsc_enable_pre() (before
 * esp_bt_controller_enable()) or vsc_enable_post() (after) -- because the point
 * at which each chip accepts them differs. See vsc.h. Background and the live
 * results are in vendors/espressif/esp-hci-uart-vsc-enablement.md.
 */
#include <stdbool.h>

#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "VSC";

/* Each chip gets its own symbol set, gated on the target rather than on the
 * header: every port ships esp_bt_vs.h, but the symbols behind it differ, and a
 * chip's header can be wrong about them (the C3 header documents its dup-exc
 * enabler as `advFilter_stack_eanbleDupExcListCmd`, which does not exist -- the
 * library exports `advFilter_stack_enableDupExcListVsCmd`). The lists below were
 * taken from `nm` over each chip's controller library, not from the headers.
 *
 * Gating on the target also matters for linking: without it a build works only
 * while the enabler stays uncalled -- --gc-sections drops it -- and breaks with a
 * wall of undefined references the moment it is called. */
#if defined(__has_include) && __has_include("esp_bt_vs.h")
#  if defined(CONFIG_IDF_TARGET_ESP32)
#    define VSC_SYMBOLS_ESP32 1
#  elif defined(CONFIG_IDF_TARGET_ESP32C5)
#    define VSC_SYMBOLS_C5 1
#  elif defined(CONFIG_IDF_TARGET_ESP32C3)
#    define VSC_SYMBOLS_C3 1
#  endif
#endif

#ifdef VSC_SYMBOLS_ESP32
/* Classic ESP32, dual mode (libbtdm_app.a). Its enablers register into the
 * external-HCI command-descriptor table, built during controller init and frozen
 * at enable(), so they go in vsc_enable_pre() -- BEFORE enable(). That ordering
 * is what makes the classic-BT AFH / TX-power blocks (0xFD81-0xFD99) reachable:
 * 10 -> 23 opcodes over external HCI. (0xFD13 CFG_TEST_RELATED is enabled too but
 * stays internal-host-only by construction, so it never answers on external
 * HCI.) The controller exports no VS *event* enablers. */
extern void bt_stack_enableEchoVsCmd(bool en);
extern void bt_stack_enableCoexVsCmd(bool en);
extern void bt_stack_enableSecCtrlVsCmd(bool en);
extern void bt_stack_enablePwrCtrlVsCmd(bool en);
extern void bt_stack_enableAfhVsCmd(bool en);
extern void bt_stack_enableBasicVsCmd(bool en);
extern void bt_stack_enableClkCtrlVsCmd(bool en);
extern void bt_stack_enablePktCtrlVsCmd(bool en);
extern void bt_stack_enableRateCtrlVsCmd(bool en);
extern void scan_stack_enableAdvFlowCtrlVsCmd(bool en);
extern void adv_stack_enableClearLegacyAdvVsCmd(bool en);
extern void advFilter_stack_enableDupExcListVsCmd(bool en);
extern void arr_stack_enableMultiConnVsCmd(bool en);
extern void esp_ble_internalTestFeaturesEnable(bool en);

void vsc_enable_pre(void)
{
    bt_stack_enableEchoVsCmd(true);                   /* 0xFC81 ECHO */
    bt_stack_enableCoexVsCmd(true);                   /* 0xFC82 SET_COEX_STATUS */
    bt_stack_enableSecCtrlVsCmd(true);                /* 0xFD82 SET_MIN_ENC_KEY_SIZE */
    bt_stack_enablePwrCtrlVsCmd(true);                /* 0xFD91-0xFD99 RD/WR TX power + RSSI */
    bt_stack_enableAfhVsCmd(true);                    /* 0xFD87/0xFD89/0xFD8A AFH */
    bt_stack_enableBasicVsCmd(true);                  /* 0xFD88 SET_EVT_MASK */
    bt_stack_enableClkCtrlVsCmd(true);                /* 0xFD83 CLK_UPDATE */
    bt_stack_enablePktCtrlVsCmd(true);                /* 0xFD81 WR_DM1_ENABLE */
    bt_stack_enableRateCtrlVsCmd(true);               /* 0xFD8B WR_AUTO_RATE_INIT */
    scan_stack_enableAdvFlowCtrlVsCmd(true);          /* 0xFD09/0xFD0A */
    adv_stack_enableClearLegacyAdvVsCmd(true);        /* 0xFD0C */
    advFilter_stack_enableDupExcListVsCmd(true);      /* 0xFD08 */
    arr_stack_enableMultiConnVsCmd(true);             /* 0xFD0F/0xFD10 */
    esp_ble_internalTestFeaturesEnable(true);         /* 0xFD13 CFG_TEST_RELATED (internal only) */
    ESP_LOGI(TAG, "vendor-specific HCI commands enabled");
}

void vsc_enable_post(void) { }

#elif defined(VSC_SYMBOLS_C5)
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

void vsc_enable_pre(void) { }

void vsc_enable_post(void)
{
    /* Commands. */
    advFilter_stack_enableDupExcListVsCmd(true);      /* 0xFD08/0xFD0D/0xFD0E */
    scan_stack_enableAdvFlowCtrlVsCmd(true);          /* 0xFD09/0xFD0A (+ ADV_LOST 0xF0) */
    adv_stack_enableClearLegacyAdvVsCmd(true);        /* 0xFD0C */
    arr_stack_enableMultiConnVsCmd(true);             /* 0xFD0F/0xFD10 */
    pcl_stack_enableSetRssiThreshVsCmd(true);         /* 0xFD11 */
    chanSel_stack_enableSetCsaVsCmd(true);            /* 0xFD12 */
    log_stack_enableLogsRelatedVsCmd(true);           /* 0xFD14 */
    hci_stack_enableSetVsEvtMaskVsCmd(true);          /* 0xFD16 */
    winWiden_stack_enableSetConstPeerScaVsCmd(true);  /* 0xFD18 */
    esp_ble_internalTestFeaturesEnable(true);         /* 0xFD13 CFG_TEST_RELATED */

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
    adv_stack_enableScanReqRxdVsEvent(true);          /* LE meta 0xC0 */
    conn_stack_enableChanMapUpdCompVsEvent(true);     /* LE meta 0xC1 */
#if defined(CONFIG_BT_LE_SLEEP_ENABLE)
    /* SLEEP_WAKEUP stores its emitter into g_ble_lll_sleep_env_p, which the
     * controller only allocates when BLE sleep is enabled. Calling it without
     * sleep faults (store through a NULL env), so it is gated here. */
    sleep_stack_enableWakeupVsEvent(true);            /* LE meta 0xC3 */
#endif
    ESP_LOGI(TAG, "vendor-specific HCI events enabled");
}

#elif defined(VSC_SYMBOLS_C3)
/* The C3's controller (libbtdm_app.a) exports far fewer enablers than the C5's:
 * six, and no vendor-specific *event* enablers at all. Verified with nm. */
extern void bt_stack_enableEchoVsCmd(bool en);
extern void advFilter_stack_enableDupExcListVsCmd(bool en);
extern void scan_stack_enableAdvFlowCtrlVsCmd(bool en);
extern void adv_stack_enableClearLegacyAdvVsCmd(bool en);
extern void chanSel_stack_enableSetCsaVsCmd(bool en);
extern void esp_ble_internalTestFeaturesEnable(bool en);

void vsc_enable_pre(void) { }

void vsc_enable_post(void)
{
    bt_stack_enableEchoVsCmd(true);                   /* 0xFC81 ECHO */
    advFilter_stack_enableDupExcListVsCmd(true);      /* 0xFD08 CONFIG_DUP_EXC_LIST */
    scan_stack_enableAdvFlowCtrlVsCmd(true);          /* 0xFD09/0xFD0A ADV report flow ctrl */
    adv_stack_enableClearLegacyAdvVsCmd(true);        /* 0xFD0C CLR_LEGACY_ADV */
    chanSel_stack_enableSetCsaVsCmd(true);            /* 0xFD12 ENABLE_CSA2 */
    esp_ble_internalTestFeaturesEnable(true);         /* 0xFD13 CFG_TEST_RELATED */
    ESP_LOGI(TAG, "vendor-specific HCI commands enabled");
}

#else /* chip without a verified VS enable API, or pre-fix IDF */
void vsc_enable_pre(void) { }

void vsc_enable_post(void)
{
    ESP_LOGW(TAG, "no verified VS enable symbols for this chip (ESP32, ESP32-C5 "
                  "and ESP32-C3 are covered, on IDF v6.0+); vendor-specific HCI "
                  "commands left disabled");
}
#endif
