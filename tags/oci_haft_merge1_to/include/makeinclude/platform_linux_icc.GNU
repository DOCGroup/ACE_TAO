# $Id$

# According to Roger Tragin <r.tragin@computer.org>, this file
# should allow ACE to be built on Linux, using the Intel compiler.
#
# This is just a copy of platform_linux.GNU, with a few things changed.

# debug = 1
# optimize = 1

ifndef exceptions
  exceptions = 1
endif
ifeq ($(exceptions),1)
  CPPFLAGS += -Kc++eh
endif # exceptions
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
LATFORM_XT_LDFLAGS=

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

#
# Intel compiler comes in two flavors with different binaries. The first, 
# icc, is for IA32 platforms and the second, ecc, is for IA64 platforms. 
# They share most options except those influencing target machine code 
# generation, therefore we should pick sensible defaults for both
# depending on system architecture.
#
SYSARCH := $(shell uname -m)

ifeq ($(insure),1)
CC  = insure
CXX = insure
else
  ifeq ($(SYSARCH),ia64)
    CC  = ecc
    CXX = ecc
  else
    CC  = icc
    CXX = icc
  endif
endif

CFLAGS  += -w1
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads


CCFLAGS += $(CFLAGS) $(IMPLICIT_TEMPLATES_FLAG) -ip
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
ifneq ($(SYSARCH),ia64)
  OCFLAGS += -axK
endif

PIC     = -KPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS += $(CPPFLAGS) -shared
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
