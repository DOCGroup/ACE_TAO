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
CXX_MAJOR_VERSION := $(firstword $(subst ., ,$(CXX_VERSION)))

# clang 16 and newer default to C++17
ifeq ($(CXX_MAJOR_VERSION),$(filter $(CXX_MAJOR_VERSION),4 5 6 7 8 9 10 11 12 13 14 15))
  c++std ?= c++17
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

ifneq ($(c++std),)
  CCFLAGS += -std=$(c++std)
else ifeq ($(c++20),1)
  CCFLAGS += -std=c++20
else ifeq ($(c++17),1)
  CCFLAGS += -std=c++17
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

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif
