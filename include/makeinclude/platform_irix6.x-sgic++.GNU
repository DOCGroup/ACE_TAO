# $Id$

# Irix 6.[234] with SGI C++

debug = 1
optimize = 1

# Instead of setting the ABI here we rely on the the macro SGI_ABI.
CC		= cc
CXX		= CC
DLD		= $(CXX)
LD		= $(CXX) 

# Basic flags

# In Irix 6.2 w/o the thread patches this macro should have no effect.
CPPFLAGS	+= -D_SGI_MP_SOURCE
OCCFLAGS	+= -O

# Enable exceptions even in the o32 bit ABI.
CCFLAGS		+= -exceptions

# Instantiate no templates automatically, do not run prelinker. There
# are other choices available, but those have not been used in a while.
CCFLAGS		+= -ptnone -no_prelink
# Instantiate everything; without this, apps/Gateway/Gateway/gatewayd might
# not build due to missing template instantiations. NOTE: it seems
# this setting no longer works for ACE.
#CCFLAGS		+= -ptall
# Instantiate used templates, plus prelinking instantiation
#CCFLAGS	+= -ptused -prelink
# Instantiate used templates, but do not run prelinker
#CCFLAGS		+= -ptused

LDFLAGS		+= -rpath "$(ACE_ROOT)/ace"
LDFLAGS		+= -Wl,-woff,15
LDFLAGS		+= -Wl,-woff,84
LDFLAGS		+= -Wl,-woff,85
LDFLAGS		+= -Wl,-woff,133

# You may need to remove this if you are using 6.2 without the Pthread
# patches.
LIBS		+= -lpthread

PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CCFLAGS) $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
