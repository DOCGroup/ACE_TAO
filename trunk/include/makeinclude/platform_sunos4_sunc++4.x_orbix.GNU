# SunOS 4.x (Solaris 1.x) with SunC++ 4.x and Orbix.
CC              = cc
CXX             = CC
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS)
DCFLAGS		+= -g
DLD             = $(CXX)
LD		= $(CXX)
LIBS            += 
INCLDIRS        = -I$(ORBIX_ROOT)/include
AR              = CC
LDFLAGS         += -L$(ORBIX_ROOT)/lib
ARFLAGS         = -xar -o
PIC             = -pic
RANLIB          = ranlib
SOFLAGS         = -G $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
