# $Id$

# SunOS 4.x (Solaris 1.x) with g++

debug = 1
optimize = 1

CC	= gcc
CXX	= g++
CFLAGS  += -fno-strict-prototypes

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS	+= -g
DLD	= /bin/ld
LD	= $(CXX)
#
# for G++ v2.6.0
OCFLAGS	+= -O2
PIC	= -fpic
AR	= ar
ARFLAGS	= ruv
RANLIB	= ranlib
SOFLAGS	+= -assert pure-text
# SOBUILD - compile into .so directly
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB	= @true
