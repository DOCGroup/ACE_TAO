# -*- Makefile -*-
# $Id$

# platform_netbsd.GNU

debug ?= 1
optimize ?= 1
threads ?= 1
versioned_so ?= 1

CC              = gcc
CXX             = g++

AR              = ar
ARFLAGS         = ruv
CPPFLAGS        += -I/usr/local/include
DCFLAGS         += -g
DCCFLAGS        += $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += -L/usr/local/lib
OCFLAGS         += -O2
OCCFLAGS        = $(OCFLAGS)
PIC             = -fpic
PRELIB          = @true
RANLIB          = ranlib
SOFLAGS         += -shared

SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<

ifeq ($(threads),1)
  LDFLAGS       += -pthread
  CPPFLAGS      += -D_REENTRANT -DACE_HAS_THREADS
endif # threads

# Test for template instantiation, add to versioned_so if versioned_so set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU
