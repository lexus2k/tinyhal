/*
    Copyright 2016-2021 (C) Alexey Dynda

    This file is part of Tiny HAL Library.

    Tiny HAL Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tiny HAL Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tiny HAL Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

/* For fastest version of protocol assign all defines to zero.
 * In this case protocol supports no CRC field, and
 * all api functions become not thread-safe.
 */

#ifndef CONFIG_ENABLE_CHECKSUM
#   define CONFIG_ENABLE_CHECKSUM
#endif

#ifndef CONFIG_ENABLE_FCS16
#   define CONFIG_ENABLE_FCS16
#endif

#ifndef CONFIG_ENABLE_FCS32
#   define CONFIG_ENABLE_FCS32
#endif

/**
 * Mutex type used by Tiny HAL implementation.
 * The type declaration depends on platform.
 */
typedef uintptr_t tiny_mutex_t;

/**
 * Events group type used by Tiny HAL implementation.
 * The type declaration depends on platform.
 */
typedef struct
{
    /** Mutex object to protect bits */
    tiny_mutex_t  mutex;
    /** Current state of bits */
    uint8_t       bits;
} tiny_events_t;


/**
 * Structure of HAL abstraction layer
 */
typedef struct
{
    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*mutex_create)(tiny_mutex_t *mutex);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*mutex_destroy)(tiny_mutex_t *mutex);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    uint8_t (*mutex_try_lock)(tiny_mutex_t *mutex);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*mutex_unlock)(tiny_mutex_t *mutex);

    /**
     * Optional, but remember, default implementation relies on GCC built-in atomic functions
     * and tiny_sleep() implementation
     */
    void (*mutex_lock)(tiny_mutex_t *mutex);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*events_create)(tiny_events_t *events);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*events_destroy)(tiny_events_t *events);

    /**
     * Optional, but remember, default implementation relies on GCC built-in atomic functions
     * and tiny_sleep() implementation
     */
    uint8_t (*events_wait)(tiny_events_t *events, uint8_t bits,
                             uint8_t clear, uint32_t timeout);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    uint8_t (*events_check_int)(tiny_events_t *events, uint8_t bits, uint8_t clear);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*events_set)(tiny_events_t *events, uint8_t bits);

    /** Optional, but remember, default implementation relies on GCC built-in atomic functions */
    void (*events_clear)(tiny_events_t *events, uint8_t bits);

    /** Must have for Full duplex protocol. Default implementation does not do any sleep operation */
    void (*sleep)(uint32_t ms);

    /** Must have for Full duplex protocol. Default implementation does not cound milliseconds */
    uint32_t (*millis)(void);
} tiny_platform_hal_t;

/**
 * Sets custom specific HAL functions.
 * @param hal pointer to HAL functions structure.
 */
extern void tiny_hal_init(tiny_platform_hal_t *hal);
