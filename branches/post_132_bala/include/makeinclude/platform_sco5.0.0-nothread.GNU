# $Id$
#
# NOTE: the above file from ACE 4.6 has been modified by Mike P. to get rid of
# thread related stuff until we can obtain fsu-threads library.

# Requires gcc 2.7.2 Skunkware 97
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries

debug = 1
optimize = 1

CC              = gcc
# for no threads
LDLIBS		+= -lm -L $(ACE_ROOT)/ace
CXX		= g++
CFLAGS		+= -melf -w

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS		+= -melf $(TEMPLATES_FLAG)
# -g option not supported for C++ on systems using the DWARF debugging format
#DCFLAGS		+= -g
DCCFLAGS	+= $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX) /usr/lib/libc.so.1
LIBS            = -lnsl -lsocket
OCCFLAGS        += $(OCFLAGS)
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS += $(CPPFLAGS) -melf -fPIC -G
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
