# $Id$
#
# Platform macros for building on AIX with g++.  This has been tested on
# AIX 4.1.4 and gcc 2.7.2.3.
#
# A note - later versions of gcc(>2.7.2.3, I don't know which) have a -mthreads
# flag which does the same job more elegantly.
# I haven't managed to get the shared library to work.
# I know having tli_r and tli is a bad idea, but it won't link otherwise.
#
# Contributed by Ian Wright <I.Wright@elsevier.co.uk>

debug = 1
static_libs_only = 1
 
CC              = gcc
CXX             = gcc
CFLAGS          += -W -Wall -nostdlib -mcpu=common
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
#only needed for one of the tests and causes compiler errors in building libACE
#CCFLAGS         += -fhandle-exceptions
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
OCFLAGS         = -O2
OCCFLAGS        = $(OCFLAGS)
LIBS            += /lib/crt0_r.o -L/usr/lib/threads -lsvld -lC_r -lpthreads
-lc_r -ltli_r -lg++ -lstdc++ -lm -lg -ltli -lgcc
PIC             = -fPIC
ARFLAGS         = cruv
RANLIB          = ranlib
