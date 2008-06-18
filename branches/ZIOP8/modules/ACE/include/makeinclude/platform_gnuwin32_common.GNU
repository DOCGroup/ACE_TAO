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

GNUWIN32_CXX_VERSION := $(shell $(CXX) -dumpversion)

winsock2 ?= 1
exceptions ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1
pipes ?= 1

# When building shared libraries
ifeq ($(shared_libs), 1)
  ifneq ($(static_libs_only), 1)
    inline ?= 0
  endif
endif

ifeq ($(debug),0)
CFLAGS += -DNDEBUG
endif

CC      = gcc
CXX     = g++
RC      = windres

ifeq (2.9,$(findstring 2.9,$(GNUWIN32_CXX_VERSION)))
  MODEL_FLAGS += -mcpu=$(TCPU)
else
  MODEL_FLAGS += -mtune=$(TCPU)
endif

CFLAGS += -Wpointer-arith
ifeq ($(threads),1)
  CFLAGS += -mthreads
endif # threads

CFLAGS += $(MODEL_FLAGS)

EXEEXT = .exe

DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)

ifeq ($(mingw32),1)
ifeq ($(winsock2),1)
LIBS    += -lws2_32 -lmswsock
endif
LIBS    += -lwsock32 -lnetapi32
endif

OCFLAGS += -O3
PIC        =
AR         = ar
ARFLAGS    = rsuv
RANLIB     = ranlib
SOEXT      = dll
SOFLAGS    += $(MODEL_FLAGS) $(CPPFLAGS)
SOBUILD    = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<

PLATFORM_FL_CPPFLAGS  ?=
PLATFORM_FL_LIBS      ?= -lfltk -lfltk_forms -lfltk_gl -lfltk_images
PLATFORM_FL_LDFLAGS   ?=

PLATFORM_GL_CPPFLAGS  ?= -I/usr/X11R6/include
PLATFORM_GL_LIBS      ?= -lGL
PLATFORM_GL_LDFLAGS   ?= -L/usr/X11R6/lib

PLATFORM_WX_CPPFLAGS  ?= $(shell wx-config --cxxflags) -I/usr/local/include
PLATFORM_WX_LIBS      ?= $(shell wx-config --libs)
PLATFORM_WX_LDFLAGS   ?= $(shell wx-config --ldflags)

# The sed below is to strip comments on the .def file,
# to workaround to a bug in ld.
SHLIBBUILD = \
  $(RM) $(VSHDIR)/$@.def.old $(VSHDIR)/$@.def; \
  dlltool --export-all --output-def $(VSHDIR)/$@.def --dllname $@ \
    $(VSHOBJS) \
  && mv $(VSHDIR)/$@.def $(VSHDIR)/$@.def.old \
  && sed 's/;.*$$//g' < $(VSHDIR)/$@.def.old > $(VSHDIR)/$@.def \
  && $(SOLINK.cc) -Wl,--enable-auto-image-base -Wl,--out-implib,$@.a \
    -shared -o $@ $(LDFLAGS) -Wl,$(VSHDIR)/$@.def \
    $(VSHOBJS) $(ACE_SHLIBS) $(LIBS)

PRELIB  = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

ifeq ($(GXX_4_OR_BETTER), 1)
  no_attributes ?= 1
  wall ?= 1
else
  no_attributes ?= 0
  wall ?= 0
endif

ifeq ($(no_attributes), 1)
  CFLAGS += -Wno-attributes
endif

ifeq ($(wall),1)
  CFLAGS += -Wall -W
endif

CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG)
