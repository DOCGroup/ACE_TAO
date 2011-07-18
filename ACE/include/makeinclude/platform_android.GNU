# $Id$
static_libs_only=1
ssl=0

HOST_PREBUILT :=/Users/frohnhoefere/Android/android-toolchain
NDK_HOME :=/Users/frohnhoefere/Android/android-ndk-r5b
SYSROOT := $(HOST_PREBUILT)/sysroot

CC_FLAGS += $(CFLAGS) $(TEMPLATES_FLAG)

CPP_FLAGS += \
       -fpic \
       -ffunction-sections \
       -funwind-tables \
       -D__ARM_ARCH_5__ \
       -D__ARM_ARCH_5T__ \
       -D__ARM_ARCH_5E__ \
       -D__ARM_ARCH_5TE__ \
       -Wno-psabi \
       -march=armv5te \
       -mtune=xscale \
       -msoft-float \
       -fexceptions \
       -frtti \
       -mthumb \
       -O0 \
       -fomit-frame-pointer \
       -fno-strict-aliasing \
       -finline-limit=64 \
       -DANDROID \
       -g

SOFLAGS += $(NDK_HOME)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a 

LDFLAGS := \
	--sysroot=$(SYSROOT) \
	$(LDLIBS) \
	$(SOFLAGS)

POSTLINK := \
	--sysroot=$(SYSROOT) \
	$(LDLIBS) \
	$(SOFLAGS)

CC=arm-linux-androideabi-g++ $(CPP_FLAGS)
CXX=arm-linux-androideabi-g++ $(CPP_FLAGS)	
AR=arm-linux-androideabi-ar
STRIP=arm-linux-androideabi-strip
RANLIB=arm-linux-androideabi-ranlib
DLD =$(CXX)
LD =arm-linux-androideabi-g++

ifneq (,$(HOST_ROOT))
  TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
  TAO_IDL = $(HOST_ROOT)/bin/tao_idl
  TAO_IDL3_TO_IDL2 = $(HOST_ROOT)/bin/tao_idl3_to_idl2
  TAO_IDL_DEP = $(TAO_IDL)
  TAO_IDL3_TO_IDL2_DEP = $(TAO_IDL3_TO_IDL2)
  # make sure to use the target compiler, not the cross-compiler
  # as preprocessor for the cross-compiled idl tools
  TAO_IDL_PREPROCESSOR = gcc
endif
