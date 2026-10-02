/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Unlocks the ESP32-C3 controller: our own vendor-specific commands plus the
 * stock Espressif ones.
 *
 * The C3 controller is RivieraWaves, reached through the same writable
 * function-pointer tables as the classic ESP32 (r_ip_funcs_p / r_modules_funcs_p
 * from esp32c3.rom.ld). We hook the HCI command ingress slot to answer our own
 * vendor group (Company 0xF00D: INFO, SUPPORTED_CMDS, SET_BDADDR) and chain the
 * controller for everything else.
 *
 * Separately, the precompiled controller (libbtdm_app.a) registers its own
 * vendor-specific commands only when the matching enable function is called;
 * ESP-IDF wraps those in `#ifdef CONFIG_BT_BLUEDROID_ENABLED /
 * CONFIG_BT_NIMBLE_ENABLED`, so a controller-only build leaves them dormant. We
 * call them ourselves.
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

#include "esp32c3_bt_rom.h"
#include "vsc.h"

static const char *TAG = "UNCHAINED";

/* Identifiable company id stamped over the controller default (0x02E5). */
#define UNCHAINED_COMPID 0xF00D

/* The controller HCI command ingress we replace (r_ip_funcs slot +0x2c), saved
 * so we can chain it for every command that is not ours. */
static r_hci_cmd_received_fn_t s_orig_hci_cmd_received;

/* The command-descriptor lookup we replace (r_ip_funcs slot +0x90), saved so we
 * can chain it for every opcode that is not ours. */
static r_hci_look_for_cmd_desc_fn_t s_orig_look_for_cmd_desc;

/* Our replacement for the HCI command ingress: our vendor group is ours, every
 * other command stays with the controller. */
static void unchained_hci_cmd_received(uint16_t opcode, uint16_t param_len, uint8_t *param_buf)
{
    if (vsc_owns(opcode)) {
        /* Consume the host command credit the controller would have consumed in
         * r_hci_cmd_received; our Command Complete returns one in hci_tx_start,
         * and an unmatched return trips the hci_tl.c:1008 assert. */
        hci_tl_env.nb_h2c_cmd_pkts--;
        vsc_cmd_received(opcode, param_len, param_buf);
        return;
    }
    s_orig_hci_cmd_received(opcode, param_len, param_buf);
}

/* Our replacement for the command-descriptor lookup: the controller packs our
 * Command Complete with our status intact instead of stamping 0x01 over it. */
static void *unchained_look_for_cmd_desc(uint16_t opcode)
{
    esp32c3_hci_cmd_desc_t *desc = vsc_cmd_desc(opcode);
    if (desc != NULL)
        return desc;
    return s_orig_look_for_cmd_desc(opcode);
}

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

/* Turn on the controller's own vendor-specific commands. */
static void enable_stock_vs(void)
{
    bt_stack_enableEchoVsCmd(true);              /* 0xFC81 ECHO */
    advFilter_stack_enableDupExcListVsCmd(true); /* 0xFD08 CONFIG_DUP_EXC_LIST */
    scan_stack_enableAdvFlowCtrlVsCmd(true);     /* 0xFD09/0xFD0A ADV report flow ctrl */
    adv_stack_enableClearLegacyAdvVsCmd(true);   /* 0xFD0C CLR_LEGACY_ADV */
    chanSel_stack_enableSetCsaVsCmd(true);       /* 0xFD12 ENABLE_CSA2 */
    esp_ble_internalTestFeaturesEnable(true);    /* 0xFD13 CFG_TEST_RELATED */
    ESP_LOGI(TAG, "stock vendor-specific HCI commands enabled");
}

#endif /* HAVE_VS_ENABLE */

void bt_unchained_init(void)
{
    /* Validate the controller dispatch tables are available. */
    if (r_ip_funcs_p == NULL || r_modules_funcs_p == NULL) {
        ESP_LOGE(TAG, "controller dispatch tables not available");
        return;
    }

    /* Hook the HCI command ingress so our vendor group is answered here. */
    s_orig_hci_cmd_received = r_ip_funcs_p->hci_cmd_received;
    r_ip_funcs_p->hci_cmd_received = &unchained_hci_cmd_received;

    /* Hook the descriptor lookup so our replies keep their status byte. */
    s_orig_look_for_cmd_desc = r_ip_funcs_p->hci_look_for_cmd_desc;
    r_ip_funcs_p->hci_look_for_cmd_desc = &unchained_look_for_cmd_desc;

    /* Re-brand the controller with an identifiable company id. */
    sdk_cfg_priv_opts.company_id = UNCHAINED_COMPID;
    ESP_LOGI(TAG, "unchained vendor commands installed, company id 0x%04X", UNCHAINED_COMPID);

#ifdef HAVE_VS_ENABLE
    enable_stock_vs();
#else
    ESP_LOGW(TAG, "this ESP-IDF does not expose the stock VS enable API (v6.0+ needed); "
                  "stock vendor-specific commands left disabled");
#endif

    ESP_LOGI(TAG, "esp32c3_bt_rom selftest: %s",
             esp32c3_bt_rom_selftest() ? "ok" : "FAILED");
}
