# SunOS 4.x (Solaris 1.x) with SunC++ 4.x
CC              = cc
CXX             = CC
DLD             = $(CXX)
LD		= $(CXX)
LIBS            += 
AR              = CC
ARFLAGS         = -xar -o
PIC             = -pic
RANLIB          = ranlib
SOFLAGS         = -G $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
