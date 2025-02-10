# -*- Makefile -*-

include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

ifeq ($(insure),0)
  CC  ?= gcc
  CXX ?= g++
  CXX_FOR_VERSION_TEST ?= $(CXX)
else
  CXX_FOR_VERSION_TEST ?= g++
endif

ifndef CXX_FULL_VERSION
  CXX_FULL_VERSION := $(shell $(CXX_FOR_VERSION_TEST) --version)
endif
ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)
endif

ifeq ($(buildbits),32)
  FLAGS_C_CC += -m32
  LDFLAGS    += -m32
endif
ifeq ($(buildbits),64)
  FLAGS_C_CC += -m64
  LDFLAGS    += -m64
endif

ifeq ($(dynamic_loader),0)
  CPPFLAGS += -DACE_HAS_DYNAMIC_LINKING=0
else
  LIBS     += -ldl
endif

ifeq ($(threads),1)
  FLAGS_C_CC += -pthread
  SOFLAGS += -pthread
  LIBS += -lrt
endif

ifeq ($(static_stdlibs),1)
  LDFLAGS += -static-libgcc -static-libstdc++
endif

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

OCFLAGS ?= -O3
OCCFLAGS ?= -O3

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
