/**
 * @file drivers/pin.h
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_PIN_H__
#define __DRIVERS_PIN_H__

#include "simba.h"

/**
 * Configure the pin as an output pin.
 */
#define PIN_OUTPUT 0

/**
 * Configure the pin as an input pin.
 */
#define PIN_INPUT  1

#include "pin_port.h"

extern struct pin_device_t pin_device[PIN_DEVICE_MAX];

/**
 * Initialize the pin module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int pin_module_init(void);

/**
 * Initialize given driver object with given device and mode.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] dev_p Device to use.
 * @param[in] mode Pin mode. One of ``PIN_INPUT`` or ``PIN_OUTPUT``.
 *
 * @return zero(0) or negative error code.
 */
int pin_init(struct pin_driver_t *self_p,
             struct pin_device_t *dev_p,
             int mode);

/**
 * Write given value to given pin.
 *
 * @param[in] self_p Driver object.
 * @param[in] value ``1`` for high and ``0`` for low output.
 *
 * @return zero(0) or negative error code.
 */
int pin_write(struct pin_driver_t *self_p, int value);

/**
 * Read the current value of given pin.
 *
 * @param[in] self_p Driver object.
 *
 * @return ``1`` for high and ``0`` for low input, otherwise negative
 *         error code.
 */
int pin_read(struct pin_driver_t *self_p);

/**
 * Toggle the pin output value (high/low).
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int pin_toggle(struct pin_driver_t *self_p);

/**
 * Set the pin mode of given pin.
 *
 * @param[in] self_p Driver object.
 * @param[in] mode New pin mode.
 *
 * @return zero(0) or negative error code.
 */
int pin_set_mode(struct pin_driver_t *self_p, int mode);

/**
 * Pin device mode to set. One of ``PIN_INPUT`` or ``PIN_OUTPUT``.
 *
 * @param[in] self_p Pin device.
 * @param[in] mode New pin mode.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_set_mode(const struct pin_device_t *dev_p,
                                      int mode)
{
    return (pin_port_device_set_mode(dev_p, mode));
}

/**
 * Read the value of given pin device.
 *
 * @param[in] self_p Pin device.
 *
 * @return ``1`` for high and ``0`` for low input, otherwise negative
 *         error code.
 */
static inline int pin_device_read(const struct pin_device_t *dev_p)
{
    return (pin_port_device_read(dev_p));
}

/**
 * Write high to given pin device.
 *
 * @param[in] self_p Pin device.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_write_high(const struct pin_device_t *dev_p)
{
    return (pin_port_device_write_high(dev_p));
}

/**
 * Write low to given pin device.
 *
 * @param[in] self_p Pin device.
 *
 * @return zero(0) or negative error code.
 */
static inline int pin_device_write_low(const struct pin_device_t *dev_p)
{
    return (pin_port_device_write_low(dev_p));
}

#endif
