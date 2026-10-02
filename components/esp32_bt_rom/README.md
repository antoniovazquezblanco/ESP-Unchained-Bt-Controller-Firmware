# ESP32 BT ROM

A model of the ESP32 Bluetooth controller: the ROM functions, the radio
registers and the exchange-memory structures it works through. Header-only
except for the base pointers; nothing here changes controller behaviour, it only
describes.

## Layout

```
include/
  esp32_bt_rom.h    aggregate header + the component's own helpers
  hal/      Hardware (memory-mapped)
    hal.h           aggregate: the ble and em base pointers
    types.h         shared types (bdaddr, iv)
    core/     radio core registers (base 0x3ff71200)
      core_ble.h / core_ble_reg.h
    em/       exchange memory (base 0x3ffb0000)
      em.h, em_common*.h             memory map
      em_ble.h                       the BLE region
      em_ble_cs*.h                   control structures (one per link; HOPCNTL, etc.)
      em_ble_rxdesc*.h / txdesc*.h   RX / TX descriptors
      em_ble_rxbuff.h / txbuff.h     RX / TX buffers
      em_ble_enc.h / ral*.h / white.h  encryption, resolving list, whitelist
  rom/      ROM software: function tables and the structures they work on
    rom.h           aggregate of the rom/ headers
    r_ip_funcs.h / r_modules_funcs.h   the ROM function-pointer tables (741 / 104)
    co.h            intrusive lists
    em.h            software buffer environment (em_buf_env, descriptors)
    ea.h            event arbiter elements
    ke.h            kernel message types
    hli.h           high-level interrupt lock
    lld.h           link-layer driver handles (LLD_ADV_HDL)
    lld_evt.h       link-layer driver event structure
    bt_em_buf.h     BR/EDR LMP TX buffer element
    hci_desc_tabs.h HCI command/event descriptor tables
    ld_env.h        BR/EDR link-driver environment
    lmp_desc_tab.h  BR/EDR LMP PDU format tables
```

`hal/` holds the memory-mapped hardware: the radio core registers and the
exchange-memory layout the radio reads. `rom/` holds the ROM software: the two
function-pointer tables plus the data structures the ROM keeps in RAM — the BLE
glue imported from the controller model, and the BR/EDR LMP / HCI pieces the BLE
model does not cover. Everything is named at the bitfield level, cross-checked
against the ROM disassembly and verified on real hardware. The consumers
(scan_pin, lmp_monitor, ...) build on this model.

The `ble` and `em` pointers (in `hal/hal.h`) give structured access to the two
regions, e.g. `em->ble.cs.elt[LLD_ADV_HDL].hopcntl` or `ble->advchmap`.
`hal/hal.h` and `rom/rom.h` are umbrella headers for each half, and
`esp32_bt_rom.h` pulls in both.
