# $Id$

# Irix 5.2 with gcc 2.6.1
MAKE		= gmake
CC		= gcc
CXX		= g++
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS) -O2 -fno-implicit-templates
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX) 
LDFLAGS		+=
LIBS		+= -ldl
PIC		= -fpic
RANLIB		= echo
SOFLAGS		=
SOBUILD		=
