# $Id$

# SunOS 4.x (Solaris 1.x) with Lucid

debug = 1

CC		= lcc
CXX		= lcc
CFLAGS		= -Xa # use ANSI c complier
CCFLAGS		= -XF -Xpt- # use c++ ARM compiler with auto templates turned off
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
