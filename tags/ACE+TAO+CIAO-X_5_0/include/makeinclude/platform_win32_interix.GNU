# $Id$

# platform_win32_interix.GNU

#versioned_so = 1

exceptions ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1

CC          = gcc
CXX		= g++

include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CFLAGS          += -W -Wall -Wpointer-arith -pipe -frepo
CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG) -D_ALL_SOURCE
DCFLAGS         += -g
LDFLAGS	    += -L /usr/lib -L$(ACE_ROOT)/lib -ldl -lc -dynamic
DLD             = $(CXX)
LD		    = $(CXX)
LIBS            +=
AR              = ar
ARFLAGS         = ruvs
RANLIB          = ranlib

SOFLAGS += $(CPPFLAGS) -shared -i -W1,-E -W1,-Bsymbolic
#SOBUILD = $(COMPILE.cc) -o $(VSHDIR)$*.so  $<
PRELIB = @echo

ifeq ($(threads),1)
LDFLAGS         += -lpthread
#CFLAGS          += -D_THREAD_SAFE -D_REENTRANT -DACE_HAS_THREADS
CFLAGS          += -D_REENTRANT -DACE_HAS_THREADS
endif # threads

