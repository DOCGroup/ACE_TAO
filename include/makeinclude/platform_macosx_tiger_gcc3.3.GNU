# $Id$
# platform_macosx.GNU
# support for Mac OS X 10.4 (Tiger)
cidl = 1
boost = 1
zzip = 1
zlib = 1
ifndef exceptions
  exceptions = 1
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 0
endif
ifeq (,$(ssl))
  ssl = 1
endif
ifeq (,$(versioned_so))
  versioned_so = 0
endif

with_ld = macosx

pipes ?= 1

CC              = gcc-3.3
CXX             = g++-3.3
CFLAGS          += -Wall -Wpointer-arith -Wno-long-double  

DCFLAGS         += -g
DLD             = g++-3.3
DLD				= $(CXX)
LD				= $(CXX)
#LIBS += -lstdc++.6 -lSystem -lSystemStubs

OCFLAGS		   += -O2 -mcpu=G3 -mtune=G4
RANLIB          = ranlib
SOEXT			= dylib
SOFLAGS		   += -dynamiclib
SOBUILD			= -o $(VSHDIR)$*.dylib $<

# Test for template instantiation, add to SOFLAGS if versioned_so set, 
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

LDFLAGS         += -flat_namespace 
CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
