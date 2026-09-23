<div align="center">

<img src="doc/img/logo.png" alt="Bluetooth Unchained" width="640">

# ESP Unchained Bluetooth Controller Firmware

[![Build](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/build.yml/badge.svg)](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/build.yml)
[![CodeQL](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/codeql.yml/badge.svg)](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/codeql.yml)
[![OpenSSF Scorecard](https://api.securityscorecards.dev/projects/github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/badge)](https://securityscorecards.dev/viewer/?uri=github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.md)

ESP-IDF firmware that turns an Espressif chip into a Bluetooth HCI controller reachable over USB or UART with advanced features unlocked through vendor-specific HCI opcodes.

</div>

## Supported hardware

| Board                                          |                                        Image                                         |  Target   | Transport                        |    Display     |
| :--------------------------------------------- | :----------------------------------------------------------------------------------: | :-------: | :------------------------------- | :------------: |
| **ScapyCon 2026 badge**                        |  <img src="doc/img/board_scapycon2026.png" alt="SCAPYCON 2026 badge" height="120">   | `esp32c5` | Native USB-Serial/JTAG           | ST7789 320×240 |
| **ESP32-C3 SuperMini**                         | <img src="doc/img/board_esp32c3supermini.png" alt="ESP32-C3 SuperMini" height="120"> | `esp32c3` | Native USB-Serial/JTAG           |       -        |
| **ESP32-DevKitC-32E** / **ESP32-DevKitC-32UE** |  <img src="doc/img/board_esp32devkitc32.png" alt="ESP32-DevKitC-32E" height="120">   |  `esp32`  | UART0 @ 921600 (USB-UART bridge) |       -        |

## Features

|  Target   |    Radio    | Core ver. |         VSC         | BDADDR | RX ADV CH | LMP Mon | LL Mon |
| :-------: | :---------: | :-------: | :-----------------: | :----: | :-------: | :-----: | :----: |
|  `esp32`  | BR/EDR + LE |    4.2    |     **Custom**      |   ✓    |           |         |        |
| `esp32c3` |     LE      |    5.0    | **Stock Espressif** |        |           |         |        |
| `esp32c5` |     LE      |    6.0    | **Stock Espressif** |        |           |         |        |

## Documentation

* [Usage](doc/usage.md)
* [Building](doc/development/building.md)
