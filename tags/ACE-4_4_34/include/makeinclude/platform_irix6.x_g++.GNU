# $Id$

# Irix 6.2 with GNU C++

debug = 1
optimize = 1

CC              = gcc
CXX             = g++
CFLAGS		+= $(DCFLAGS)
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g
DLD             = $(CXX)
LD			= $(CXX) 
LDFLAGS         += -L$(ACE_ROOT)/ace -Wl,-dont_warn_unused
OCFLAGS		+= -O2
PIC             = -fpic
# AR		= ar
ARFLAGS		= rv
RANLIB          = true
SOFLAGS         = -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) $(LDFLAGS) -o $@ $(VSHDIR)$*.o
