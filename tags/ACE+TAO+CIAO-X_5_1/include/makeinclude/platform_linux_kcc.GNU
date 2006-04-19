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

ifeq (,$(debug))
  debug=0
endif
ifeq (,$(optimize))
  optimize=1
endif
ifeq (,$(threads))
  threads = 1
endif

CC      = KCC
CXX     = $(CC)
CPPFLAGS  += -D_REENTRANT -D_GNU_SOURCE

CCFLAGS += $(CFLAGS) -DNULL=0 --one_instantiation_per_object
DCFLAGS += +K0 -g
OCFLAGS += +K3 -O
DLD     = $(CC)
LD      = $(CC)
# Using just -ldl results in a very annoying warning (nm: no symbols),
# because the prelinker searches for missing templates in the libdl
# library.  Passing the option directly to the linker fixes the
# warning.
LIBS    += -Wl,-ldl
MATHLIB = -lm

ifeq (,$(exceptions))
  exceptions=0
endif # exceptions

ifeq ($(exceptions),0)
  CCFLAGS += --no_exceptions
endif # ! exceptions

ifeq ($(threads),1)
  CFLAGS += --thread_safe
  LDFLAGS += --thread_safe
endif # threads

# KCC allows suppresion of warnings and errors.
# Warning #280 declaration of a member with the same name as its class
CCFLAGS += --diag_suppress 280

ifdef static_libs_only
  ifneq ($(static_libs_only),0)
    static_libs=1
  endif
endif
ifneq ($(static_libs),0)
  CCFLAGS   += --one_instantiation_per_object
endif

PIC     = -fPIC
AR      = $(CC)
ARFLAGS = $(CCFLAGS) $(CPPFLAGS) -Bstatic -o
RANLIB  = @true
SOFLAGS += $(PIC) $(CCFLAGS) $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(SOFLAGS) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1
