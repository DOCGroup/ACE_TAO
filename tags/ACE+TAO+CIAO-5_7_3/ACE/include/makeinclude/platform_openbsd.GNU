# $Id$

# platform_openbsd.GNU

versioned_so = 1

exceptions ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1
pipes ?= 1

CC              = gcc
CXX		= g++

CFLAGS          += -W -Wall -Wpointer-arith
DCFLAGS         += -g
LDFLAGS		+= -Wl,-rpath $(ACE_ROOT)/lib
DLD             = $(CXX)
LD		= $(CXX)
LIBS            +=
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

#SOFLAGS += -Bshareable -x /usr/lib/c++rt0.o
SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so  $<
PRELIB = @echo

ifeq ($(threads),1)
LDFLAGS         += -pthread
CFLAGS          += -D_THREAD_SAFE -DACE_HAS_THREADS
endif # threads

PLATFORM_X11_CPPFLAGS=-I/usr/X11R6/include
PLATFORM_X11_LIBS=-lX11
PLATFORM_X11_LDFLAGS=-L/usr/X11R6/lib

PLATFORM_TK_CPPFLAGS=$(shell . /usr/local/lib/tk*/tkConfig.sh && echo -n $$TK_INCLUDE_SPEC $$TK_DEFS) $(PLATFORM_X11_CPPFLAGS)
PLATFORM_TK_LIBS=$(shell . /usr/local/lib/tk*/tkConfig.sh && echo -n $$TK_LIB_FLAG) $(PLATFORM_X11_LIBS)
PLATFORM_TK_LDFLAGS=$(PLATFORM_X11_LDFLAGS)

PLATFORM_TCL_CPPFLAGS=$(shell . /usr/local/lib/tcl*/tclConfig.sh && echo -n $$TCL_INCLUDE_SPEC $$TCL_DEFS)
PLATFORM_TCL_LIBS=$(shell . /usr/local/lib/tcl*/tclConfig.sh && echo -n $$TCL_LIB_FLAG)
PLATFORM_TCL_LDFLAGS=

# Setting the internal name and version of shared libraries isn't supported
# by OpenBSD.  In fact, the -h option gives an error and interestingly
# enough, the -soname option doesn't but just ignores its parameter.
# This tells platform_g++_common.GNU not to add these link options.
with_ld = openbsd

# Test for template instantiation, add to SOFLAGS if versioned_so set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)

