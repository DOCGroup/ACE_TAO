# SunOS 5.x (Solaris 2.x) with SunC++ 4.1
# *not* using Orbix
CC              = cc
CXX             = CC -mt -noex -pta
DLD             = $(CXX) 
LD		= $(CXX)
LDFLAGS         += -R $(WRAPPER_ROOT)/ace 
LIBS            += -lsocket -ldl -lnsl -lgen
PIC             = -PIC
AR              = CC
ARFLAGS         = -xar -o
RANLIB          = echo
SOFLAGS         = -G $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
