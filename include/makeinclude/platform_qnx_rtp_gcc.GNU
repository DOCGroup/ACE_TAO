# $Id$

# QNX/RTP hosted, using gcc-2.95.2

ACE_HAS_GNUG_PRE_2_8 := 0

#new options as suggetsed by Andy Olson 3/10/01
#---------------/*************/---------------------------------------------
inline = 1
debug = 1
optimize = 0
exceptions = 1

# note: I used CPPFLAGS instead of C or CC FLAGS, because the stupid QNX gcc
# will link different start and end sections based on the setting of
# -fexceptions, so it must be on both the compile AND link phase.
#
ifeq ($(exceptions),1)
  CPPFLAGS += -fexceptions
else
  CPPFLAGS += -fno-exceptions
endif # ! exceptions
 
CC      = gcc
CXX     = g++
DCFLAGS += -gstabs

#
# This is a workaround needed only on QNX RTP.
#
TAO_DYNAMIC_LIBS=-lTAO_PortableServer
TAO_COS_NAMING_LIBS=-lTAO_IORTable -lTAO_RTSched -lTAO_PortableServer -lTAO_Svc_Utils
TAO_COS_NOTIFICATION_LIBS=-lTAO_Svc_Utils -lTAO_DynamicAny
TAO_RT_SCHED_LIBS=-lTAO_PortableServer
TAO_FAULT_TOLERANCE_LIBS=-lTAO_IORManip
TAO_COS_TRADING_LIBS=-lTAO_Svc_Utils

CFLAGS  += -pipe -W -Wall -Wpointer-arith
CCFLAGS += $(CFLAGS) -fno-implicit-templates -fexceptions

#
# Under the current version of QNX RTP ACE crashes
# if compiled with debug option enabled. We leave the option here,
# but disable debug by default.
#
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS +=
LIBS    += -lsocket -lstdc++ -lm
OCFLAGS += -O2
PIC     = -fPIC
AR      = ar
ARFLAGS = ruv
#RANLIB  = ranlib
SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true


