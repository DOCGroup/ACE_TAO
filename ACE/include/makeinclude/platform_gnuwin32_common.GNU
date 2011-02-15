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

# Choose your target CPU (by default we set it to pentiumpro. In your
# platform_macros.GNU file you can override this to pentium, i486 or i386
TCPU ?= pentiumpro

# Cygwin/MinGW doesn't allow to add the version number of ACE to the dll name
# because ace.dll.6.0.1 isn't a valid dll name
versioned_so = 0

# Cygwin/MinGW doesn't have rwho
rwho = 0

# Disable auto-import warnings. The Cygwin/MinGW linker has a problem
# with imports
# See https://sourceforge.net/tracker/?func=detail&atid=102435&aid=683455&group_id=2435
# for the details why to do this.
LDFLAGS += -Wl,--enable-auto-import

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
  CPPFLAGS += -DNDEBUG
endif

ifeq ($(threads),1)
  FLAGS_C_CC += -mthreads
endif # threads

CC      = ${COMPARCH}gcc
CXX     = ${COMPARCH}g++
RC      = ${COMPARCH}windres
DLLTOOL = ${COMPARCH}dlltool
AR      = ${COMPARCH}ar
RANLIB  = ${COMPARCH}ranlib

FLAGS_C_CC += -Wall -W -Wpointer-arith
DCFLAGS    += -g
DLD        = $(CXX)
LD         = $(CXX)
OCFLAGS    += -O3
PIC        =
ARFLAGS    = rsuv
SOEXT      = dll
EXEEXT     = .exe
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

PRELIB = @true

CCFLAGS += $(CFLAGS) $(FLAGS_C_CC)
