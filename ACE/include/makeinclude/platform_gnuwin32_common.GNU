# -*- Makefile -*-
# $Id$

ifneq ($(mingw32),1)
  ifneq ($(cygwin32),1)
    fail := \
      $(shell \
          echo "You should not use this file but one of"; \
          echo "platform_mingw32.GNU or platform_cygwin32.GNU"; \
          exit 1)
  endif
endif

no_hidden_visibility ?= 1

winsock2 ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1
pipes ?= 1

cpumodelflag ?= 0
tunemodelflag ?= 1

# When building shared libraries
ifeq ($(shared_libs), 1)
  ifneq ($(static_libs_only), 1)
    inline ?= 0
  endif
endif

ifeq ($(debug),0)
  CFLAGS += -DNDEBUG
endif

ifeq ($(threads),1)
  CFLAGS += -mthreads
endif # threads

CC      = ${COMPARCH}gcc
CXX     = ${COMPARCH}g++
RC      = ${COMPARCH}windres
DLLTOOL = ${COMPARCH}dlltool
AR      = ${COMPARCH}ar
RANLIB  = ${COMPARCH}ranlib

CFLAGS  += -Wall -W -Wpointer-arith
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
OCFLAGS += -O3
PIC     =
ARFLAGS = rsuv
SOEXT   = dll
EXEEXT  = .exe
SOFLAGS += $(CPPFLAGS)
RC_OUTPUT_FLAG = -o

SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<

SHLIBBUILD = $(DLLTOOL) --dllname $@ --output-lib $@.a $(VSHOBJS) && \
  $(SOLINK.cc) -Wl,--enable-auto-image-base -Wl,--out-implib,$@.a \
  -shared -o $@ $(LDFLAGS) $(VSHOBJS) $(ACE_SHLIBS) $(LIBS)

PLATFORM_FL_CPPFLAGS  ?=
PLATFORM_FL_LIBS      ?= -lfltk -lfltk_forms -lfltk_gl -lfltk_images
PLATFORM_FL_LDFLAGS   ?=

PLATFORM_GL_CPPFLAGS  ?= -I/usr/X11R6/include
PLATFORM_GL_LIBS      ?= -lGL
PLATFORM_GL_LDFLAGS   ?= -L/usr/X11R6/lib

PLATFORM_WX_CPPFLAGS  ?= $(shell wx-config --cxxflags) -I/usr/local/include
PLATFORM_WX_LIBS      ?= $(shell wx-config --libs)
PLATFORM_WX_LDFLAGS   ?= $(shell wx-config --ldflags)

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

PRELIB  = @true

CCFLAGS += $(CFLAGS) $(FLAGS_C_CC) $(TEMPLATES_FLAG)
