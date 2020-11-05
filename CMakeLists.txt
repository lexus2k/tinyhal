cmake_minimum_required (VERSION 3.5)

option(UNITTEST "Build unit tests" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/buildsys)
include(package-linker)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB BASEDIR src)
# file(GLOB_RECURSE HEADER_FILES src/*.h)

register_static_library(NAME tinyhal
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
    INCLUDE_DIR ${BASEDIR}
    PKGCONFIG tinyhal.pc
    CMAKE_MODULE Findtinyhal.cmake
)

if (UNITTEST)
    add_subdirectory(unittest)
endif()

