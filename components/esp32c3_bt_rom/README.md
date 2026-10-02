# ESP32-C3 BT ROM

Typed declarations for the ESP32-C3 BLE controller (the RivieraWaves LE stack in
mask ROM plus the `libbtdm_app.a` glue). The C3 reaches the controller through
the same writable function-pointer tables as the classic ESP32, exported as
absolute symbols by `esp32c3.rom.ld`, so this component models the slices the
unchained layer hooks or calls rather than reimplementing anything.

## Layout

```
include/
  esp32c3_bt_rom.h   aggregate header + selftest + the HCI command-descriptor structs
  ip_funcs.h         the dispatch tables (the r_ip_funcs analog):
                       r_ip_funcs_p      +0x2c hci_cmd_received, +0x8c hci_send_2_host,
                                         +0x90 look_for_cmd_desc, +0x98 look_for_evt_desc
                       r_modules_funcs_p +0xc8 ke_msg_alloc, +0xe0 ke_msg_send
                       p_llm_env         +0xc public BD_ADDR
                       hci_tl_env        +0x15 host command credit (nb_h2c_cmd_pkts)
  sdk_config.h       sdk_cfg_priv_opts (company_id @ +0x34)
```

Offsets are verified against the `ip_funcs.o` / `modules_funcs.o` relocations and
the rev3 ROM disassembly, and asserted at compile time with `_Static_assert`.
`esp32c3_bt_unchained` builds on this to install the custom vendor commands and
SET_BDADDR; see `reversing/esp32c3-bt-rom.md` for the full map.
