/*
    Copyright 2016,2019,2021 (C) Alexey Dynda

    This file is part of Tiny Protocol Library.

    Protocol Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Protocol Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Protocol Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

#ifndef CONFIG_ENABLE_CHECKSUM
#define CONFIG_ENABLE_CHECKSUM
#endif

#ifndef CONFIG_ENABLE_FCS16
#define CONFIG_ENABLE_FCS16
#endif

#ifndef CONFIG_ENABLE_FCS32
#define CONFIG_ENABLE_FCS32
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Mutex type used by Tiny Protocol implementation.
 * The type declaration depends on platform.
 */
typedef pthread_mutex_t tiny_mutex_t;

/**
 * Event groups type used by Tiny Protocol implementation.
 * The type declaration depends on platform.
 */
typedef struct
{
    uint8_t bits;
    uint16_t waiters;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} tiny_events_t;

#endif
