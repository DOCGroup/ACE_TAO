# SunOS 5.x (Solaris 2.x) with SunC++ 4.0.1 or earlier
# *not* using Orbix
CC		= cc
CXX		= CC +w -mt -pta -noex # This option should solve some evil problems with Solaris
DLD		= $(CXX) 
LD		= $(CXX)
LDFLAGS		+= -R $(WRAPPER_ROOT)/ace 
LIBS		+= -lsocket -ldl -lnsl -lgen # -lposix4
PIC		= -PIC
AR		= CC
ARFLAGS		= -xar -o
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
