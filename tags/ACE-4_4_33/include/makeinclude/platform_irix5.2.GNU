# $Id$

# Irix 5.2 with gcc 2.6.1

debug = 1
optimize = 1

MAKE		= gmake
CC		= gcc
CXX		= g++
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
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
