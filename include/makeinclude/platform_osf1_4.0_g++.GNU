# $Id$

# This platform macros file is intended to work with Digital UNIX 4.0
# (OSF/1 4.0) and GCC in version 2.7.2.1 (or newer).

CC		= gcc
CXX		= g++
CFLAGS          += $(DCFLAGS) -D_REENTRANT -O2
CCFLAGS         += $(CFLAGS) -fno-implicit-templates -fno-strict-prototypes
DCFLAGS		+= -g
LD		= $(CXX)
DLD		= $(CXX)
LIBS		+= -lpthread -lmach -lexc -ltli -lrt
PIC		= -fpic
ARFLAGS		= ruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB		= /usr/bin/true
SOFLAGS		= -shared $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB		= @true

