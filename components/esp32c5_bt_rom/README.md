# ESP32-C5 BT ROM

Typed declarations for the ESP32-C5 BLE controller. Unlike the ESP32 and C3, the
**C5 has no Bluetooth code in ROM** — the controller is a NimBLE/Mynewt-lineage
stack shipped entirely in `libble_app.a`. So despite the component name this is a
**library model, not a ROM model**: every symbol below resolves at link time by
name, which makes interposition easier than the ROM case.

## Layout

```
include/
  esp32c5_bt_rom.h   aggregate header + the CompId get/set helpers
  ble_ll.h           the controller surface the unchained layer uses:
                       ble_ll_hci_env_p        vendor-command list head @ +0x34
                       ble_ll_hci_vs_cmd_t     list node {ocf, cb, next}
                       r_esp_ble_ll_set_public_addr   public-address setter
  sdk_config.h       priv_config_opts (company_id @ +0, via priv_config_opts_ptr)
```

`esp32c5_bt_unchained` head-inserts static `ble_ll_hci_vs_cmd_t` nodes into the
controller's vendor-command list to add the custom commands, and uses
`r_esp_ble_ll_set_public_addr` for SET_BDADDR. The controller auto-packs the
Command Complete from each handler's return status, so the handlers are thin.
See `reversing/esp32c5-bt-rom.md` for the full map.
