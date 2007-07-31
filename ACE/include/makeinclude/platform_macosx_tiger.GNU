# $Id$
# platform_macosx.GNU
# support for Mac OS X 10.4 (Tiger)
# By default, this uses the default compiler selected using gcc_select.

cidl = 1
boost = 1
zzip = 1
zlib = 1
exceptions ?= 1
threads ?= 1
debug ?= 1
optimize ?= 0
ssl ?= 1
versioned_so ?= 0
universal ?= 0

ifeq (,$(no_hidden_visibility))
# Disable symbol visibility support by default.
#
# Apple's g++ 4 compiler doesn't appear to correctly support
# visibility attributes, at least as well as the vanilla g++.
  no_hidden_visibility = 1
endif

with_ld = macosx

pipes ?= 1

CC              = gcc
CXX             = g++
CFLAGS          += -Wall -Wpointer-arith -Wno-long-double

DCFLAGS         += -g
DLD				= $(CXX)
LD				= $(CXX)
#LIBS += -lstdc++.6 -lSystem -lSystemStubs

OCFLAGS		   += -O2 
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

ifeq ($(universal),1)
  CFLAGS += -arch i386 -arch ppc
  LDFLAGS += -arch i386 -arch ppc
endif
