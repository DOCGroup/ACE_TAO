# $Id$
#
# This is for the HP-UX 10.x and 11.x using the aC++ compiler.
#
# For 10.x, most of the difference for threaded vs. non-threaded is
# contained in ace/config-hpux-10.x.h.  However, to build without threads,
# you should run the make with "threads=0" since the default is to build
# with threads.
#
# HP-UX 11 supports both 32-bit and 64-bit builds, regardless of which
# architecture the build is done on.  This file is set up to build native
# to the machine it's running on.  To cross-build for another platform, add
# "buildbits=32" to make a 32-bit build, and "buildbits=64" to do a 64-bit
# build.  Note that if you explicitly specify a 64-bit build, the option
#  -Wl,+vnocompatwarnings is added to shut up the "At least one PA 2.0 object
# file detected..." messages.

buildbits ?= 0
debug ?= 1
distrib ?= 0
exceptions ?= 1
optimize ?= 0
threads ?= 1
versioned_so ?= 0

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPMODEL := $(shell uname -m)
HPUX_VERS       := $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC              = /bin/cc
CXX             = aCC

# Determine compiler version
CXXVERS_WORDS := $(subst ., , $(shell $(CXX) -V 2>&1))
CXXMINORVERS := $(word 8, $(CXXVERS_WORDS))
CXXMAJORVERS := $(word 7, $(CXXVERS_WORDS))

ifeq (,$(stdcpplib))
# If the user didn't explicitly set stdcpplib, assume that the compiler
# can do -AA, unless it's a .2x version (e.g., 03.24). 03.27 can do -AA,
# so check for that one explicitly and enable it if so.
  ifeq (,$(filter 2%,$(CXXMINORVERS)))
    stdcpplib = 1
  else
    ifeq (27,$(CXXMINORVERS))
      stdcpplib = 1
    else
      stdcpplib = 0
    endif
  endif
endif

ifeq (1,$(stdcpplib))
  CCFLAGS += -AA
  ifeq (03,$(CXXMAJORVERS))
    # Warning 930 is spurious when new(std::nothrow) is used. Reported to HP
    # as support call 3201224717. (Steve Huston, 23-Nov-2002)
    CCFLAGS += +W930
    # aCC 3.31 has poor member template support.
    ifeq (31,$(CXXMINORVERS))
      CCFLAGS += -DACE_LACKS_MEMBER_TEMPLATES
    endif
  endif
# Compile errors using -AA and +noeh together - HP problem report
# JAGad12608, fix slated for Dec 2001 Release -- Steve Huston, April 10, 2001.
# If you still have problems with this area, uncomment the following line:
#  exceptions = 1
endif

# Set the appropriate preprocessor defs for threading based on OS version
# and specified compiler options.
ifeq ($(threads),1)
  THR_DEFS = -DACE_HAS_THREADS

  # x.30 was the first version offering -mt. So, if it's a .2x version,
  # set the defs by hand; anything else, use -mt. This leaves out .1x
  # versions, which should be no problem at this point.
  ifeq (,$(filter 2%,$(CXXMINORVERS)))
    CC_THR_DEFS += -mt
  else
    # Different for v2 vs. v1 C++ library selection.
    ifeq (1,$(stdcpplib))
      THR_DEFS += -D_RWSTD_MULTI_THREAD
    else
      THR_DEFS += -DRWSTD_MULTI_THREAD -D_THREAD_SAFE
    endif

    # For HP-UX 11.x, choose kernel threads over CMA (user) threads.
    ifeq ($(word 2,$(HPVERS_WORDS)), 11)
      THR_DEFS += -D_POSIX_C_SOURCE=199506L
    endif
  endif
else
  THR_DEFS = -DACE_HAS_THREADS=0
  # On HP-UX 11, even without building for threads, the *_r reentrant
  # functions are available, and _REENTRANT enables them.
  ifeq ($(word 2,$(HPVERS_WORDS)), 11)
    THR_DEFS += -D_REENTRANT
  endif
endif

ifeq ($(HPMODEL), ia64)
  itanium = 1
endif

