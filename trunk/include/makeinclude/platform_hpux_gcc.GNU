# $Id$
#

debug = 1

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC		= gcc
CXX		= g++
CFLAGS		+= -w -O2
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g -DHPUX_VERS=$(HPUX_VERS)
DLD		= $(CXX)
LD		= $(CXX)
PIC		= -fPIC
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L$(ACE_ROOT)/ace -Wl,+s
LIBS		+= -lxti
SOFLAGS		= $(CPPFLAGS) -shared -nostdlib
SOEXT		= sl
SOBUILD 	= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
SHLIBBUILD 	= $(SOLINK.cc) -o $@ $(VSHOBJS1)
PRELIB = @true
