# SunOS 5.x (Solaris 2.x) with SunC++ 4.0.1 or earlier
# *using* Orbix
CC              = cc
CXX             = CC
CFLAGS          += $(DCFLAGS) +w -mt
CCFLAGS         += $(CFLAGS) -noex -pta
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
INCLDIRS	= -I$(ORBIX_ROOT)/include
PIC		= -PIC
LDFLAGS		+= $(PIC) -L$(ORBIX_ROOT)/lib -R $(WRAPPER_ROOT)/ace -R $(ORBIX_ROOT)/lib
LIBS		+= -lITsrv -lsocket -ldl -lnsl -lthread -lgen
AR		= CC
ARFLAGS		= -xar -o
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
