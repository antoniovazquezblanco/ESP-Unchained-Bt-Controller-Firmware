# Building

The firmware is built with [ESP-IDF v6.1](https://docs.espressif.com/projects/esp-idf/en/v6.1/).
Install and activate it first so that `idf.py` is on your `PATH`.

## Configure

Pick a target, and a board definition if one exists for your hardware.
The board sets the pins, transport and enabled features for a specific device; without one you get a generic build for the bare chip.

```
# Bare target (esp32c5, esp32c3 or esp32).
idf.py set-target esp32c5

# Target plus a board definition from boards/ (recommended).
idf.py -DBOARD=scapycon2026badge set-target esp32c5
```

Available boards:

| Board               | Target    | Notes                                                                                  |
| ------------------- | --------- | -------------------------------------------------------------------------------------- |
| `scapycon2026badge` | `esp32c5` | SCAPYCON 2026 badge; ST7789 display                                                    |
| `esp32c3supermini`  | `esp32c3` | ESP32-C3 Super Mini; no display                                                        |
| `esp32devkitc`      | `esp32`   | ESP32-DevKitC (WROOM-32E / -32UE); dual-mode BR/EDR + BLE; HCI over UART0, console off |

Building with no board selected is a compile check only: it produces a controller with no HCI transport, which runs but cannot be talked to.

`set-target` runs `fullclean` first, so it wipes `build/`.
Run it once when you switch target or board; rebuild with plain `idf.py build` afterwards.

## Build

```
idf.py build
```

## Flash

```
idf.py -p <port> flash
```

`<port>` is the serial device the board enumerates as; see [Usage](usage.md) for how to find it on each operating system.
To flash without a toolchain, use the prebuilt release packages instead (also covered in [Usage](usage.md)).
