# SunOS 5.x (Solaris 2.x) with SunC++ 4.x
# *not* using Orbix
CC		= gcc
#CXX		= ec++ -relax -tproff
CXX		= ec++ -relax -v -eh
DLD		= $(CXX) 
#LDFLAGS	+= -R $(WRAPPER_ROOT)/ace 
LDFLAGS		+=
LIBS		+= -lgen -lsocket -ldl -lnsl -lc
PIC		= -KPIC
AR		= ar
ARFLAGS		= -ruv
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
#SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
#		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
