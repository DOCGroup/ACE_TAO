# -*- Makefile -*-
# $Id$

# This file should allow ACE to be built for Android 2.3.1 (API Level 9)
# or greater, by cross compiling on Linux.

# The standalone gcc compilers in NDK r6 have issues with the visibility.
no_hidden_visibility ?= 1

# as of NDK r6 inlining and static libs are required
inline ?= 1
static_libs_only ?= 1

include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

#This is a cross compiled build, $HOST_ROOT must point to local build tools
TAO_IDL := $(HOST_ROOT)/bin/tao_idl
TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
TAO_IDL_DEP := $(HOST_ROOT)/bin/tao_idl$(EXEEXT)

#Standalone toolchain required.
#Required environment variables are
#$NDK - install directory for android NDK, at least r5c
#$SDK - install directory for android SDK
#$SYSROOT - $NDK/platform/android-9
#$TOOLCHAIN/bin in path, $TOOLCHAIN is root of standalone toolchain
#
#Toolchain defaults to arm architecture
ANDROID_ARCH ?= arm

SYSROOT ?= $(NDK)/platforms/android-9/arch-$(ANDROID_ARCH)

ifeq ($(ANDROID_ARCH),arm)
  CROSS_COMPILE=arm-linux-androideabi-
else
  ifeq ($(ANDROID_ARCH),x86)
    CROSS_COMPILE=i686-android-linux-
  else
    CROSS_COMPILE=unknown-android-arch-
  endif
endif

# fix this
pipes ?= 1

# NDK says required for working around a CPU bug in some Cortex-A8 implementations
ifeq ($(ANDROID_ARCH),arm)
LDFLAGS += -Wl,--fix-cortex-a8
endif

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

