# $Id$

# For pthreads support on Linux, you need
# -D_MIT_POSIX_THREADS 
# -D_POSIX_THREADS
# -D_POSIX_THREAD_SAFE_FUNCTIONS 
# in the CXX command line. Also, add -lpthreads to the LIBS.
# libpthreads.so comes with the sources of Linux libc-5.3.*, you need
# to compile it yourself (no binaries included) -- 
# Jan Rychter <jwr@icm.edu.pl>

debug = 1

CC              = gcc
CXX		= g++
CFLAGS		+= -w -O2 -fno-strict-prototypes -D_MIT_POSIX_THREADS -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS
CCFLAGS		+= $(CFLAGS) -fno-implicit-templates
DCFLAGS		+= -g
DLD             = $(CXX)
LD		= $(CXX) 
LIBS            += -lpthreads
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true
