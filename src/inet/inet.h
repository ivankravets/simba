/**
 * @file inet/inet.h
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_INET_H__
#define __INET_INET_H__

#include "simba.h"

struct inet_ip_addr_t {
    /** IPv4 address. */
    uint32_t number;
};

struct inet_addr_t {
    /** IPv4 address. */
    struct inet_ip_addr_t ip;
    /** Port. */
    uint16_t port;
};

/**
 * Initialize the inet module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int inet_module_init(void);

/**
 * Convert the Internet host address src_p from the IPv4
 * numbers-and-dots notation into binary form (in network byte order)
 * and stores it in the structure that dst_p points to.
 *
 * The address supplied in src_p can have one of the following forms:
 *
 * - a.b.c.d Each of the four numeric parts specifies a byte of the
 *           address; the bytes are assigned in left-to-right order to
 *           produce the binary address.
 *
 * @param[in] src_p Address a.b.c.d to convert into a number.
 * @param[out] dst_p Converted address.
 *
 * @return zero(0) or negative error code.
 */
int inet_aton(const char *src_p,
              struct inet_ip_addr_t *dst_p);

/**
 * Convert the Internet host src_p from the IPv4 binary form (in
 * network byte order) to numbers-and-dots notation and stores it in
 * the structure that dst_p points to.
 *
 * @param[in] src_p Address to convert into a string.
 * @param[out] dst_p Converted address as a string.
 *
 * @return Converted address pointer or NULL on failure.
 */
char *inet_ntoa(const struct inet_ip_addr_t *src_p,
                char *dst_p);

/**
 * Calculate the internet checksum of given buffer.
 *
 * @param[in] buf_p Buffer to calculate the chechsum of.
 * @param[in] size Size of the buffer.
 *
 * @return Calculated checksum.
 */
uint16_t inet_checksum(void *buf_p, size_t size);

#endif
