# $Id$
#
# Platform macros for building on AIX 4.x with g++.  This has been tested on
# AIX 4.3 with egcs 1.1.2 and gcc 2.95.2.
#
# Original contributed by Ian Wright <I.Wright@elsevier.co.uk>. Updated for
# AIX 4 and egcs by Steve Huston <shuston@riverace.com>.


ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(distrib))
  distrib = 0
endif
ifeq (,$(optimize))
  optimize = 0
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(exceptions))
  exceptions = 0
endif

ifeq (0,$(threads))
  CFLAGS  += -DACE_MT_SAFE=0
endif
ifeq (1,$(threads))
  CFLAGS  += -mthreads
endif
 
# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)
AIX_VERS := $(AIX_MAJOR_VERS)0$(AIX_MINOR_VERS)

CC             = gcc
CXX            = g++
CFLAGS         += -W -Wall -pipe -mcpu=common -DACE_AIX_VERS=$(AIX_VERS)
CCFLAGS        += $(CFLAGS) -fno-implicit-templates
DCFLAGS        += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS	       += -Wl,-bI:/lib/pse.exp
# Linking TAO_IDL runs out of TOC space unless -bbigtoc is given to ld.
# Don't do it for everything because it decreases run-time performance.
ifeq ($(notdir $(shell pwd)), TAO_IDL)
LDFLAGS        += -Wl,-bbigtoc
endif
OCFLAGS         = -O2
OCCFLAGS        = $(OCFLAGS)
ifeq ($(threads),1)
SOFLAGS        += -mthreads
endif
SOFLAGS        += -shared -Wl,-bexpall # -Wl,-bloadmap:lib.map
LIBS           += -ltli_r -ldl
ARFLAGS         = cruv
RANLIB          = ranlib

# On AIX, shared libraries are not generally left in .so form to link against.
# To do so requires the run-time linker and that is an unnecessary evil.
# So build the .so as usual, but then put in an archive library to link
# against. The archive lib is the one installed for run-time use also, though
# if you want to use ACE's dynamic loading features, you'll also need the
# .so file.
ifeq ($(shared_libs),1)
ACELIB          = -lACEshr
SHLIBA          = $(SHLIB:lib%.so=lib%shr.a)
endif
