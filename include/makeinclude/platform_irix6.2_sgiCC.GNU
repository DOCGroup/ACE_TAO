# Irix 6.2 with SGI C++

# This suppresses common compiler warnings which appear in the 
# ACE code but should not matter.  The warnings can be turned on
# again by removing the -woff clause in the CPPFLAGS definition.
#

CC              = cc
CXX             = CC
DLD             = $(CXX)
LD              = $(CXX) 


CPPFLAGS        += -ptall -O +pp -D_SGI_MP_SOURCE -woff 3203,3209,3161,3262,3665

##CPPFLAGS      += -ptused -prelink +pp -D_SGI_MP_SOURCE -woff 3203,3209,3161,3
2
62,3665
LDFLAGS         += -rpath "$(WRAPPER_ROOT)/ace"
### LDFLAGS             += -rpath "$(WRAPPER_ROOT)/ace" -Wl,-woff,85
LIBS            += -lpthread
PIC             = -KPIC
AR              = ar
ARFLAGS         = r
RANLIB          = echo
SOFLAGS         = -shared $(CPPFLAGS) -all
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                 $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
