# $Id$
#
# Platform macros for building on AIX 4.x with g++.  This has been tested on
# AIX 4.3 with egcs 1.1.2.
#
# Original contributed by Ian Wright <I.Wright@elsevier.co.uk>. Updated for
# AIX 4 and egcs by Steve Huston <shuston@riverace.com>.

debug = 1
optimize = 0
threads = 1
 
# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)

CC             = gcc
CXX            = g++
CFLAGS         += -W -Wall -pipe -mcpu=common -DACE_AIX_MAJOR_VERS=$(AIX_MAJOR_VERS) -DACE_AIX_MINOR_VERS=$(AIX_MINOR_VERS)
ifeq ($(threads),1)
CFLAGS         += -mthreads
endif
CCFLAGS        += $(CFLAGS) -fno-implicit-templates
DCFLAGS        += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS	       += -Wl,-bI:/lib/pse.exp 
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