ifeq (03,$(CXXMAJORVERS))
  # Suppress Warning 302 ((...) parameter list is a non-portable feature)
  CCFLAGS                += +W302

  ifeq (57,$(CXXMINORVERS))
    # aCC 3.57 has a bug. When a class has private constructors
    # and is trying to be created from a friend class we end up with
    # a compile error. We disable array optimization in the compiler
    # by setting this environment variable. HP CR JAGaf43094
    export aCC_ARRAY_OPT=OFF
  endif
  ifeq (60,$(CXXMINORVERS))
    # Same bug as above
    export aCC_ARRAY_OPT=OFF
  endif
endif

ifeq (05,$(CXXMAJORVERS))
  # Warnings about binary incompatiblity
  CCFLAGS += +W1016,1031
endif

CPPFLAGS        += $(THR_DEFS) -D_HPUX_SOURCE -DHPUX_VERS=$(HPUX_VERS) -DACE_LACKS_PRAGMA_ONCE
CCFLAGS         += $(CC_THR_DEFS)

# If exception support is explicitly disabled, tell the compiler.
# This is not recommended since the run-time library can throw exceptions.
ifneq ($(exceptions),1)
  CCFLAGS  += +noeh
endif

# Additionally, on HP-UX 10.20, suppress 495 to shut up the warnings from
# the system header files.  667 is also suppressed, but the compiler still
# tells you there was a future error, but at least you can pick out any real
# errors by quickly scanning the output. 829 is suppressed because the
# system headers have offending string literals assigned to char *.
ifeq ($(word 2,$(HPVERS_WORDS)), 10)
CCFLAGS         += +W495,667,829,908
endif
DCCFLAGS        += -g
DLD             = $(CXX) $(CCFLAGS)
LD              = $(CXX)
OCCFLAGS        = -O
PIC             = +Z

PRELIB          = $(COMPILE.cc) $^
AR              = /usr/ccs/bin/ar
ARFLAGS         = ruv
RANLIB          = echo
LDFLAGS         = -Wl,+s -z
SOFLAGS         += -b

ifeq ($(distrib),1)

ifeq ($(word 2,$(HPVERS_WORDS)), 10)
SOFLAGS         += -Wl,-x -Wl,+h/usr/lib/$(@F)
else
ifeq ($(buildbits),32)
SOFLAGS         += -Wl,-x -Wl,+h$(@F) -Wl,+b/usr/lib
endif
ifeq ($(buildbits),64)
SOFLAGS         += -Wl,-x -Wl,+h$(@F) -Wl,+b/usr/lib/pa20_64
endif
endif

endif

ifeq ($(itanium),1)
 SOEXT		= so
else
 SOEXT           = sl
endif

ifeq ($(buildbits),32)
  ifeq ($(itanium),1)
    CFLAGS += +DD32
    CCFLAGS += +DD32
  else
    CFLAGS += +DA1.1 +DS1.1
    CCFLAGS += +DA1.1 +DS1.1
  endif
else
ifeq ($(buildbits),64)
  ifeq ($(itanium),1)
    CFLAGS += +DD64
    CCFLAGS += +DD64
  else
    ifeq ($(word 2,$(HPVERS_WORDS)), 11)
      ## Without the W, it isn't really a 64-bit build
      CFLAGS += +DA2.0W +DS2.0W
      CCFLAGS += +DA2.0W +DS2.0W
    else
      CFLAGS += +DA2.0 +DS2.0
      CCFLAGS += +DA2.0 +DS2.0
    endif
  endif
LDFLAGS         += -Wl,+vnocompatwarnings
else
  ifneq ($(itanium),1)
    # HP aCC on Itanium doesn't support this option, defaults to 32bit then.
    CFLAGS += +DAportable
    CCFLAGS += +DAportable
  endif
endif
endif

#
# Grab the appropriate libraries based on the version of HP-UX we're building
# on and whether or not thread support is being used.
#
ifeq ($(word 2,$(HPVERS_WORDS)), 11)

  #11.x:
  ifeq ($(threads),1)
    # If -mt is in CCFLAGS, we don't need to add anything to LIBS; -mt does it.
    ifeq ($(findstring -mt,$(CCFLAGS)),)
      ## -L./ must directly preceed the system libraries for 64-bit builds
      ## with aCC 3.27.  Without this change, libpthread.sl can not be found.
      LIBS += -L./ -lpthread
    endif
  endif
  LIBS          += -lxti -lrt -ldld

else

  # 10.x:
  LIBS          += -lxti -ldld
  ifeq ($(threads),1)
    LIBS                += -lcma
  endif

endif
