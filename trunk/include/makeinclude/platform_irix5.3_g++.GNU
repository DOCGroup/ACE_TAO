# $Id$

# Irix 5.3 with GNU C++

debug = 1

CC              = gcc
CXX             = g++
CCFLAGS		+= $(CFLAGS) -O2 -fno-implicit-templates
DCFLAGS		+= -g
DLD             = $(CXX)
LD		= $(CXX) 
LDFLAGS         += -L$(ACE_ROOT)/ace
PIC             = -fpic
RANLIB          = echo
SOFLAGS         = -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o

