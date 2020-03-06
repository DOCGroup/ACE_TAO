# -*- Makefile -*-

# This file allows ACE and applications using ACE GNU Makefiles to be built for
# Android by cross compiling on Linux.

# We always include config-android.h on Android platforms.
ACE_PLATFORM_CONFIG ?= config-android.h

# Common Linux Functionality
include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

# as of NDK r6 inlining is required
inline ?= 1

# as of API level 16 executables can be linked as Position Independent Executables
# as of API level 21 PIE is mandatory
PIE	?= -pie

#No rwho on Android
rwho = 0

# Android Studio does not seem to recognize so files with versions
versioned_so ?= 0

# Only try to use clang, unless this is set to 0, then try to use g++
android_force_clang ?= 1

# This section deals with selecting the architecture/compiler
# As of writing information on ABIs can be found at https://developer.android.com/ndk/guides/abis

ifndef ANDROID_ABI
  ifdef ANDROID_ARCH # Handle Possiblity of ANDROID_ARCH being passed
    ifeq ($(ANDROID_ARCH),arm)
      ANDROID_ABI := armeabi-v7a
    else
      ANDROID_ABI := $(ANDROID_ARCH)
    endif
  else # Else default to ARMv7
    ANDROID_ABI := armeabi-v7a
  endif
endif

# Alias neon
ifeq ($(ANDROID_ABI), neon)
  ANDROID_ABI := armeabi-v7a-with-neon
endif

android_neon ?= 0
ifeq ($(ANDROID_ABI),armeabi-v7a-with-neon)
  ANDROID_ABI := armeabi-v7a
  android_neon := 1
endif

# NDK says -Wl,--fix-cortex-a8 is required for working around a CPU bug
# in some Cortex-A8 implementations

ifeq ($(ANDROID_ABI),armeabi-v7a)
  CROSS_COMPILE := arm-linux-androideabi-
  FLAGS_C_CC += -march=armv7-a -mfloat-abi=softfp
  ifeq ($(android_neon),1)
    FLAGS_C_CC += -mfpu=neon
  else
    FLAGS_C_CC += -mfpu=vfpv3-d16
  endif
  LDFLAGS    += -march=armv7-a -Wl,--fix-cortex-a8
endif

ifeq ($(ANDROID_ABI),arm64-v8a)
  CROSS_COMPILE := aarch64-linux-android-
  FLAGS_C_CC += -march=armv8-a
  LDFLAGS    += -march=armv8-a
endif

ifeq ($(ANDROID_ABI),x86)
  CROSS_COMPILE := i686-linux-android-
endif

ifeq ($(ANDROID_ABI),x86_64)
  CROSS_COMPILE := x86_64-linux-android-
endif

ifndef CROSS_COMPILE
  $(error ANDROID_ABI passed (set to: $(ANDROID_ABI)) is not valid)
endif

# Export so child processes can use tools from the same toolchain.
export CROSS_COMPILE

ifeq ($(android_force_clang),0)
  # Determine if the g++ compiler is GCC or Clang.
  # There are at least 3 assumptions being made here:
  #  - There is a g++ command to get the version from
  #  - Clang will have "clang" in the version string
  #  - If g++ is actually clang, then we can replace that with clang++
  gplusplus_version = $(shell $(CROSS_COMPILE)g++$(CROSS_COMPILE_SUFFIX) --version)
  actually_is_clang := $(strip $(findstring clang,$(gplusplus_version)))
else
  ifeq ($(android_force_clang),1)
    actually_is_clang := 1
  else
    $(error Invalid value for android_force_clang: $(android_force_clang))
  endif
endif

FLAGS_C_CC += -W -Wall -Wpointer-arith
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads

# Use -pipes by default
pipes ?= 1

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
LD       = $(CXX) $(PIE)

ifndef actually_is_clang
  # make sure to add the required libs for RTTI and exceptions (libsupc++)
  # and a shared STL lib (libgnustl_shared by default)
  static_libs_only ?=
  ANDROID_STL_DLL ?= gnustl_shared
  LIBS     += -ldl -lsupc++
  ifneq ($(static_libs_only),1)
    LIBS   += -l$(ANDROID_STL_DLL)
  endif
endif

ifeq ($(optimize),1)
  SOFLAGS += -Wl,-O3
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

ifdef actually_is_clang
  include $(ACE_ROOT)/include/makeinclude/platform_clang_common.GNU
else
  include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU
endif

ifeq ($(shell echo '\#include "android/ndk-version.h"' | $(CXX) -E - > /dev/null 2>&1; echo $$?), 0)
  CPPFLAGS += -DACE_ANDROID_NDK_HAS_NDK_VERSION_H
endif

ifdef __NDK_MAJOR__
  CPPFLAGS += -D__NDK_MAJOR__=$(__NDK_MAJOR__)
endif

ifdef __NDK_MINOR__
  CPPFLAGS += -D__NDK_MINOR__=$(__NDK_MINOR__)
endif

OCFLAGS ?= -O3
OCCFLAGS ?= -O3

# Android preloads the (almost certainly incompatible) system SSL library
# (either OpenSSL or BoringSSL) for the Java Android API, so we must:
#   1. Statically link OpenSSL to this library
#   2. Keep our OpenSSL symbols internal
# Number 1 is described in ACE-INSTALL.html.
# We are doing number 2 here.
ifeq ($(ssl),1)
  PLATFORM_SSL_LDFLAGS += --exclude-libs libcrypto.a,libssl.a
endif

# Link To Android Logging Library for Log_Msg_Android_Logcat
LIBS += -llog
