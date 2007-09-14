# $Id$

# UnixWare V2.2.1.2 with g++ version 2.7.2.2

debug ?= 0
exceptions ?= 1
threads ?= 1

CC              = gcc
CXX             = g++

ACE_GNUG_ON_UNIXWARE = 1

#ifeq ($(exceptions),1)
  CCFLAGS += -fexceptions
#endif

DCFLAGS         += -g
#DCFLAGS         += -g

OCCFLAGS         += -O2

DLD             = $(CXX) -g
LD              = $(CXX)
LIBS            += -lsocket -lnsl -lgen -lthread
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true
#SOFLAGS        = -assert pure-text
SOFLAGS         += -shared
# SOBUILD - compile into .so directly
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK        = $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB          = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

pipes ?= 1
CFLAGS          += -W -Wall -Wpointer-arith #### -Winline
CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
