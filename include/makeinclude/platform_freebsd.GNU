# $Id$

# platform_freebsd.GNU

# ***** Notice: This file is still under testing. *****
# To be used on FreeBSD platform without pthread support.

CC              = gcc
CXX		= g++
CFLAGS          += $(DCFLAGS) -w -O2 -fno-strict-prototypes
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = ld
LD		= $(CXX) 
LIBS            += -lstdc++ -lcompat
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = -Bshareable -x
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
