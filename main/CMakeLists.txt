idf_component_register(SRCS "main.c"
                            "ui/display.c" "ui/font.c" "ui/gfx.c" "ui/console.c" "ui/ui.c"
                            "bluetooth/bluetooth.c" "bluetooth/vsc.c" "bluetooth/hci_usb.c"
                       PRIV_REQUIRES bt nvs_flash esp_driver_uart esp_driver_usb_serial_jtag
                                     esp_lcd esp_driver_spi esp_driver_gpio
                       INCLUDE_DIRS "." "ui" "bluetooth"
                       REQUIRED_IDF_TARGETS esp32c5 esp32c3)
