# $Id$

# AIX 4.3 and higher using the Visual Age C++ 5 batch compiler (xlC_r)
# *not* using Orbix

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
ifeq (,$(threads))
  threads = 1
endif

# Note - Visual Age C++ pre-V5 didn't have a way to turn exception handling
# off, and V5 building ACE without exception handling causes segv on startup
# of every test. Until this is straightened out, force exceptions=1
exceptions=1

# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)

ifeq ($(threads),1)
# Note - the _r form of the compiler command turns on -qthreads and the
# thread-required preprocessor defines, as well as -lpthreads for linking.
CC	    = xlc_r
CXX	    = xlC_r
else
CC          = xlc
CXX         = xlC
endif
DLD	    = $(CXX) -qmkshrobj

# -qextchk is handy, but produces false type mismatches when linking
# netsvcs with 3.1.4, so it's disabled.  IBM reports this fixed in 3.6.4.
CCFLAGS     += $(CFLAGS) -qonce -qtempinc
ifeq ($(exceptions),0)
CCFLAGS     += -qnoeh
endif

CPPFLAGS    += -DACE_AIX_MAJOR_VERS=$(AIX_MAJOR_VERS) -DACE_AIX_MINOR_VERS=$(AIX_MINOR_VERS)
ifeq (1,$(rtti))
  CCFLAGS += -qrtti=dynamiccast
else  # ! rtti
  CCFLAGS += -DACE_LACKS_RTTI
endif # ! rtti

# Define options used for debug=1 builds.
# -qextchk still produces type mismatch errors... should look into this.
# -qcheck messes up ACE_Allocator::instance?   - no, seems ok
# How about -qinitauto?
DCFLAGS     += -g

LD          = $(CXX)

# Yes, believe it or not...
# The way to make a shared lib on AIX is to make a shared object file and
# then put that in an archive.
# Non-shared archives are named lib<x>ns.a - ns = non-shared.
SOEXT       = a

SOFLAGS = $(CCFLAGS) $(CPPFLAGS) $(INCLDIRS)

aix_build = 1
ifeq ($(shared_libs),1)
ACELIB      = -lACE
else
ACELIB      = -lACEns
endif

LIBS	    += -ltli_r -ldl

ARFLAGS	    = ruv
AR	    = ar
LDFLAGS	    += -bloadmap:vacpp.map
# Default OCCFLAGS builds common code for all RS/6000 architectures but
# this can be set to optimize for your particular computer. The commented
# out example optimizes for RS/6000 43P.
# OCCFLAGS is not used by default.  To used it, set optimize=1
# either in this file or on the command line.
#OCCFLAGS    += -qarch=ppc -qtune=604
OCCFLAGS    += -qlibansi -O2 -qarch=com 
RANLIB	    = ranlib
#TEMPINCDIR  = tempinc
SOBUILD = 
