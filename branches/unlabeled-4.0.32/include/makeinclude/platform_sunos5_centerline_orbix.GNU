# SunOS 5.x (Solaris 2.x) with Centerline C++
CC		= cc
CXX		= CC -mt
DLD		= $(CXX) -mt 
INCLDIRS	= -I$(ORBIX_ROOT)/include
LDFLAGS		+= -L$(ORBIX_ROOT)/lib
LIBS		+= -lITsrv -lsocket -ldl -lnsl -lgen
PIC		= -PIC
AR		= CC
ARFLAGS		= -xar -o
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS) $(PTDIRS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) $(PIC) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
