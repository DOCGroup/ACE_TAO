# $Id$

# SunOS 5.x (Solaris 2.x) with g++ *not* using Orbix

debug = 1

CC		= gcc 
CXX		= g++
CCFLAGS         += $(CFLAGS) -fno-implicit-templates -fno-strict-prototypes
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
LDFLAGS		+=     # -z muldefs
LIBS		+= -lsocket -ldl -lgen -lnsl -lthread -lw
PIC		= -fpic
AR		= ar
ARFLAGS		= ruv
RANLIB		= /bin/true
SOFLAGS		= -G $(CPPFLAGS)
# SOBUILD - compile into .so directly
SOBUILD 	= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK 	= cp $< $@
# SOLINK 	= ln -s $< $@
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB		= @true
