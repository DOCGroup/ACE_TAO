# $Id$

# QNX/RTP hosted, using gcc-2.95.2


inline = 1
debug = 1
optimize = 1
exceptions = 1

ifeq ($(exceptions),1)
  CCFLAGS += -fexceptions
  LDFLAGS += -fexceptions
else
  CCLAGS += -fno-exceptions
  LDLAGS += -fno-exceptions
endif # ! exceptions

CC      = gcc
CXX     = g++

include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

DCFLAGS += -gstabs

CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG)

DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -lsocket -lstdc++ -lm
OCFLAGS += -O3
PIC     = -fPIC
AR      = ar
ARFLAGS = ruv
#RANLIB  = ranlib
SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
