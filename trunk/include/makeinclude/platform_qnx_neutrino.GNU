# $Id$

# QNX 4.25 hosted GNU g++ for Neutrino 2.0 Beta

# NOTE: with g++ 2.8.0, you'll need to disable optimization in order to
#       instantiate ACE_Map_Manager (ace/Map_Manager.cpp).  The easiest
#       way to do that is to set "optimize = 0", either below or on
#       your make command line.

ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 0
endif

CC      = /usr/gcc/bin/i386-nto-gcc
CXX     = /usr/gcc/bin/i386-nto-g++
CFLAGS  += -pipe   #### -Wall -Wpointer-arith -Winline -bntox86
CCFLAGS += $(CFLAGS) -fno-implicit-templates
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS +=
LIBS    += -lsocket -lstdc++
OCFLAGS += -O2
PIC     = -fPIC
AR      = /usr/gcc/bin/ar
ARFLAGS = ruv
RANLIB  = /usr/gcc/bin/ranlib
SOFLAGS = -G $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
