ESP Unchained Bluetooth Controller - firmware package
=====================================================

This package installs the firmware onto your device.
It bundles everything needed you do NOT need Python, ESP-IDF or any driver toolchain.

How to flash
------------

  Windows      Double-click       flash.bat
  Linux/macOS  Run in a terminal  ./flash.sh

Connect only the board you want to flash before starting.
The flasher detects the serial port automatically.

If you have several serial devices connected and detection picks the wrong one, pass the port explicitly:

  Windows      flash.bat --port COM5
  Linux        ./flash.sh --port /dev/ttyUSB0
  macOS        ./flash.sh --port /dev/cu.usbserial-XXXX

Notes
-----

* Flashing installs a complete firmware image, so any settings previously stored on the device are reset to defaults.

More information at https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware
