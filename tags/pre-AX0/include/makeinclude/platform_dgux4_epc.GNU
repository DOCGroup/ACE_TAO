# $Id$

# Digital Unix v.4.X using epc?
# (Apparently based on SunOS 5.x (Solaris 2.x) with SunC++ 4.x)

debug = 1

CC		= gcc
CXX		= ec++
## CCFLAGS	+= -relax -tproff
CCFLAGS		+= $(CFLAGS) -relax -v -eh
DCFLAGS		+= -g
DLD		= $(CXX) 
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
