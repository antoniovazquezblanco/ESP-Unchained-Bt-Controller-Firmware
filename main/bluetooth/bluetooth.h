/*
 * BLE controller / HCI transport bring-up.
 */
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_err.h"

/*
 * Bring up the BLE controller and its HCI transport (the transport itself and
 * its pins are selected by Kconfig, see boards/<name>.defaults). Failures are
 * logged here; ESP_OK means the controller is enabled and HCI is reachable.
 */
esp_err_t bluetooth_init(void);

#endif /* BLUETOOTH_H */
