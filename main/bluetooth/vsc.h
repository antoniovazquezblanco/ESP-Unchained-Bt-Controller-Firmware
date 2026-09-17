/*
 * Espressif vendor-specific (OGF 0x3F) HCI command enablement.
 */
#ifndef VSC_H
#define VSC_H

/*
 * Register Espressif's vendor-specific HCI commands/events with the controller.
 * The enablers must run at the point the controller makes its command tables /
 * link-layer envs writable, and that point differs by chip -- so this is split
 * into two hooks the controller bring-up calls at the right moments:
 *
 *   vsc_enable_pre()   between esp_bt_controller_init() and _enable().
 *                      The classic ESP32 registers here: its enablers write the
 *                      external-HCI command-descriptor table, which is frozen at
 *                      enable(), so calling before enable() is what makes its
 *                      AFH / TX-power VS blocks reachable.
 *   vsc_enable_post()  after esp_bt_controller_enable().
 *                      The C3/C5 register here: the envs their enablers write into
 *                      are allocated by init()/enable().
 *
 * Each is a no-op on chips that don't use that phase. No-op with a warning on an
 * ESP-IDF too old to expose the enable API.
 */
void vsc_enable_pre(void);
void vsc_enable_post(void);

#endif /* VSC_H */
