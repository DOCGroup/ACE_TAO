# $Id$
 
# According to Ben Eng <ben@jetpen.com>, this file
# should allow ACE to be built on Linux using KAI C++.

debug = 1
optimize = 0

CC              = KCC
CXX		= KCC
CFLAGS		+=
CCFLAGS		+= $(CFLAGS)
DCFLAGS		+= +K0 -g
DLD             = $(CXX)
LD		= $(CXX)
LIBS		+= -ldl 
OCFLAGS		+= +K3 -O
PIC             = -fPIC
AR              = KCC
ARFLAGS         = -o
RANLIB          = true

SOFLAGS = $(CPPFLAGS) -tall
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1
