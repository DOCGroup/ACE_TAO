# $Id$

# Requires FSU pthreads and gcc 2.7.2 Skunkware 97
# to produce libACE.so correctly changed the specs file for gcc 2.7.2
# for not link -lc and -lgcc and other not shared libraries

debug = 1
optimize = 1

CC              = cc
# for threads
# for no threads
#CXX		= gcc -fno-implicit-templates -w -fno-strict-prototypes
# LDFLAGS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o # -z muldefs
# LDLIBS	+= -L $(ACE_ROOT)/ace -L ./ ${FSU_PTHREADS_DIR}/init/init.o -lACE # -z muldefs
LDLIBS	+= -L $(ACE_ROOT)/ace # -z muldefs
CXX		= CC
CFLAGS		+= -belf
CCFLAGS		+= +.cpp +d -Dvolatile= -belf -Kpic -pts -I ${FSU_PTHREADS_DIR}/include
DCFLAGS		+= -g
DCCFLAGS	+= $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX) /usr/lib/libc.so.1
#LD              = $(CXX)
LIBS            = -lm -L${FSU_PTHREADS_DIR}/lib -lgthreads -lnsl -lsocket  -lgthreads
OCFLAGS		+= -O2
OCCFLAGS        += $(OCFLAGS)
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

SOFLAGS += $(CPPFLAGS) -melf -fPIC -G
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB  = @true
