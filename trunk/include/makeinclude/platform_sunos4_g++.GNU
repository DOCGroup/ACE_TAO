# $Id$

# SunOS 4.x (Solaris 1.x) with g++ *not* using Orbix

debug = 1

CC	= gcc 
CXX	= g++
CFLAGS  += -O2 -fno-strict-prototypes
CCFLAGS += $(CFLAGS) -fno-implicit-templates
DCFLAGS	+= -g
DLD	= /bin/ld
LD	= $(CXX)
#
# for G++ v2.6.0
PIC	= -fpic
AR	= ar
ARFLAGS	= ruv
RANLIB	= ranlib
SOFLAGS	= -assert pure-text 
# SOBUILD - compile into .so directly
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB	= @true
