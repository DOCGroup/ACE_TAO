# $Id$

# QNX 4.25 hosted GNU g++ for Neutrino 2.0

CROSS-COMPILE = 1
ACE_HAS_GNUG_PRE_2_8 := 0
#static_libs_only = 1

#
# Under the current version of QNX RTP ACE crashes
# if compiled with debug oprion enabled.
#

ifeq (,$(optimize))
  optimize = 1
endif # ! optimize

ifeq (,$(repo))
  repo = 1
endif # ! repo

CC      = gcc
CXX     = g++
CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) -fno-implicit-templates

#
# Under the current version of QNX RTP ACE crashes
# if compiled with debug oprion enabled.
#
#DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS +=
LIBS    += -lsocket -lstdc++ -lm
OCFLAGS += -O2
PIC     = -fPIC
AR      = ar
ARFLAGS = ruv
#RANLIB  = ranlib
SOFLAGS = -G $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
