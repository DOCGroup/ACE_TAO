# $Id$

# For pthreads support on Linux, you need
# -D_POSIX_THREADS
# -D_POSIX_THREAD_SAFE_FUNCTIONS
# in the CXX command line. Also, add -lpthreads to the LIBS.
# libpthreads.so comes with the sources of Linux libc-5.3.*, you need
# to compile it yourself (no binaries included) --
# Jan Rychter <jwr@icm.edu.pl>

debug = 1
optimize = 1
xt_reactor = 0

PLATFORM_XT_CPPFLAGS=
PLATFORM_XT_LIBS=-lXt
PLATFORM_XT_LDFLAGS=

fl_reactor = 0

PLATFORM_FL_CPPFLAGS=
PLATFORM_FL_LIBS=-lfltk
PLATFORM_FL_LDFLAGS=

PLATFORM_X11_CPPFLAGS=
PLATFORM_X11_LIBS=-lX11
PLATFORM_X11_LDFLAGS=-L/usr/X11R6/lib

PLATFORM_GL_CPPFLAGS=
PLATFORM_GL_LIBS=-lMesaGL
PLATFORM_GL_LDFLAGS=

ifeq ($(NO_IMPLICIT_TEMPLATES),)
      NO_IMPLICIT_TEMPLATES = -fno-implicit-templates
endif

CC      = gcc
CXX     = g++
CFLAGS  += -Wall -pipe \
           -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT
CCFLAGS += $(CFLAGS) $(NO_IMPLICIT_TEMPLATES)
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl -lpthread
OCFLAGS += -O3
PIC     = -fPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
ifndef PRELIB
  PRELIB = @true
endif # ! PRELIB


#### GNU gas has a string limit of 4096 characters.  On Alphas,
#### builds will fail due to running over that limit.  There are
#### at least two workarounds:
#### 1) Change the limit to 8192 characters and rebuild gas.  See
####    ACE-INSTALL.html for more information.
#### 2) Don't use -g when compiling those files.
#### If you're building on an Alpha and you haven't hacked and
#### rebuilt gas, you might need to uncomment the following.
#### ifeq ($(debug),1)
####   SUPPRESS_DASH_G = 1
#### endif # debug
