# $Id$

# Irix 6.4 with SGI C++

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.

CC		= cc
CXX		= CC
DLD		= $(CXX)
LD		= $(CXX) 

# Basic flags
CPPFLAGS	+= -D_SGI_MP_SOURCE
CCFLAGS		+= -O
CCFLAGS		+= -woff 1188,1682,1171,1681
# Instantiate everything; without this, apps/Gateway/Gateway/gatewayd might
# not build due to missing template instantiations.
#CCFLAGS		+= -ptall
# Instantiate used templates, plus prelinking instantiation
#CCFLAGS	+= -ptused -prelink
# Instantiate used templates, but do not run prelinker
#CCFLAGS		+= -ptused
# Instantiate no templates automatically, do not run prelinker
CCFLAGS		+= -ptnone -no_prelink
# Turn on exceptions
CCFLAGS		+= -exceptions -ptv
# Unknown version of SGI CC, -n32 mode
# NOTE: -32/-n32/-64 mode is usually better set by the environment variable 'SGI_ABI'
#CPPFLAGS	+= -n32 -woff 1174,1209,1375,1506,1110,1552,1021,1171
LDFLAGS		+= -rpath "$(ACE_ROOT)/ace"
LDFLAGS		+= -Wl,-woff,15
LDFLAGS		+= -Wl,-woff,85
LIBS		+= -lpthread
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CCFLAGS) $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
