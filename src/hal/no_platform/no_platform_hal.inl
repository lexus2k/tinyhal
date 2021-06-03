/*
    Copyright 2019-2021 (C) Alexey Dynda

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

static void mutex_create_default(tiny_mutex_t *mutex)
{
    *mutex = 0;
}

static void mutex_destroy_default(tiny_mutex_t *mutex)
{
}

static void mutex_lock_default(tiny_mutex_t *mutex)
{
    uint8_t locked;
    do
    {
        locked = tiny_mutex_try_lock(mutex);
        if ( locked )
        {
            break;
        }
        tiny_sleep(1);
    } while ( !locked );
}

static uint8_t mutex_try_lock_default(tiny_mutex_t *mutex)
{
    uint8_t locked = __sync_bool_compare_and_swap(mutex, 0, 1);
    locked = !!locked;
    return locked;
}

static void mutex_unlock_default(tiny_mutex_t *mutex)
{
    /*uint8_t result =*/__sync_bool_compare_and_swap(mutex, 1, 0);
    // assert( result == 1 );
}

static void events_create_default(tiny_events_t *events)
{
    tiny_mutex_create(&events->mutex);
    events->bits = 0;
}

static void events_destroy_default(tiny_events_t *events)
{
    tiny_mutex_destroy(&events->mutex);
}

static uint8_t events_wait_default(tiny_events_t *events, uint8_t bits, uint8_t clear, uint32_t timeout)
{
    uint8_t locked = 0;
    uint32_t ts = tiny_millis();
    do
    {
        tiny_mutex_lock(&events->mutex);
        locked = events->bits;
        if ( clear && (locked & bits) )
            events->bits &= ~bits;
        tiny_mutex_unlock(&events->mutex);
        if ( !(locked & bits) && (uint32_t)(tiny_millis() - ts) >= timeout )
        {
            locked = 0;
            break;
        }
        tiny_sleep(1);
    } while ( !(locked & bits) );
    return locked;
}

static uint8_t events_check_int_default(tiny_events_t *events, uint8_t bits, uint8_t clear)
{
    uint8_t locked;
    tiny_mutex_lock(&events->mutex);
    locked = events->bits;
    if ( clear && (locked & bits) )
        events->bits &= ~bits;
    tiny_mutex_unlock(&events->mutex);
    return locked;
}

static void events_set_default(tiny_events_t *events, uint8_t bits)
{
    tiny_mutex_lock(&events->mutex);
    events->bits |= bits;
    tiny_mutex_unlock(&events->mutex);
}

static void events_clear_default(tiny_events_t *events, uint8_t bits)
{
    tiny_mutex_lock(&events->mutex);
    events->bits &= ~bits;
    tiny_mutex_unlock(&events->mutex);
}

static void sleep_default(uint32_t ms)
{
    // No default support for sleep
    uint32_t start = tiny_millis();
    while ( (uint32_t)(tiny_millis() - start) < ms )
        ;
}

static void sleep_us_default(uint32_t us)
{
    // No default support for sleep
    uint32_t start = tiny_micros();
    while ( (uint32_t)(tiny_micros() - start) < us )
        ;
}

static uint32_t millis_default()
{
    // No default support for milliseconds
    static uint32_t cnt = 0;
    return cnt++;
}

static uint32_t micros_default()
{
    // No default support for microseconds
    static uint32_t cnt = 0;
    return cnt++;
}

static tiny_platform_hal_t s_hal = {
    .mutex_create = mutex_create_default,
    .mutex_destroy = mutex_destroy_default,
    .mutex_try_lock = mutex_try_lock_default,
    .mutex_unlock = mutex_unlock_default,
    .mutex_lock = mutex_lock_default,

    .events_create = events_create_default,
    .events_destroy = events_destroy_default,
    .events_wait = events_wait_default,
    .events_check_int = events_check_int_default,
    .events_set = events_set_default,
    .events_clear = events_clear_default,

    .sleep = sleep_default,
    .millis = millis_default,
    .sleep_us = sleep_us_default,
    .micros = micros_default,
};

void tiny_mutex_create(tiny_mutex_t *mutex)
{
    s_hal.mutex_create(mutex);
}

void tiny_mutex_destroy(tiny_mutex_t *mutex)
{
    s_hal.mutex_destroy(mutex);
}

void tiny_mutex_lock(tiny_mutex_t *mutex)
{
    s_hal.mutex_lock(mutex);
}

uint8_t tiny_mutex_try_lock(tiny_mutex_t *mutex)
{
    return s_hal.mutex_try_lock(mutex);
}

void tiny_mutex_unlock(tiny_mutex_t *mutex)
{
    s_hal.mutex_unlock(mutex);
}

void tiny_events_create(tiny_events_t *events)
{
    s_hal.events_create(events);
}

void tiny_events_destroy(tiny_events_t *events)
{
    s_hal.events_destroy(events);
}

uint8_t tiny_events_wait(tiny_events_t *events, uint8_t bits, uint8_t clear, uint32_t timeout)
{
    return s_hal.events_wait(events, bits, clear, timeout);
}

uint8_t tiny_events_check_int(tiny_events_t *events, uint8_t bits, uint8_t clear)
{
    return s_hal.events_check_int(events, bits, clear);
}

void tiny_events_set(tiny_events_t *events, uint8_t bits)
{
    s_hal.events_set(events, bits);
}

void tiny_events_clear(tiny_events_t *events, uint8_t bits)
{
    s_hal.events_clear(events, bits);
}

void tiny_sleep(uint32_t ms)
{
    s_hal.sleep(ms);
}

void tiny_sleep_us(uint32_t us)
{
    s_hal.sleep_us(us);
}

uint32_t tiny_millis(void)
{
    return s_hal.millis();
}

uint32_t tiny_micros(void)
{
    return s_hal.micros();
}

void tiny_hal_init(tiny_platform_hal_t *hal)
{
    if ( hal->mutex_create )
        s_hal.mutex_create = hal->mutex_create;
    if ( hal->mutex_destroy )
        s_hal.mutex_destroy = hal->mutex_destroy;
    if ( hal->mutex_try_lock )
        s_hal.mutex_try_lock = hal->mutex_try_lock;
    if ( hal->mutex_unlock )
        s_hal.mutex_unlock = hal->mutex_unlock;
    if ( hal->mutex_lock )
        s_hal.mutex_lock = hal->mutex_lock;
    if ( hal->events_create )
        s_hal.events_create = hal->events_create;
    if ( hal->events_destroy )
        s_hal.events_destroy = hal->events_destroy;
    if ( hal->events_wait )
        s_hal.events_wait = hal->events_wait;
    if ( hal->events_check_int )
        s_hal.events_check_int = hal->events_check_int;
    if ( hal->events_set )
        s_hal.events_set = hal->events_set;
    if ( hal->events_clear )
        s_hal.events_clear = hal->events_clear;
    if ( hal->sleep )
        s_hal.sleep = hal->sleep;
    if ( hal->millis )
        s_hal.millis = hal->millis;
    if ( hal->sleep_us )
        s_hal.sleep_us = hal->sleep_us;
    if ( hal->micros )
        s_hal.micros = hal->micros;
}
