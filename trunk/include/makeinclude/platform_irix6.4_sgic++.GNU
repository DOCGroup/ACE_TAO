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
#CCFLAGS		+= +pp
#CCFLAGS		+= -woff 3203,3209,3161,3262,3665
CCFLAGS		+= -woff 1188,1682,1171
# Instantiate everything; without this, apps/Gateway/Gateway/gatewayd might
# not build due to missing template instantiations.
CCFLAGS		+= -ptall
# Instantiate used templates, plus prelinking instantiation
# CCFLAGS	+= -ptused -prelink
# Unknown version of SGI CC, -n32 mode
# NOTE: -32/-n32/-64 mode is usually better set by the environment variable 'SGI_ABI'
#CPPFLAGS	+= -n32 -woff 1174,1209,1375,1506,1110,1552,1021,1171
LDFLAGS		+= -rpath "$(WRAPPER_ROOT)/ace"
LDFLAGS		+= -Wl,-woff,15
LIBS		+= -lpthread
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CCFLAGS) $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
