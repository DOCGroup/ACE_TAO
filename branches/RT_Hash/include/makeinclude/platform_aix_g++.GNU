# $Id$
#
# Platform macros for building on AIX with g++.  This has been tested on
# AIX 4.3 with gcc 3. Earlier versions of gcc may compile, but are known to
# have serious problems on AIX.
#
# Contributed by Ian Wright <I.Wright@elsevier.co.uk>
# Updated for AIX 4.3 and gcc 3 by Steve Huston  <shuston@riverace.com>

ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(distrib))
  distrib = 0
endif
ifeq (,$(exceptions))
  exceptions = 1
endif
ifeq (,$(optimize))
  optimize = 1
endif
ifeq (,$(rtti))
  rtti = 0
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(buildbits))
  buildbits = native
endif
ifeq (,$(with_ld))
  with_ld = aix
endif

# Yes, believe it or not...
# The way to make a shared lib on AIX is to make a shared object file and
# then put that in an archive.
# Non-shared archives are named lib<x>ns.a - ns = non-shared.
SOEXT       = a
# aix_build turns on the funky shared library building code in
# rules.lib.GNU
aix_build = 1

ifeq (0,$(threads))
  CFLAGS  += -DACE_MT_SAFE=0
endif
ifeq (1,$(threads))
  CFLAGS  += -pthread
endif

# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)
AIX_VERS := $(AIX_MAJOR_VERS)0$(AIX_MINOR_VERS)

CC             = gcc
CXX            = g++

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CFLAGS          += -W -Wall -mcpu=common -DACE_AIX_VERS=$(AIX_VERS)
ifeq ($(buildbits),32)
  CFLAGS    += -m32
endif
ifeq ($(buildbits),64)
  CFLAGS    += -m64
endif
CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
# Linking TAO_IDL runs out of TOC space unless -bbigtoc is given to ld.
# Don't do it for everything because it decreases run-time performance.
ifeq ($(notdir $(shell pwd)), TAO_IDL)
LDFLAGS        += -Wl,-bbigtoc
endif

OCFLAGS         = -O2
OCCFLAGS        = $(OCFLAGS)
ifeq ($(threads),1)
SOFLAGS        += -pthread
endif
SOFLAGS        += -shared -Wl,-bexpall # -Wl,-bloadmap:lib.map
LIBS           += -ltli_r -ldl
ARFLAGS         = cruv
RANLIB          = ranlib
SONAME		=
SOVERSION	=
