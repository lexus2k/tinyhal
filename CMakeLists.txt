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

#    set(options OPTIONAL FAST)
#    set(options)
#    set(oneValueArgs NAME INCLUDE_DIR)
#    set(multiValueArgs SOURCES HEADERS PKGCONFIG CMAKE_MODULE LINK_EXTERNAL)

#if (NOT DEFINED COMPONENT_DIR)
#
#    project (tinyhal)

#    include_directories(src)

#    add_library(tinyhal STATIC ${HEADER_FILES} ${SOURCE_FILES})

#    set_target_properties(tinyhal PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

#    install(TARGETS tinyhal
#        ARCHIVE DESTINATION usr/lib
#    )

#    foreach(HFILE ${HEADER_FILES})
#        string(LENGTH ${BASEDIR} SLEN)
#        get_filename_component(DIR ${HFILE} DIRECTORY BASE_DIR src/)
#        string(SUBSTRING ${DIR} ${SLEN} -1 DIR)
#        install(FILES ${HFILE} DESTINATION usr/include/${DIR})
#    endforeach()
#    install(FILES tinyhal.pc DESTINATION usr/lib/pkgconfig/)
#    install(FILES Findtinyhal.cmake DESTINATION usr/share/cmake/)

#else()

#    idf_component_register(SRCS ${SOURCE_FILES}
#                           INCLUDE_DIRS "src")

# endif()
