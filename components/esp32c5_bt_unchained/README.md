# ESP32-C5 BT Unchained

Unlocks the ESP32-C5 Bluetooth controller with our own custom vendor command set,
a link traffic monitor and a scan-channel pin.

## Features

- **Custom vendor commands** (`vsc.c`, Company 0xF00D), head-inserted into the
  controller's vendor-command list: `INFO` (0xFC00), `SUPPORTED_CMDS` (0xFC01),
  `SET_BDADDR` (0xFC02), `SET_TRAFFIC_MONITOR` (0xFC03), `SET_SCAN_CHANNEL`
  (0xFC04). The opcodes and the 0xFF capture-event layout match the classic ESP32
  and the C3, so one host tool drives every target.
- **LL traffic monitor** (`traffic_monitor.c`): reports controller LL PDUs (data
  and control, both directions) to the host as 0xFF vendor events. The controller
  (`libble_app.a`) is statically linked with no writable dispatch table, so the
  taps are installed at **link time** with `-Wl,--wrap` (see `CMakeLists.txt`):
  `r_ble_ll_conn_rx_data_pdu` for RX and `r_ble_lll_conn_append_tx_buffer` for TX.
  Each `__wrap_` captures the PDU, then tail-calls the real `__real_` function.
- **Scan-channel pin** (`scan_pin.c`): pins advertising reception to one primary
  channel (37/38/39), or restores hopping. It writes the controller's own
  forced-scan-channel byte (`ble_ll_env_p + 0x38`) -- the exact mechanism the
  controller's built-in SET_SCAN_CHAN QA command uses -- so no hook is needed.

All of the above is hardware-validated on the C5 (COM11) against a C3 peer; see
`reversing/esp32c5-bt-rom.md` for the full reversing map.
