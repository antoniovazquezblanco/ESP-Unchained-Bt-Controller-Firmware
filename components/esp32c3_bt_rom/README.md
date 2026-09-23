# ESP32-C3 BT ROM

Typed declarations for the ESP32-C3 BLE controller's ROM functions (the
RivieraWaves LE stack shipped in mask ROM).

The symbols are exported by ESP-IDF's `esp32c3.rom.bt_funcs.ld` (linked whenever
the controller runs from IRAM, i.e. not `CONFIG_BT_CTRL_RUN_IN_FLASH_ONLY`), so
this component only declares typed prototypes for the subset the unchained layer
needs.