# $Id$

# Irix 5.2 with gcc 2.6.1

debug = 1
optimize = 1

MAKE		= gmake
CC		= gcc
CXX		= g++

DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX) 
LDFLAGS		+=
LIBS		+= -ldl
OCFLAGS		+= -O2
PIC		= -fpic
RANLIB		= echo
SOFLAGS		=
SOBUILD		=

# Test for template instantiation, add to SOFLAGS if SONAME set, 
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS		+= $(CFLAGS) $(TEMPLATES_FLAG)
