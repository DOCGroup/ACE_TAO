# $Id$

# SunOS 4.x (Solaris 1.x) with Lucid
CC		= lcc
CXX		= lcc
CFLAGS		= $(DCFLAGS) -Xa # use ANSI c complier
CCFLAGS		= $(DCCFLAGS) -XF -Xpt- # use c++ ARM compiler with auto templates turned off
DCFLAGS		+= -g
DCCFLAGS	+= $(DCFLAGS)
DLD		= /bin/ld
LD		= $(CXX)
LIBS		+= 
PIC		= -pic
ARFLAGS		= ruv
RANLIB		= ranlib
SOFLAGS		= -assert pure-text 
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
