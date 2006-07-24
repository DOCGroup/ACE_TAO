# -*- Makefile -*-

# $Id$

# SunOS 5.x (Solaris 2.x) with g++

# NOTE: On Solaris86, you'll need to use GNU as instead of /usr/ccs/bin/as,
#       if you want -gstabs+ and -pipe support.

debug ?= 1
optimize ?= 1
threads ?= 1
exceptions ?= 1
pipes ?= 1

ifeq ($(exceptions),0)
  CFLAGS += -fno-exceptions
endif # exceptions

ifeq ($(shell /bin/uname -m),i86pc)
  #### gcc on Solaris86 doesn't use -g
  DCFLAGS         += -gstabs+
else  # ! i86pc
  DCFLAGS         += -g
endif # ! i86pc

CC              = gcc
CXX             = g++
CFLAGS          += -W -Wall -Wpointer-arith #### -Winline

ifeq ($(threads),1)
  CPPFLAGS      += -D_REENTRANT
endif

DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         +=
ifeq ($(shell /bin/uname -r),5.9)
  # Solaris 9 puts sendfile() in a separate library.
  LIBS          += -lsendfile
endif
ifeq ($(shell /bin/uname -r),5.10)
  # Solaris 10 puts sendfile() in a separate library.
  LIBS          += -lsendfile
endif
LIBS            += -lsocket -ldl -lgen -lnsl -lposix4 -lthread
OCFLAGS         += -O3
PIC             = -fPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
SOFLAGS         += -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)

PLATFORM_X11_CPPFLAGS= -I/usr/openwin/include -I/usr/dt/include
PLATFORM_X11_LIBS =-lX11
PLATFORM_X11_LDFLAGS=

# TODO: Fix the parameters
#PLATFORM_TCL_CPPFLAGS=-I/usr/include/tcl8.4
#PLATFORM_TCL_LIBS=-ltcl8.4
#PLATFORM_TCL_LDFLAGS=
#
#PLATFORM_TK_CPPFLAGS=-I$(shell . /usr/lib/tk8.4/tkConfig.sh && echo -n $$TK_INC_DIR $$TK_DEFS)
#PLATFORM_TK_LIBS=$(shell . /usr/lib/tk8.4/tkConfig.sh && echo -n $$TK_LIB_FLAG)
#PLATFORM_TK_LDFLAGS=

## Unfortunately, gcc 3.0 fails to many cpp files with
## optimization enabled.  Even -O causes the compiler
## to core dump on some files.
ifeq ($(shell $(CXX) --version),3.0)
  override optimize = 0
endif
