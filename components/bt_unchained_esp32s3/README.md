# ESP32-S3 BT Unchained

Unlocks the ESP32-S3 Bluetooth controller with our own vendor-specific commands
and link-layer taps.

The S3 controller is RivieraWaves, reached through the same writable
function-pointer tables as the classic ESP32 (`r_ip_funcs_p` / `r_modules_funcs_p`
from `esp32s3.rom.ld`). The S3 runs the same controller build as the C3 (ESP-IDF
maps its BT sources to `esp32c3`), so this backend mirrors `bt_unchained_esp32c3`;
only the ROM addresses (from `esp32s3.rom.ld`) and the memory barrier (Xtensa
`memw`) differ. This component hooks the HCI command ingress to answer our own
vendor group (Company 0xF00D) and chains the controller for everything else.

## Features

- **Custom vendor commands** (`vsc.c`): `INFO` (0xFC00), `SUPPORTED_CMDS` (0xFC01),
  `SET_BDADDR` (0xFC02), `SET_TRAFFIC_MONITOR` (0xFC03), `SET_SCAN_CHANNEL`
  (0xFC04). The opcodes and the 0xFF capture-event layout match the classic ESP32
  and the C5, so one host tool drives every target.
- **LL traffic monitor** (`traffic_monitor.c`): reports controller LL PDUs (data
  and control, both directions) to the host as 0xFF vendor events, by tapping the
  `lld_con_rx_llcp_check` / `lld_con_data_tx` / `lld_con_llcp_tx` dispatch slots.
- **Scan-channel pin** (`scan_pin.c`): pins advertising reception to one primary
  channel (37/38/39), or restores hopping, via the scan control structure's
  hop-control register re-applied from the `lld_scan_sched` slot.

See `reversing/esp32c3-bt-rom.md` for the full C3-family reversing map.
