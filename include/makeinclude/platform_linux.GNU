# $Id$

# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow ACE to be built on Linux.

# debug = 1
# optimize = 1

ifndef exceptions
  exceptions = 1
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 1
endif

ifeq (,$(threads))
  threads = 1
endif

ifeq ($(threads),0)
  CPPFLAGS += -DACE_MT_SAFE=0
endif # threads


PLATFORM_XT_CPPFLAGS=
PLATFORM_XT_LIBS=-lXt
PLATFORM_XT_LDFLAGS=

PLATFORM_FL_CPPFLAGS=
PLATFORM_FL_LIBS=-lfltk
PLATFORM_FL_LDFLAGS=

PLATFORM_X11_CPPFLAGS=-I/usr/X11R6/include
PLATFORM_X11_LIBS=-lX11
PLATFORM_X11_LDFLAGS=-L/usr/X11R6/lib

PLATFORM_GL_CPPFLAGS=-I/usr/X11R6/include
PLATFORM_GL_LIBS    =-lGL
PLATFORM_GL_LDFLAGS =-L/usr/X11R6/lib

PLATFORM_GTK_CPPFLAGS=$(shell gtk-config --cflags)
PLATFORM_GTK_LIBS    =$(shell gtk-config --libs)
PLATFORM_GTK_LDFLAGS=

PLATFORM_WX_CPPFLAGS= $(shell wx-config --cflags) $(PLATFORM_GTK_CPPFLAGS)
PLATFORM_WX_LIBS    = $(shell wx-config --libs)   $(PLATFORM_GTK_LIBS)
PLATFORM_WX_LDFLAGS =

PLATFORM_AIO_SUPPORT := \
       $(shell test -e /usr/lib/librt.so && echo -DACE_HAS_AIO_CALLS)

ifeq ($(insure),1)
CC  = insure
CXX = insure
else
CC  = gcc
CXX = g++
endif

CXX_VERSION := $(shell $(CXX) -dumpversion)

# If no option has been specified, try to enable implicit_templates based
# on the version of the compiler.
#
ifeq ($(implicit_templates),)
  ifeq (2.95,$(findstring 2.95,$(CXX_VERSION)))
    implicit_templates = 1
  else
    ifeq (3.,$(findstring 3.,$(CXX_VERSION)))
      implicit_templates = 1
    else
      ifeq (2.96,$(findstring 2.96,$(CXX_VERSION)))
        implicit_templates = 1
      else
        implicit_templates = 0
      endif
    endif
  endif
endif

# User didn't specify and we couldn't guess if this version of the compiler
# works fine with implicit template instantiation. Turn if off.
#
ifeq ($(implicit_templates),0)
  ifeq ($(IMPLICIT_TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    IMPLICIT_TEMPLATES_FLAG=-fno-implicit-templates
  endif
endif

CFLAGS  += -W -Wall -Wpointer-arith -pipe
ifeq ($(threads),1)
    CPPFLAGS  += -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT \
               $(PLATFORM_AIO_SUPPORT)
endif # threads

CCFLAGS += $(CFLAGS) $(IMPLICIT_TEMPLATES_FLAG)
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl

ifeq ($(threads),1)
  LIBS += -lpthread
  ifeq (-DACE_HAS_AIO_CALLS,$(PLATFORM_AIO_SUPPORT))
    LIBS += -lrt
  endif
endif

PLATFORM_QT_CPPFLAGS=-I$(QTDIR)/include
PLATFORM_QT_LIBS=-lqt
PLATFORM_QT_LDFLAGS=-L$(QTDIR)/lib

OCFLAGS += -O3
PIC     = -fPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif

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
