/**
 * @file kernel/event.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MEREVENTTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __KERNEL_EVENT_H__
#define __KERNEL_EVENT_H__

#include "simba.h"

/* Event. */
struct event_t {
    struct chan_t base;
    uint32_t mask;
};

/**
 * Initialize given event channel.
 * @param[in] event Event channel to initialize.
 * @return zero(0) or negative error code
 */
int event_init(struct event_t *event_p);

/**
 * Wait for a event in given mask to occur.
 * @param[in] event Event channel to read from.
 * @param[in] buf_p The mask of events to wait for. When the function
 *                   return the mask contains the events that occured.
 * @param[in] size Size to read (always sizeof(mask).
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_read(struct event_t *event_p, void *buf_p, size_t size);

/**
 * Write given events to the event channel.
 * @param[in] event Event channel to write to.
 * @param[in] buf The mask of events to write.
 * @param[in] size Always sizeof(mask).
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write(struct event_t *event_p, const void *buf_p, size_t size);

/**
 * Write given events to the event channel from interrupt context.
 * @param[in] event Event channel to write to.
 * @param[in] buf The mask of events to write.
 * @param[in] size Always sizeof(mask).
 * @return sizeof(mask) or negative error code.
 */
ssize_t event_write_irq(struct event_t *event_p,
                        const void *buf_p,
                        size_t size);

/**
 * Get the number of bytes currently stored in the event. May return
 * less bytes than number of bytes stored in the channel.
 * @param[in] event Event.
 * @return Number of bytes in event.
 */
ssize_t event_size(struct event_t *event_p);

#endif