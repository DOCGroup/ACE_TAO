# $Id$

# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

debug = 1

CC              = gcc
CXX		= g++
CFLAGS		+= -w -O2 -fno-strict-prototypes
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g
DLD             = $(CXX)
LD		= $(CXX)
LIBS		+= -ldl 
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
