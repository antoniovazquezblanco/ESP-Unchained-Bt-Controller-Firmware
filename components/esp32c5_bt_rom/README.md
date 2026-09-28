# ESP32-C5 BT ROM

Placeholder for ESP32-C5 BLE controller definitions.

Unlike the ESP32 and C3, the **C5 has no Bluetooth code in ROM** — the controller
is a NimBLE-lineage stack shipped entirely in `libble_app.a`. There is therefore
no ROM symbol table to declare here. This component is reserved for future C5
controller glue; for now it only exposes the symbols needed to change the
controller's Company Identifier (`priv_config_opts_ptr`, offset 0).