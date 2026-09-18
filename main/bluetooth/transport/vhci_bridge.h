/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * A generic bridge between the controller virtual HCI interface and
 * a byte stream interface represented as a write and read function pair.
 */

#ifndef VHCI_BRIDGE_H
#define VHCI_BRIDGE_H

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

/*
 * Byte stream representation.
 * A byte stream name and a read and write function pointer pair.
 *
 * The read() function must block until at least one byte is there and return
 * up to len bytes. Returning <= 0 is treated as no data was available and will
 * simply be retried.
 *
 * The write() is blocking and reports how much it took.
 */
typedef struct
{
    const char *name;
    int (*read)(uint8_t *buf, size_t len);
    int (*write)(const uint8_t *buf, size_t len);
} vhci_bridge_io_t;

/*
 * Attach the bridge to the controller and start pumping data.
 * Call once, after the transport has its driver up.
 * The io structure must outlive the call.
 */
esp_err_t vhci_bridge_start(const vhci_bridge_io_t *io);

#endif /* VHCI_BRIDGE_H */
