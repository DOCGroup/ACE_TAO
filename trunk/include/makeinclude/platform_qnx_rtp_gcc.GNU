# $Id$

# QNX/RTP hosted, using gcc-2.95.2

ACE_HAS_GNUG_PRE_2_8 := 0

# ACE crashes when compiled with debugging symbols
ifeq (,$(debug))
  debug := 0
endif # ! debug

ifeq (,$(optimize))
  optimize = 1
endif # ! optimize

CC      = gcc
CXX     = g++
CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) -fno-implicit-templates

#
# Under the current version of QNX RTP ACE crashes
# if compiled with debug option enabled. We leave the option here,
# but disable debug by default.
#
DCFLAGS += -g
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
