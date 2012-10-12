# $Id$

# This file should allow ACE to be built on Linux, using the clang compiler.

include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

ifeq ($(insure),0)
  CC  = clang
  CXX = clang++
endif

ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) --version)
endif

CCFLAGS += $(CFLAGS)
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl

ifeq ($(threads),1)
  LIBS += -lpthread
  LIBS += -lrt
endif

OCFLAGS += -O3

ifeq ($(optimize),0)
  CPPFLAGS += -O0
endif

ifeq ($(c++0x),1)
  CCFLAGS += -std=c++0x
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

# Visibility doesn't seem to work with clang 2.8, default to off
no_hidden_visibility ?= 1

ifeq ($(shared_libs), 1)
  ifneq ($static_libs_only), 1)
    LDFLAGS += -Wl,-E
    ifneq ($(no_hidden_visibility),1)
      CCFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
    else
      CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
    endif # no_hidden_visibility
  endif
endif


# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif
