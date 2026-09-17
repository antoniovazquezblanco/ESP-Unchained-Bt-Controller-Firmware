# Building

The firmware is built with ESP-IDF v6.1.

## Configure

Set a target and, optionally, a board definition if there is one for your
hardware.

```
# Set a supported target (esp32c5, esp32c3 or esp32).
idf.py set-target esp32c5

# If your hardware has a board definition under boards/, select it as well.
idf.py -DBOARD=scapycon2026badge set-target esp32c5
```

Available boards:

| Board | Target | Notes |
| --- | --- | --- |
| `scapycon2026badge` | `esp32c5` | SCAPYCON 2026 badge; ST7789 display |
| `esp32c3supermini` | `esp32c3` | ESP32-C3 Super Mini; no display |
| `esp32devkitc` | `esp32` | ESP32-DevKitC (WROOM-32E / -32UE); dual-mode BR/EDR + BLE; HCI over UART0, console off |

Building with no board selected is a compile check: it produces a controller
with no HCI transport, which runs but cannot be talked to.

Note that `set-target` runs `fullclean` first, so it deletes `build/` and any
binaries in it.

## Build

```
idf.py build
```

## Flash

```
idf.py -p <port> flash
```
