# Irix 5.3 with SGI C++

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.

CC		= cc
CXX		= CC
DLD		= $(CXX)
CPPFLAGS        += -ptused -prelink +pp -woff 3203,3209,3161,3262,3665
LDFLAGS		+= -rpath "$(WRAPPER_ROOT)/ace"
LIBS		= 
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
