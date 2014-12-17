# -*- Makefile -*-
# $Id$

# platform_openbsd.GNU

debug ?= 1
optimize ?= 1
threads ?= 1

CC              = gcc
CXX             = g++

AR              = ar
ARFLAGS         = ruv
CPPFLAGS        += -I/usr/local/include
DCFLAGS         += -g
DCCFLAGS        += $(DCFLAGS)
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += -L/usr/local/lib
OCFLAGS         += -O2
OCCFLAGS        = $(OCFLAGS)
PIC             = -fpic
PRELIB          = @true
RANLIB          = ranlib
SOFLAGS         += -shared

ifeq ($(threads),1)
  CPPFLAGS += -D_THREAD_SAFE
  FLAGS_C_CC += -pthread
  SOFLAGS += -pthread
endif # threads

PLATFORM_X11_CPPFLAGS = -I/usr/X11R6/include
PLATFORM_X11_LIBS = -lX11
PLATFORM_X11_LDFLAGS = -L/usr/X11R6/lib

PLATFORM_XT_CPPFLAGS =
PLATFORM_XT_LIBS = -lXt
PLATFORM_XT_LDFLAGS =

PLATFORM_FL_CPPFLAGS =
PLATFORM_FL_LIBS = -lfltk -lfltk_forms -lfltk_gl
PLATFORM_FL_LDFLAGS =

PLATFORM_GL_CPPFLAGS = -I/usr/X11R6/include
PLATFORM_GL_LIBS = -lGL
PLATFORM_GL_LDFLAGS = -L/usr/X11R6/lib

PLATFORM_GTK_CPPFLAGS = $(shell gtk-config --cflags)
PLATFORM_GTK_LIBS = $(shell gtk-config --libs)
PLATFORM_GTK_LDFLAGS =

PLATFORM_FOX_CPPFLAGS ?= -I/usr/include/fox
PLATFORM_FOX_LIBS ?= -lFOX
PLATFORM_FOX_LDFLAGS ?=

PLATFORM_TK_CPPFLAGS=$(shell . /usr/local/lib/tcl/tk*/tkConfig.sh && echo -n $$TK_INCLUDE_SPEC) $(PLATFORM_X11_CPPFLAGS)
PLATFORM_TK_LIBS=$(shell . /usr/local/lib/tcl/tk*/tkConfig.sh && echo -n $$TK_LIB_FLAG) $(PLATFORM_X11_LIBS)
PLATFORM_TK_LDFLAGS=$(PLATFORM_X11_LDFLAGS)

PLATFORM_TCL_CPPFLAGS=$(shell . /usr/local/lib/tcl/tcl*/tclConfig.sh && echo -n $$TCL_INCLUDE_SPEC)
PLATFORM_TCL_LIBS=$(shell . /usr/local/lib/tcl/tcl*/tclConfig.sh && echo -n $$TCL_LIB_FLAG)
PLATFORM_TCL_LDFLAGS=

# Test for template instantiation, add to versioned_so if versioned_so set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU
