# -*- Makefile -*-

# This file allows ACE and applications using ACE GNU Makefiles to be built for
# Android by cross compiling on Linux.
#
# See here for latest documentation on how to invoke the NDK:
# https://android.googlesource.com/platform/ndk/+/master/docs/BuildSystemMaintainers.md

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

# Android Studio does not seem to recognize so files with versions, but if
# trying to add gnuace-built libraries as imported libraries in CMake in a
# Android Studio project, at runtime apparently the Android loader needs the
# SONAME be set to the filename or else it was fail trying to load the so file
# path from the host computer.
versioned_so = 3

# Only try to use clang, unless this is set to 0, then try to use g++
android_force_clang ?= 1

# This section deals with selecting the architecture/compiler
# As of writing information on ABIs can be found at https://developer.android.com/ndk/guides/abis

# Make sure android_abi or ANDROID_ABI are defined and they are the same.
# ANDROID_ABI and ANDROID_ARCH are acceptable for compatibility reasons.
ifndef android_abi
  ifdef ANDROID_ABI
    android_abi := $(ANDROID_ABI)
  else
    ifdef ANDROID_ARCH
      ifeq ($(ANDROID_ARCH),arm)
        android_abi := armeabi-v7a
      else
        android_abi := $(ANDROID_ARCH)
      endif
    else # Default to ARMv7 if nothing is set
      android_abi := armeabi-v7a
    endif
    ANDROID_ABI := $(android_abi)
  endif
else
  ANDROID_ABI := $(android_abi)
endif

# Alias neon
ifeq ($(android_abi), neon)
  android_abi := armeabi-v7a-with-neon
endif

android_neon ?= 0
ifeq ($(android_abi),armeabi-v7a-with-neon)
  android_abi := armeabi-v7a
  android_neon := 1
endif

ifeq ($(android_abi),armeabi-v7a)
  CROSS_COMPILE := armv7a-linux-androideabi
  FLAGS_C_CC += -march=armv7-a -mfloat-abi=softfp
  ifeq ($(android_neon),1)
    FLAGS_C_CC += -mfpu=neon
  else
    FLAGS_C_CC += -mfpu=vfpv3-d16
  endif
  # NDK says -Wl,--fix-cortex-a8 is required for working around a CPU bug
  # in some Cortex-A8 implementations
  LDFLAGS += -march=armv7-a -Wl,--fix-cortex-a8

else ifeq ($(android_abi),arm64-v8a)
  CROSS_COMPILE := aarch64-linux-android
  FLAGS_C_CC += -march=armv8-a
  LDFLAGS += -march=armv8-a

else ifeq ($(android_abi),x86)
  CROSS_COMPILE := i686-linux-android

else ifeq ($(android_abi),x86_64)
  CROSS_COMPILE := x86_64-linux-android
endif

ifndef CROSS_COMPILE
  $(error android_abi $(android_abi) is not valid)
endif

ifdef android_ndk
  ifndef android_api
    $(error android_ndk also requires defining android_api)
  endif

  android_ndk_tools ?= $(wildcard $(android_ndk)/toolchains/llvm/prebuilt/*/bin)
  ifndef android_ndk_tool_prefix
    android_ndk_tool_prefix := $(CROSS_COMPILE)$(android_api)-
  endif

  # We don't want this being used again except to signal that this is a
  # cross-compile build. If it is then the resulting command probably won't
  # exist and cause an error.
  CROSS_COMPILE := THIS_VALUE_SHOULD_NOT_BE_USED

  # Ignore value of CROSS_COMPILE because ar doesn't match clang like in
  # platform_clang_common.GNU.
  override_cross_compile = 1
  CC = $(android_ndk_tools)/$(android_ndk_tool_prefix)clang
  CXX = $(android_ndk_tools)/$(android_ndk_tool_prefix)clang++
  AR = $(android_ndk_tools)/llvm-ar

else # Standalone Toolchain
  CROSS_COMPILE := $(CROSS_COMPILE)-

  ifeq ($(android_abi),armeabi-v7a)
    # According to Google the armv7a-linux-androideabi- prefix should be
    # preferred because it produces more efficient code. However if it doesn't
    # exist since we're using an older NDK we have to fallback to
    # arm-linux-androideabi-. This isn't a problem when directly using the NDK
    # because the NDKs we support for that have armv7a-* clangs.
    ifeq (,$(shell command -v $(CROSS_COMPILE)clang $(ACE_NUL_STDERR)))
      CROSS_COMPILE := arm-linux-androideabi-
    endif
  endif

  # Export so child processes can use tools from the same toolchain.
  export CROSS_COMPILE
endif

ifeq ($(android_force_clang),0)
  ifdef android_ndk
    $(error Must use a standalone toolchain to use GCC)
  endif

  # Determine if the g++ compiler is GCC or Clang.
  # There are at least 3 assumptions being made here:
  #  - There is a g++ command to get the version from
  #  - Clang will have "clang" in the version string
  #  - If g++ is actually clang, then we can replace that with clang++
  gplusplus_version = $(shell $(CROSS_COMPILE)g++$(CROSS_COMPILE_SUFFIX) --version)
  actually_is_clang := $(strip $(findstring clang,$(gplusplus_version)))
else ifeq ($(android_force_clang),1)
  actually_is_clang := 1
else
  $(error Invalid value for android_force_clang: $(android_force_clang))
endif

FLAGS_C_CC += -Wpointer-arith
ifeq ($(threads),1)
  CPPFLAGS += -D_REENTRANT
  ifdef PLATFORM_AIO_SUPPORT
    CPPFLAGS += $(PLATFORM_AIO_SUPPORT)
  endif
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

ifneq ($(shell echo '\#include <android/ndk-version.h>' | $(CXX) -E - > /dev/null 2>&1; echo $$?), 0)
  CPPFLAGS += -DACE_ANDROID_NDK_MISSING_NDK_VERSION_H
endif

ifdef __NDK_MAJOR__
  CPPFLAGS += -D__NDK_MAJOR__=$(__NDK_MAJOR__)
endif

ifdef __NDK_MINOR__
  CPPFLAGS += -D__NDK_MINOR__=$(__NDK_MINOR__)
endif

ifdef __NDK_BETA__
  CPPFLAGS += -D__NDK_BETA__=$(__NDK_BETA__)
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

# link step to avoid 'command line too long' error on Windows
ifeq ($(OS), Windows_NT)
  SHOBJS_FILE = $(VSHDIR)$(MAKEFILE)_object_list.tmp
  CLEANUP_OBJS += $(SHOBJS_FILE)
  define SHLIBBUILD
    $(file >$(SHOBJS_FILE), $^)
    $(SHR_FILTER) $(SOLINK.cc) $(SO_OUTPUT_FLAG) $@ @$(SHOBJS_FILE) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS)
  endef
endif
