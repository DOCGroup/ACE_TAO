# $Id$

# This platform macros file is intended to work with Digital UNIX 4.x
# (OSF/1 4.x) and GCC in version 2.9.6 or newer
debug ?= 1
exceptions ?= 1
rtti ?= 1
threads ?= 1
optimize ?= 1

CC              = gcc
CXX             = g++
CFLAGS          += -D_REENTRANT

DCFLAGS         += -g
LD              = $(CXX)
DLD             = $(CXX)
LIBS            += -lpthread -lmach -lexc -lxti -lrt
OCFLAGS         += -O3
PIC             = -fpic
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         += -shared $(CPPFLAGS) $(ACELIB)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
