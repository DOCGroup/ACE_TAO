# $Id$

# Requires FSU pthreads and gcc 2.7.2 Skunkware 97
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries

debug = 1
optimize = 1

CC              = gcc
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -fno-strict-prototypes
# LDFLAGS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o # -z muldefs
# LDLIBS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o -lACE # -z muldefs
LDLIBS	+= -L $(ACE_ROOT)/ace # -z muldefs
CXX		= g++
CFLAGS		+= -melf -w
CCFLAGS		+= -melf -fpic -fno-implicit-templates -I ${FSU_PTHREADS_DIR}/include 
DCFLAGS		+= -gstabs
DCCFLAGS	+= $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX) /usr/lib/libc.so.1
LIBS            = -lm -L${FSU_PTHREADS_DIR}/lib -lgthreads -lnsl -lsocket -lmalloc
OCFLAGS		+= -O2
OCCFLAGS        += $(OCFLAGS)
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS = $(CPPFLAGS) -melf -fpic -G 
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB  = @true
