# $Id$

# QNX 4.25 hosted GNU g++ for Neutrino 2.0

CROSS-COMPILE = 1
ACE_HAS_GNUG_PRE_2_8 := 0
static_libs_only = 1

ifeq (,$(debug))
  debug = 1
endif # ! debug
ifeq (,$(optimize))
  optimize = 1
endif # ! optimize
ifeq (,$(repo))
  repo = 1
endif # ! repo

CC      = i386-nto-gcc
CXX     = i386-nto-g++
CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) -fno-implicit-templates
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS +=
LIBS    += -lsocket -lstdc++
OCFLAGS += -O2
PIC     = -fPIC
AR      = i386-nto-ar
ARFLAGS = ruv
RANLIB  = i386-nto-ranlib
SOFLAGS = -G $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
