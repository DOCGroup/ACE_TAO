# $Id$

# Irix 6.[234] with SGI C++ in -32 (aka -o32) mode

# Note that -32 mode doesn't have exceptions enabled by default, and 
# that the man-page cautions against it's use (you may not be able to
# mix code compiled with and without exceptions for example!)

# If you want exceptions it's recommended that you use -n32 mode, 
# which you should probably do anyway unless you have some special 
# reasons not to.

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.

CC		= cc -32
CXX		= CC -32
DLD		= $(CXX)
LD		= $(CXX) 

# Basic flags
CPPFLAGS	+= -D_SGI_MP_SOURCE
CCFLAGS		+= -O
CCFLAGS		+= +pp -woff 3203,3209,3161,3262,3665,3937,3259,3920
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
LDFLAGS		+= -Wl,-woff,15 -Wl,-woff,84 -Wl,-woff,85 -Wl,-woff,133
LIBS		+= -lpthread
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CCFLAGS) $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
