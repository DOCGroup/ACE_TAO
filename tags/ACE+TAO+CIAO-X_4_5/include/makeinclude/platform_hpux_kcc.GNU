# $Id$
#
# This is for HP-UX 11.x using the KCC compiler.
#
# HP-UX 11 supports both 32-bit and 64-bit builds, regardless of which
# architecture the build is done on.  This file is set up to build native
# to the machine it's running on.  To cross-build for another platform, add
# "buildbits=32" to make a 32-bit build, and "buildbits=64" to do a 64-bit
# build.  Note that if you explicitly specify a 64-bit build, the option
#  -Wl,+vnocompatwarnings is added to shut up the "At least one PA 2.0 object
# file detected..." messages.
#
# NOTE: This has only been tested with "buildbits" left as the default.
#

ifeq (,$(buildbits))
  buildbits = 0
endif
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

# The only way we can disable exceptions is to also disable threads
ifeq ($(threads),1)
  THR_DEFS   = --thread_safe -DACE_HAS_THREADS -D_POSIX_C_SOURCE=199506L
  LDFLAGS    = --thread_safe
  ARFLAGS    = --thread_safe
  exceptions = 1
else
  THR_DEFS   = -DACE_HAS_THREADS=0
  LDFLAGS    =
  ARFLAGS    =
  ifeq (0,$(exceptions))
    CCFLAGS += --no_exceptions
    LDFLAGS += --no_exceptions
  endif
endif


CC           = cc
CXX          = KCC
CCFLAGS     += $(CFLAGS) $(THR_DEFS) -DACE_LACKS_PRAGMA_ONCE --one_instantiation_per_object
DCFLAGS     += +K0 -g
DLD          = $(CXX) $(CCFLAGS)
LD           = $(CXX)
OCCFLAGS     = +K3 -O
PIC          = +Z
LDFLAGS     += -Wl,+s -z
SOFLAGS      =
SOEXT        = sl

# Please note that KCC can not replace object files within a
# static library.  You must rebuild the library with all of the .o's
# The ar rule will require you to touch all of the object files in .obj,
# including the ones under .obj/ti_files, to do this.
AR           = KCC
ARFLAGS     += -Bstatic -o

# KCC allows suppresion of warnings and errors.
# Warning #111 statement is unreachable
CCFLAGS     += --diag_suppress 111

# KCC always uses the standard cpp library
CCFLAGS     += -DACE_HAS_STANDARD_CPP_LIBRARY=1

#
# libpthread is automatically linked in by KCC if we use --thread_safe,
# therefore we do not need to explicitly supply it.
# If we link statically and libpthread is supplied with $(LIBS), the
# program will core dump.
#
LIBS        += -Bdynamic -lxti -lrt

ifdef static_libs_only
  ifneq ($(static_libs_only),0)
    static_libs=1
  endif
endif
# Kind of a hack since LDFLAGS is used for both non-static
# library generation and binary generation.  LD is only used for
# binary generation.
ifneq ($(static_libs),0)
  CCFLAGS   += --one_instantiation_per_object
  LD        += -Bstatic
  # libc is also automatically linked in by KCC, but when we are linking
  # statically libc must be linked dynamically or gethostbyname does not
  # function properly.
  LIBS      += -lc -Bstatic
endif

#
# This section has not been tested.
#
ifeq ($(distrib),1)
  ifeq ($(buildbits),32)
    SOFLAGS += -Wl,-x -Wl,+h/usr/lib/$(@F)
  endif
  ifeq ($(buildbits),64)
    SOFLAGS += -Wl,-x -Wl,+h/usr/lib/pa20_64/$(@F)
  endif
endif
ifeq ($(buildbits),32)
  CCFLAGS   += +DA1.1 +DS1.1
else
  ifeq ($(buildbits),64)
    CCFLAGS += +DA2.0 +DS2.0
    LDFLAGS += -Wl,+vnocompatwarnings
  else
    CCFLAGS += +DAportable
  endif
endif

SONAME		=
SOVERSION	=
