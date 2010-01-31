# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the GNU compiler.

# Of late, this platform and compiler combination hasn't been tested. In
# fact neither compilation nor testing has been done for almost a year.
# So it's better to avoid this combination, if possible.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU

debug ?= 1
exceptions ?= 1
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

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU
