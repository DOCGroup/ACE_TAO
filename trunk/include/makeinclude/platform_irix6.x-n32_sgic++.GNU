# $Id$

# Irix 6.[234] with SGI C++ in -n32 mode

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.

CC		= cc -n32
CXX		= CC -n32
DLD		= $(CXX)
LD		= $(CXX) 

# Basic flags
CPPFLAGS	+= -D_SGI_MP_SOURCE
CCFLAGS		+= -O
CCFLAGS		+= -woff 1188,1682,1171,1681,1110
# Instantiate everything; without this, apps/Gateway/Gateway/gatewayd might
# not build due to missing template instantiations.
#CCFLAGS		+= -ptall
# Instantiate used templates, plus prelinking instantiation
#CCFLAGS	+= -ptused -prelink
# Instantiate used templates, but do not run prelinker
#CCFLAGS		+= -ptused
# Instantiate no templates automatically, do not run prelinker
CCFLAGS		+= -ptnone -no_prelink
LDFLAGS		+= -rpath "$(ACE_ROOT)/ace"
LDFLAGS		+= -Wl,-woff,15 -Wl,-woff,84 -Wl,-woff,85
LIBS		+= -lpthread
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CCFLAGS) $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
