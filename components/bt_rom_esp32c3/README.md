# ESP32-C3 BT ROM

A model of the ESP32-C3 BLE controller (the RivieraWaves LE stack in mask ROM
plus the `libbtdm_app.a` glue). The C3 reaches the controller through the same
writable function-pointer tables as the classic ESP32, exported as absolute
symbols by `esp32c3.rom.ld`, and works through exchange memory, so this component
models the slices the unchained layer hooks or calls rather than reimplementing
anything. Split like the classic `bt_rom_esp32`: `hal/` for the memory-mapped
hardware, `rom/` for the ROM software.

## Layout

```
include/
  bt_rom_esp32c3.h   aggregate header + selftest
  hal/      Hardware (memory-mapped)
    hal.h           aggregate of the hal/ headers
    core/
      core_ble.h    BLE core registers at base 0x60031000 (same RivieraWaves core
                    as the classic): control, interrupts, the base/fine-time clock
                    (basetimecnt/finetimecnt), diag and error status
    em.h            exchange memory: the control structure (em_ble_cs: access
                    address, CRC init, hop control, channel map), RX/TX
                    descriptors, region ids and strides
  rom/      ROM software: function tables and the structures they work on
    rom.h           aggregate of the rom/ headers
    ip_funcs.h      the dispatch tables (the r_ip_funcs analog):
                      r_ip_funcs_p      +0x2c hci_cmd_received, +0x8c hci_send_2_host,
                                        +0x90 look_for_cmd_desc, +0x98 look_for_evt_desc,
                                        +0x264 lld_read_clock, +0x330 lld_con_data_tx,
                                        +0x368 lld_con_llcp_tx, +0x394 lld_con_rx_llcp_check,
                                        +0x430 lld_scan_sched
                      r_modules_funcs_p +0xc8 ke_msg_alloc, +0xe0 ke_msg_send
                      r_plf_funcs_p     +0xbc em_buf_get (exchange-memory address)
    hci.h           HCI: cmd/evt/vendor descriptors, hci_tl_env (command credit),
                    the CC/EVT ke_msg ids, and the r_hci_* ROM entry points
    lld.h           link-layer envs: p_llm_env (public BD_ADDR), p_lld_env (rx idx),
                    lld_tx_elem_t (queued outgoing-PDU element), lld_scan_env /
                    lld_scan_sub_env_t (scan activities, cs_idx @ +0x38)
    sdk_config.h    sdk_cfg_priv_opts (company_id @ +0x34)
```

Offsets are verified against the `ip_funcs.o` / `modules_funcs.o` relocations and
the rev3 ROM disassembly, and asserted at compile time with `_Static_assert`.
`bt_unchained_esp32c3` builds on this to install the custom vendor commands,
SET_BDADDR, the LL traffic monitor and the single-channel scan pin; see
`reversing/esp32c3-bt-rom.md` for the full map.
