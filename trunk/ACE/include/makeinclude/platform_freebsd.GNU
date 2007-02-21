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

exceptions ?= 1
threads ?= 1
debug ?= 1
optimize ?= 1
pipes ?= 1

CC              = gcc
CXX             = g++

CFLAGS          += -W -Wall -Wpointer-arith
DCFLAGS         += -g
RET = $(shell test $(OSVERSION) -ge 300000 && printf 1)
ifneq ($(RET),1)
  LDFLAGS       += -Wl,-rpath $(ACE_ROOT)/lib
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
  PTHREAD_CPPFLAGS = -D_THREAD_SAFE
  PTHREAD_LIBS   = -pthread
else
  RET = $(shell test $(OSVERSION) -lt 502102 && printf 1)
  ifeq ($(RET),1)
  PTHREAD_LIBS   = -lc_r
  else
  PTHREAD_LIBS   = -lpthread
  endif
endif # RET

CPPFLAGS  += $(PTHREAD_CPPFLAGS)
LIBS += $(PTHREAD_LIBS)

else
CPPFLAGS += -DACE_MT_SAFE=0
endif # threads

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
