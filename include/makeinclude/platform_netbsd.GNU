# $Id$

# platform_netbsd.GNU

# ***** Notice: This file is still under testing. *****
# To be used on netbsd platform without pthread support.

debug = 1
optimize = 1
versioned_so = 1

CC              = gcc
CXX		= g++

include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CFLAGS          += -w -fno-strict-prototypes
CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g
DLD             = ld
LD		= $(CXX)
LIBS            += -lstdc++  #-lcompat -lresolv
##OCFLAGS         += -O2
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS += -Bshareable -x /usr/lib/c++rt0.o
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so  $<
PRELIB = @echo
