# $Id$

# This platform macros file is intended to work with Digital UNIX 4.x
# (OSF/1 4.x) and GCC in version 2.7.2.1 (or newer).

ifeq (,$(debug))
  debug = 1
endif

ifeq (,$(exceptions))
  exceptions = 1
endif
ifeq (,$(rtti))
   rtti = 1
endif

ifeq (,$(threads))
  threads = 1
endif

ifeq (,$(optimize))
  optimize = 1
endif

CC              = gcc
CXX             = g++
CFLAGS          += -D_REENTRANT
CCFLAGS         += $(CFLAGS) -fno-implicit-templates -fno-strict-prototypes
DCFLAGS         += -g
LD              = $(CXX)
DLD             = $(CXX)
LIBS            += -lpthread -lmach -lexc -ltli -lrt
OCFLAGS         += -O2
PIC             = -fpic
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         = -shared $(CPPFLAGS) $(ACELIB)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true
