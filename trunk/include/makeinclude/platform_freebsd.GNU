# $Id$

# platform_freebsd.GNU

#####################################################
# Replace 3 with 2 if you are running FreeBSD 2.x.x
#  ** How do we detect os version here?
#####################################################
VERSION = 3

ifneq ($(VERSION),3)
versioned_so = 1
endif

optimize = 1

debug = 1

CC              = gcc
CXX		= g++
CFLAGS          += -Wall -Wpointer-arith -pipe
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
ifeq ($(VERSION),3)
  LDFLAGS		+= -Wl,-rpath $(ACE_ROOT)/ace
endif
DLD             = $(CXX)
LD		= $(CXX)
LIBS            += -lstdc++
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
