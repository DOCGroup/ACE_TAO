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

#
# This is a workaround needed only on QNX RTP.
#
TAO_DYNAMIC_LIBS=-lTAO_PortableServer
TAO_COS_NAMING_LIBS=-lTAO_IORTable -lTAO_RTSched -lTAO_PortableServer -lTAO_Svc_Utils
TAO_COS_NOTIFICATION_LIBS=-lTAO_Svc_Utils -lTAO_DynamicAny
TAO_RT_SCHED_LIBS=-lTAO_PortableServer
TAO_FAULT_TOLERANCE_LIBS=-lTAO_IORManip
TAO_COS_TRADING_LIBS=-lTAO_Svc_Utils

CC      = gcc
CXX     = g++
CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) -fno-implicit-templates -fexceptions

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

exceptions=1
