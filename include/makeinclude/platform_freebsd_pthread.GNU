# platform_freebsd_pthread.GNU

# ***** Notice: This configuration is still under debugging. *****
# ***** It still doesn't work quite the way it expected to.  *****

# For pthreads support on FreeBSD, you need
# -D_POSIX_THREADS
# You must have the latest pthread library by John Birrell.
# and link with reentrant library libc_r.  If you can't find this
# library (libc_r.so) in your system, do a "man pthread" and follow the
# instruction to build your own.  You must have at least 3.0-current as
# of 2/9/1997 to take advantage of the latest feature of pthread library.

CC              = gcc
CXX		= g++
CFLAGS          += $(DCFLAGS) -w -O2 -fno-strict-prototypes -D_POSIX_THREADS
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = ld
LD		= $(CXX) 
LIBS            += -lc_r -lstdc++ -lcompat
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

# SOFLAGS = $(CPPFLAGS) -shared
SOFLAGS = -Bshareable -x
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo
