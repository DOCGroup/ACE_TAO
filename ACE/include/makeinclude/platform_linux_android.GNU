# -*- Makefile -*-
# $Id$

# This file should allow ACE to be built for Android 2.3.1 (API Level 9)
# or greater, by cross compiling on Linux.

# The standalone gcc compilers in NDK r5c have issues with the visibility.
no_hidden_visibility ?= 1


include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU


ifeq ($(insure),0)
  CC  ?= arm-linux-androideabi-gcc
  CXX ?= arm-linux-androideabi-g++
  CXX_FOR_VERSION_TEST ?= $(CXX)
  AR ?= arm-linux-androideabi-ar
else
  CXX_FOR_VERSION_TEST ?= arm-linux-androideabi-g++
endif

# fix this
pipes ?= 1

# NDK says required for working around a CPU bug in some Cortex-A8 implementations
LDFLAGS += -Wl,--fix-cortex-a8

CXX_FULL_VERSION := $(shell $(CXX_FOR_VERSION_TEST) --version)

FLAGS_C_CC += -W -Wall -Wpointer-arith -Wno-psabi
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads

ifeq ($(buildbits),32)
  FLAGS_C_CC += -m32
  LDFLAGS    += -m32
endif
ifeq ($(buildbits),64)
  FLAGS_C_CC += -m64
endif

# DT_RUNPATH is preferred over DT_RPATH, but the linker will only use it when
# this extra flag option (enable-new-dtags) is present
LD_RPATH_FLAGS += -Wl,--enable-new-dtags

# Rely on _GNU_SOURCE to set these defaults defined in /usr/include/features.h
# instead of setting them directly here (older versions of gcc don't set it
# for you): _SVID_SOURCE _BSD_SOURCE _POSIX_SOURCE _POSIX_C_SOURCE=199506L, ...
CPPFLAGS += -D_GNU_SOURCE

DCFLAGS  += -ggdb
DCCFLAGS += -ggdb
DLD      = $(CXX)
LD       = $(CXX)
LIBS     += -ldl

ifeq ($(optimize),1)
  SOFLAGS += -Wl,-O3
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

OCFLAGS ?= -O3
OCCFLAGS ?= -O3

CFLAGS  += $(FLAGS_C_CC)
CCFLAGS += $(FLAGS_C_CC) $(TEMPLATES_FLAG)

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif

