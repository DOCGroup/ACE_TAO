# $Id$

# According to Ben Eng <ben@jetpen.com>, this file
# should allow ACE to be built on Linux using KAI C++ 3.3a.
#
# NOTE:  evaluation copies of KCC might include libraries that
# were compiled with exception handling enabled.  To enable link
# compatibility with those libraries, you must build ACE with
# exception handling enabled, as well.  To do that, either set
# the exceptions macro below to 1, or build with:
# make exceptions=1

debug = 0
exceptions =
optimize = 1

CC      = KCC
CXX     = $(CC)
CFLAGS  += -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT \
           -D_GNU_SOURCE

CCFLAGS += $(CFLAGS) -DNULL=0
DCFLAGS += +K0 -g
DLD     = $(CC)
LD      = $(CC)
LIBS    += -Bdynamic -ldl -lpthread -lm

ifndef exceptions
  EXCEPTION = --no_exceptions
endif # ! exceptions

# KCC allows suppresion of warnings and errors.
# Warning #280 declaration of a member with the same name as its class
CCFLAGS += --diag_suppress 280

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
  # If we must link with static libraries, libc must be linked in dynamically
  # or else the executable will core dump.  This is only necessary because the
  # -Bstatic causes the hidden libraries (KCC, gcc and c) to get statically
  # linked.  You might get a big warning at link time.  It's nothing with
  # which to be concerned.
  LIBS      += -lc -Bstatic
endif

OCFLAGS += +K3 -O --one_instantiation_per_object $(EXCEPTION)
PIC     = -fPIC
AR      = $(CC)
ARFLAGS = $(EXCEPTION) -Bstatic -o
RANLIB  = @true
SOFLAGS = $(EXCEPTION)
SOBUILD = $(COMPILE.cc) $(PIC) $(EXCEPTION) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1

