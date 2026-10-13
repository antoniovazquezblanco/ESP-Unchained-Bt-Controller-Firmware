/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Addressable status LED(s): a semantic indicator for the controller.
 *
 * Callers emit events; one task owns the LED and renders them. When the LED is
 * not configured in (CONFIG_UNCHAINED_LED_ENABLED off) the whole API is a no-op,
 * so call sites need no #ifdef.
 */
#ifndef STATUS_LED_H
#define STATUS_LED_H

#include "sdkconfig.h"

/* What just happened, from the LED's point of view. Emitted from any context,
 * including the controller callback, so notify() must stay non-blocking. */
typedef enum
{
    STATUS_LED_READY,   /* controller enabled and HCI transport attached */
    STATUS_LED_HOST_TX, /* host -> controller (HCI command / ACL in)     */
    STATUS_LED_HOST_RX, /* controller -> host (HCI event / ACL out)      */
    STATUS_LED_CAPTURE, /* a 0xFF traffic-monitor event went to the host */
    STATUS_LED_DROP,    /* a packet was dropped (back-pressure / no buf)  */
} status_led_event_t;

#if CONFIG_UNCHAINED_LED_ENABLED

void status_led_init(void);
void status_led_notify(status_led_event_t ev);

#else /* no LED built in: the whole API vanishes */

static inline void status_led_init(void)
{
}
static inline void status_led_notify(status_led_event_t ev)
{
    (void)ev;
}

#endif /* CONFIG_UNCHAINED_LED_ENABLED */

#endif /* STATUS_LED_H */
