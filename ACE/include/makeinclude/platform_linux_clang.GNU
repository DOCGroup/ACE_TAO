# This file should allow ACE to be built on Linux, using the clang compiler.

include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

ifeq ($(insure),0)
  ifeq ($(CXX),g++)
    CC  = clang
    CXX = clang++
  endif
endif

ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) -dumpversion)
endif
ifeq (cmd,$(findstring cmd,$(SHELL)))
CXX_MAJOR_VERSION := $(firstword $(subst ., ,$(CXX_VERSION)))
else
CXX_MAJOR_VERSION := $(shell $(CXX) -dumpversion | sed -e 's/[^0-9\.]//g' | sed -e 's/\..*$$//')
endif

# clang5 has C++03 as default C++ version, enable this to be C++11
# the older clang versions all return 4.2.1 as part of dumpversion
ifeq ($(findstring $(CXX_MAJOR_VERSION),4),$(CXX_MAJOR_VERSION))
  c++11 ?= 1
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

ifeq ($(c++11),1)
  CCFLAGS += -std=c++11
endif

ifeq ($(no_deprecated),1)
  CCFLAGS += -Wno-deprecated-declarations
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

# Compiling TAO requires relaxing strict 2-phase name lookup rules:
CCFLAGS += -fdelayed-template-parsing

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif
