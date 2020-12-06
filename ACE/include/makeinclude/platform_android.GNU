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

# This section deals with selecting the architecture/compiler
# As of writing information on ABIs can be found at https://developer.android.com/ndk/guides/abis

ifndef android_abi
  ifdef ANDROID_ABI
    android_abi := $(ANDROID_ABI)
  else
    $(error android_abi or ANDROID_ABI must be defined)
  endif
endif

# Alias neon
ifeq ($(android_abi), neon)
  android_abi := armeabi-v7a-with-neon
endif

android_neon ?= 1
ifeq ($(android_abi),armeabi-v7a-with-neon)
  android_abi := armeabi-v7a
  android_neon := 1
endif

ifeq ($(android_abi),armeabi-v7a)
  CROSS_COMPILE := armv7a-linux-androideabi
  ifeq ($(android_neon),1)
    FLAGS_C_CC += -mfpu=neon
  else
    FLAGS_C_CC += -mfpu=vfpv3-d16
  endif
  # NDK says -Wl,--fix-cortex-a8 is required for working around a CPU bug
  # in some Cortex-A8 implementations
  LDFLAGS += -Wl,--fix-cortex-a8

else ifeq ($(android_abi),arm64-v8a)
  CROSS_COMPILE := aarch64-linux-android

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

  # We dont want this being used again except to signal that this is a
  # cross-compile build. If it is this command (probably) wont exist.
  CROSS_COMPILE := THIS_VALUE_SHOULD_NOT_BE_USED

  override_cross_compile = 1
  CC = $(android_ndk_tools)/$(android_ndk_tool_prefix)clang
  CXX = $(android_ndk_tools)/$(android_ndk_tool_prefix)clang++
  AR = $(android_ndk_tools)/llvm-ar

else
  CROSS_COMPILE := $(CROSS_COMPILE)-

  # Export so child processes can use tools from the same toolchain.
  export CROSS_COMPILE
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

android_set_lld ?= 1
ifeq ($(android_set_lld),1)
  # Use the LLVM linker as recommended by Google
  # The other two arguments are explained by
  # https://android.googlesource.com/platform/ndk/+/master/docs/BuildSystemMaintainers.md#additional-required-arguments
  LDFLAGS += -fuse-ld=lld -Wl,--build-id=sha1 -Wl,--no-rosegment
endif

DCFLAGS  += -ggdb
DCCFLAGS += -ggdb
DLD      = $(CXX)
LD       = $(CXX) $(PIE)

ifeq ($(optimize),1)
  SOFLAGS += -Wl,-O3
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

include $(ACE_ROOT)/include/makeinclude/platform_clang_common.GNU

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

# link step to avoid 'command line too long' error on Windows
ifeq ($(OS), Windows_NT)
  SHOBJS_FILE = $(VSHDIR)$(MAKEFILE)_object_list.tmp
  CLEANUP_OBJS += $(SHOBJS_FILE)
  define SHLIBBUILD
    $(file >$(SHOBJS_FILE), $^)
    $(SHR_FILTER) $(SOLINK.cc) $(SO_OUTPUT_FLAG) $@ @$(SHOBJS_FILE) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS)
  endef
endif
