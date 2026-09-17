<div align="center">

<img src="doc/img/logo.png" alt="Bluetooth Unchained" width="640">

# ESP Unchained Bluetooth Controller Firmware

[![Build](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/build.yml/badge.svg)](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/actions/workflows/build.yml)

ESP-IDF firmware that turns an Espressif chip into a Bluetooth HCI controller reachable over USB or UART with advanced features unlocked through vendor-specific HCI opcodes.

</div>

## Supported hardware

| Board                   |                                        Image                                         |  Target   | Transport              |    Display     |
| :---------------------- | :----------------------------------------------------------------------------------: | :-------: | :--------------------- | :------------: |
| **ScapyCon 2026 badge** |  <img src="doc/img/board_scapycon2026.jpg" alt="SCAPYCON 2026 badge" height="120">   | `esp32c5` | Native USB-Serial/JTAG | ST7789 320×240 |
| **ESP32-C3 SuperMini**  | <img src="doc/img/board_esp32c3supermini.jpg" alt="ESP32-C3 SuperMini" height="120"> | `esp32c3` | Native USB-Serial/JTAG |       -        |


## Documentation

* [Building](doc/development/building.md)
