# $Id$

# This file is for use with AIX 4.3 and up, using either of the IBM
# compilers (IBM C/C++ 3.6.6 or Visual Age C++ 5 batch compiler).
# If you wish to use the CSet++ compiler, please use platform_aix4_cset++.GNU.
# Note that as of ACE 6.0, the only supported compiler from IBM will
# be Visual Age C++ 5 and up (either batch or incremental).

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
  optimize = 0
endif
ifeq (,$(rtti))
  rtti = 0
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(buildbits))
  buildbits = 32
endif

# First, find out which compiler we're building with. The settings we
# use for compiling, as well as how to build shared libraries, depend on
# the compiler version. There's no easy way to ask lslpp which is installed.
# Instead, compile a file and see which compiler is set up for use. This
# trick was submitted by Craig Rodrigues <rodrigc@mediaone.net>, originally
# from the vacpp compiler newsgroup.
# It relies on the preprocessor defining __xlC__ to the proper version
# number of the compiler.
XLCVERSION := $(shell echo "__xlC__" > ./testAIXCompilerVersion.cpp)
XLCVERSION := $(shell xlC -E ./testAIXCompilerVersion.cpp | tail -1')
ACE_AIX_CLEAN := $(shell $(RM) ./testAIXCompilerVersion.cpp)

# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)
AIX_VERS := $(AIX_MAJOR_VERS)0$(AIX_MINOR_VERS)

# Use of exceptions should be a compiler-specific setting, but for now it's
# not. IBM C/C++ doesn't have a way to turn exceptions off, and Visual
# Age C++ V5 building ACE without exception handling causes segv on startup
# of every test. Until this is straightened out, force exceptions=1 for
# both compilers.
exceptions=1

# Yes, believe it or not...
# The way to make a shared lib on AIX is to make a shared object file and
# then put that in an archive.
# Non-shared archives are named lib<x>ns.a - ns = non-shared.
SOEXT       = a
# aix_build turns on the funky shared library building code in
# rules.lib.GNU
aix_build = 1
SHR_FILTER=$(ACE_ROOT)/bin/aix_shr

ifeq ($(threads),1)
# Note - the _r form of the compiler command turns on -qthreads and the
# thread-required preprocessor defines, as well as -lpthreads for linking.
# If this is Visual Age C++ 5, DLD will get reset below.
CC	    = xlc_r
CXX	    = xlC_r
DLD	    = makeC++SharedLib_r
else
CC          = xlc
CXX         = xlC
DLD	    = makeC++SharedLib
endif

ARFLAGS	    = ruv
AR	    = ar
LD          = $(ACE_ROOT)/bin/aix_shr $(CXX)
RANLIB	    = ranlib
TEMPINCDIR  = tempinc

CPPFLAGS    += -DACE_AIX_VERS=$(AIX_VERS)
# -qinitauto seems like an interesting option, but some tests (specifically
# Process_Strategy_Test) hang with it turned on. This should be investigated
# at some point.
DCFLAGS     += -g -qcheck=nobounds:div:null

# Now sort out the compiler differences here...
ifeq ($(XLCVERSION),0x0306)
 CCFLAGS    += -qlanglvl=ansi -qflag=e:e
 SOFLAGS     = -p 0
# ibmcxx_build turns on the extra step for instantiating and compiling
# template instances in rules.lib.GNU.
 ibmcxx_build = 1
 rtti = 0
else
 ifeq ($(XLCVERSION),0x0500)
  CCFLAGS    += -qflag=w:w -qstaticinline
  DLD         = $(CXX) -qmkshrobj
  SOFLAGS     = $(CCFLAGS) $(CPPFLAGS) $(INCLDIRS)
  ifeq ($(buildbits),64)
    DLD     += -q64
    CCFLAGS += -q64
    ARFLAGS := -X64 $(ARFLAGS)
  endif
 else
  CXX = echo "Unrecognized compiler version $(XLCVERSION)\n"
 endif
endif

# -qextchk is handy, but produces false type mismatches when linking
# netsvcs, so it's disabled.  IBM reports this fixed in 3.6.4, but it's
# still producing the warnings... should be looked at.
CCFLAGS     += $(CFLAGS) -qonce -qtempinc
ifeq ($(exceptions),0)
CCFLAGS     += -qnoeh
endif

ifeq (1,$(rtti))
  CCFLAGS += -qrtti=dynamiccast
else  # ! rtti
  CCFLAGS += -DACE_LACKS_RTTI
endif # ! rtti

ifeq ($(shared_libs),1)
ACELIB      = -lACE
else
ACELIB      = -lACEns
endif

LIBS	    += $(ACELIB) -ltli_r -ldl

# Default OCCFLAGS builds common code for all RS/6000 architectures but
# this can be set to optimize for your particular computer. The commented
# out example optimizes for RS/6000 43P.
# OCCFLAGS is not used by default.  To used it, set optimize=1
# either in this file or on the command line.
#OCCFLAGS    += -qarch=ppc -qtune=604

# -O2 is too much for Visual Age C++ 5.0.  It ends up calling
# the wrong methods in some cases (orbsvcs/tests/EC_Throughput).
# Chad Elliott 7/24/2001
OCCFLAGS    += -qlibansi -O -qarch=com 
