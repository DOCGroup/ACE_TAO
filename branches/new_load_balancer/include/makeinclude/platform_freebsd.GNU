# $Id$

# platform_freebsd.GNU

VERSION := $(shell uname -r | sed -e "s/`uname -r | sed -e 's/.//'`//")
ifeq ($(VERSION),2)
versioned_so = 1
endif

optimize = 1

debug = 1

CC              = gcc
CXX             = g++
CFLAGS          += -W -Wall -Wpointer-arith -pipe
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
ifneq ($(VERSION),3)
  LDFLAGS       += -Wl,-rpath $(ACE_ROOT)/ace
endif
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lstdc++
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
