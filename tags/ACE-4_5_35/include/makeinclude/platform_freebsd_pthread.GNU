# $Id$

# platform_freebsd_pthread.GNU

optimize = 1

# For pthreads support on FreeBSD, you need

debug = 1
versioned_so = 1

CC              = gcc
CXX		= g++
CFLAGS          += -Wall -Wpointer-arith -pipe -D_THREAD_SAFE
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = $(CXX)
LD		= $(CXX) -pthread
LIBS            += -lstdc++ -lcompat
OCFLAGS         += -O
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
# SOFLAGS = -Bshareable -x /usr/lib/c++rt0.o
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
