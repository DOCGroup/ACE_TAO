# $Id$

# platform_freebsd.GNU

##  See /usr/ports/Mk/bsd.port.mk for sample usage of the OSVERSION
##  variable, especially with respect to defining the correct compiler
##  flags for threads.
OSVERSION := $(shell sysctl -n kern.osreldate)

## GNU make does not support boolean conditional expressions
## so we have to use a dummy RET variable.
RET = $(shell test $(OSVERSION) -ge 200000 && printf 1)
ifeq ($(RET),1)
versioned_so = 1
endif

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
  optimize = 1
endif

CC              = gcc
CXX             = g++

CFLAGS          += -W -Wall -Wpointer-arith -pipe
DCFLAGS         += -g
RET = $(shell test $(OSVERSION) -ge 300000 && printf 1)
ifneq ($(RET),1)
  LDFLAGS       += -Wl,-rpath $(ACE_ROOT)/ace
endif
DLD             = $(CXX)
LD              = $(CXX)
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo

## Set the correct compiler flags for pthreads.  This
## test is copied from /usr/ports/Mk/bsd.port.mk
ifeq ($(threads),1)
RET = $(shell test $(OSVERSION) -lt 500016 && printf 1)
ifeq ($(RET),1)
  PTHREAD_CFLAGS = -D_THREAD_SAFE
  PTHREAD_LIBS   = -pthread
else
  PTHREAD_CFLAGS = -D_THREAD_SAFE
  PTHREAD_LIBS   = -lc_r
endif # RET

CFLAGS  += $(PTHREAD_CFLAGS)
LDFLAGS += $(PTHREAD_LIBS)
 
endif # threads

# Test for template instantiation, add to SOFLAGS if SONAME set, 
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
