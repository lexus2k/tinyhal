default: libtinyhal
UNITTEST ?= n

include buildsys/toolchain.mk
include buildsys/project.mk
include buildsys/package_make.mk

######################## Custom options #########################

LOG_LEVEL ?= 0
ENABLE_DEBUG ?= n

ifneq ($(LOG_LEVEL),)
    CPPFLAGS += -DTINY_LOG_LEVEL_DEFAULT=$(LOG_LEVEL)
endif

ifeq ($(ENABLE_DEBUG),y)
    CPPFLAGS += -DTINY_DEBUG=1
endif

##################################################################

CFLAGS += -std=gnu99
CPPFLAGS += -Os -Wall -Werror -ffunction-sections -fdata-sections $(EXTRA_CPPFLAGS)
CXXFLAGS += -std=c++11

####################### Compiling library #########################
PKG = libtinyhal
$(PKG)_SOURCES = \
	src/hal/tiny_list.c \
	src/hal/tiny_types.c \
	src/hal/tiny_serial.c
$(PKG)_PKGCONFIG = tinyhal.pc
$(PKG)_CMAKE_MODULE = Findtinyhal.cmake
$(PKG)_CPPFLAGS += -I./src
$(eval $(project-static-lib))

####################### Compiling unit tests ####################
ifeq ($(PLATFORM),linux)
PKG = unittest
$(PKG)_NAME = unit_test
$(PKG)_SOURCES = \
	unittest/main.o \
	unittest/hal_tests.o
$(PKG)_DEPENDENCIES = libtinyhal
$(PKG)_LIBRARIES =
$(PKG)_CPPFLAGS = -pthread -I./src -I./unittest
$(PKG)_LDFLAGS = -pthread -lm -L. -ltinyhal -lCppUTest -lCppUTestExt
$(PKG)_INSTALL_TARGET = NO
$(eval $(project-binary))

.PHONY: check
unit_test: .ts_unittest_build
check:
	./unit_test

ifeq ($(UNITTEST),y)
check: unittest
all: unittest
endif

endif

