# Irix 6.2 with SGI C++

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.
#
# I (Gonzalo Diethelm <gonzo@@ing.puc.cl>) also turned off warning 85
# from the linker; otherwise there are a lot of warnings about symbols
# in the pthread library overriding symbols in the C library (which is
# OK), and added libpthread.so to the link phase.

CC		= cc
CXX		= CC
DLD		= $(CXX)
LD		= $(CXX) 
CPPFLAGS	+= -ptused -prelink -D_SGI_MP_SOURCE 
# For the new 32 bit C++ compiler (-n32)
#CPPFLAGS	+= -n32 -woff 1174,1209,1375,1506,1110,1552,1021,1171
# For the old C++ compiler (-32)
CPPFLAGS	+= +pp
LDFLAGS		+= -rpath "$(WRAPPER_ROOT)/ace" -Wl,-woff,85
LIBS		+= -lpthread
PIC		= -KPIC
AR		= ar
ARFLAGS		= r
RANLIB		= echo
SOFLAGS		= -shared $(CPPFLAGS) -all
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		 $(SOLINK.cc) -o $@@ $(LDFLAGS) $(VSHDIR)$*.o
