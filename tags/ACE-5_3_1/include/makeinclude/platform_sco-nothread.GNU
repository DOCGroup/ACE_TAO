# $Id$

debug = 1
optimize = 1

CC              = gcc
CXX		= g++

include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CFLAGS		+= -w
CCFLAGS		+= $(CFLAGS) $(TEMPLATES_FLAG) -fno-strict-prototypes
DCFLAGS		+= -g #### should this be -gstabs?
DLD             = $(CXX)
LD		= $(CXX)
LIBS            = -lsocket
OCFLAGS		= -O2
PIC             =
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS += $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
