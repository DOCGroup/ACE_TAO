# $Id$

# platform_freebsd_pthread.GNU

optimize = 1

# For pthreads support on FreeBSD, you need

debug = 1
versioned_so = 1

CC              = gcc
CXX		= g++
CFLAGS          += -w -fno-strict-prototypes -D_THREAD_SAFE
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = ld
LD		= $(CXX)
LIBS            += -lc_r -lstdc++ -lcompat
OCFLAGS         += -O2
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

# SOFLAGS = $(CPPFLAGS) -shared
SOFLAGS = -Bshareable -x /usr/lib/c++rt0.o
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
