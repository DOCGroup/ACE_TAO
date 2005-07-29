# Hey Emacs, this is a -*- Makefile -*-
# $Id$

# platform_linux_borland.GNU

ifndef BCB
BCB = /usr/local/kylix3
endif

exceptions ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1

PLATFORM_TCL_CPPFLAGS=-I/usr/include/tcl8.4
PLATFORM_TCL_LIBS=-ltcl8.4
PLATFORM_TCL_LDFLAGS=

PLATFORM_TK_CPPFLAGS=-I$(shell . /usr/lib/tk8.4/tkConfig.sh && echo -n $$TK_INC_DIR $$TK_DEFS)
PLATFORM_TK_LIBS=$(shell . /usr/lib/tk8.4/tkConfig.sh && echo -n $$TK_LIB_FLAG)
PLATFORM_TK_LDFLAGS=

PLATFORM_AIO_SUPPORT := \
       $(shell test -e /usr/lib/librt.so && echo -DACE_HAS_AIO_CALLS)

ifeq ($(threads),1)
  LIBS += libpthread.so
  ifeq (-DACE_HAS_AIO_CALLS,$(PLATFORM_AIO_SUPPORT))
    CPPFLAGS += -DACE_HAS_AIO_CALLS
    LIBS += librt.so
  endif
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

# NOTE: we only support wxWindows over GTK
PLATFORM_WX_CPPFLAGS= $(shell wx-config --cxxflags) $(PLATFORM_GTK_CPPFLAGS)
PLATFORM_WX_LIBS    = $(shell wx-config --libs)     $(PLATFORM_GTK_LIBS)
PLATFORM_WX_LDFLAGS = $(shell wx-config --ldflags)  $(PLATFORM_GTK_LDFLAGS)

ifeq ($(insure),1)
CC  = insure
CXX = insure
else
CC  = $(BCB)/bin/bc++
CXX = $(BCB)/bin/bc++
endif

# Test for template instantiation.
#

#
# Common flags
#

ifdef debug
DCFLAGS = -v -y -Od -r- -vi- -D_DEBUG
DCCFLAGS = #-v -y -Od -r- -vi- -k -D_DEBUG
else
DCFLAGS = -DNDEBUG
DCCFLAGS =
endif

kylix = 1
INCLUDEPATH = $(BCB)/include/stlport:$(BCB)/include:$(BCB)/include/vcl:/usr/include:$(ACE_ROOT)
LIBPATH = $(BCB)/lib/obj:$(BCB)/lib:/usr/lib:/lib:/usr/X11R6/lib:$(BCB)/bin
ACELIBPATH = $(ACE_ROOT)/lib:./

AR      = ar
ARFLAGS = rsuv
RANLIB  = @true

DLD     = $(BCB)/bin/ilink
LD      = $(BCB)/bin/ilink

TAO_IDL_PREPROCESSOR = bcpp

WARNINGS_CFLAGS = -w-rvl -w-rch -w-ccc -w-obs -w-aus -w-pia -w-inl

CFLAGS = $(WARNINGS_CFLAGS) -q -VP -Vx -a8 -b- -k $(DCFLAGS) \
	-c -I$(INCLUDEPATH)

CPPFLAGS += #$(CFLAGS)
CCFLAGS += $(CFLAGS)
BORLDFLAGS = -c -L$(LIBPATH):$(ACELIBPATH) -x -Gn -v
BORINITEXEOBJ = borinit.o
BORINITSHAREDOBJ = borinitso.o
BORSHAREDOBJ += libborcrtl.so libborstl.so libborunwind.so libc.so libm.so libdl.so
BOREXELIBS += crt1.o
LIBS += $(BORSHAREDOBJ)

PRELINK=
SOFLAGS= -Tpd $(BORLDFLAGS)

LINK.cc = override
LINK.cc.override = $(LD) $(LDFLAGS) $(BOREXELIBS) $(BORLDFLAGS) $(POSTLINK)
SOLINK.cc = $(LD) -Tpd

versioned_so=0
