/*
 * Espressif vendor-specific (OGF 0x3F) HCI command enablement.
 */
#ifndef VSC_H
#define VSC_H

/*
 * Register Espressif's vendor-specific HCI commands/events with the controller.
 * MUST be called AFTER esp_bt_controller_enable() (the link-layer envs the
 * enablers write into are allocated by controller init/enable). No-op with a
 * warning on an ESP-IDF too old to expose the enable API.
 */
void vsc_enable(void);

#endif /* VSC_H */
