/*
    Copyright 2017-2019 (C) Alexey Dynda

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


#include <functional>
#include <CppUTest/TestHarness.h>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "hal/tiny_types.h"
#include "hal/tiny_list.h"
#include "hal/tiny_debug.h"


TEST_GROUP(HAL)
{
    void setup()
    {
        // ...
    }

    void teardown()
    {
        // ...
    }
};

TEST(HAL, mutex)
{
    tiny_mutex_t mutex;
    tiny_mutex_create( &mutex );
    try
    {
        tiny_mutex_lock( &mutex );
        CHECK_EQUAL( 0, tiny_mutex_try_lock( &mutex ) );
        tiny_mutex_unlock( &mutex );
        CHECK_EQUAL( 1, tiny_mutex_try_lock( &mutex ) );
        CHECK_EQUAL( 0, tiny_mutex_try_lock( &mutex ) );
        tiny_mutex_unlock( &mutex );
        tiny_mutex_destroy( &mutex );
    }
    catch(...)
    {
        tiny_mutex_destroy( &mutex );
        throw;
    }
}

TEST(HAL, deadlock)
{
    bool concurrent_lock = false;
    tiny_mutex_t mutex;
    tiny_mutex_create( &mutex );
    tiny_mutex_lock( &mutex );
    std::thread concurrent_thread( [&concurrent_lock](tiny_mutex_t &mutex)->void
    {
        tiny_mutex_lock(&mutex);
        concurrent_lock = true;
        tiny_mutex_unlock(&mutex);
    }, std::ref(mutex) );
    try
    {
        tiny_sleep( 100 );
        CHECK_EQUAL( false, concurrent_lock );
        tiny_mutex_unlock( &mutex );
        tiny_sleep( 100 );
        CHECK_EQUAL( true, concurrent_lock );
    }
    catch(...)
    {
        concurrent_thread.join();
        tiny_mutex_destroy( &mutex );
        throw;
    }
    concurrent_thread.join();
    tiny_mutex_destroy( &mutex );
}

extern "C" void tiny_list_init(void);

TEST(HAL, list)
{
    list_element el1{};
    list_element el2{};
    list_element el3{};
    list_element *list = nullptr;
    tiny_list_init();
    tiny_list_add( &list, &el1 );
    CHECK_EQUAL( &el1, list );
    tiny_list_add( &list, &el2 );
    CHECK_EQUAL( &el2, list );
    tiny_list_add( &list, &el3 );
    CHECK_EQUAL( &el3, list );
    static int cnt = 0;
    tiny_list_enumerate( list, [](list_element *, uint16_t data)->uint8_t { cnt += data; return 1; }, 2 );
    CHECK_EQUAL( 6, cnt );
    tiny_list_remove( &list, &el2 );
    CHECK_EQUAL( &el3, list );
    tiny_list_remove( &list, &el3 );
    CHECK_EQUAL( &el1, list );
    tiny_list_clear( &list );
    CHECK_EQUAL( (list_element *)nullptr, list );
}

TEST(HAL,loglevel)
{
    tiny_log_level( 5 );
    CHECK_EQUAL( 5, g_tiny_log_level );
    tiny_log_level( 0 );
    CHECK_EQUAL( 0, g_tiny_log_level );
}

