#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the PACE tests
#----------------------------------------------------------------------------

ifndef static_libs_only
  ifndef shared_libs_only
    static_libs_only = 1
  endif # shared_libs_only
endif # static_libs_only

# If we are inlining the PACE functions then we don't want to include
# the PACE library (and we need to define PACE_HAS_INLINE). If we're not
# inlining then we need to include the PACE library. Inlining is the default.
ifndef inline
  CFLAGS += -DPACE_HAS_INLINE
else
  ifneq (0,$(inline))
    CFLAGS += -DPACE_HAS_INLINE
  endif # inline
endif # ! inline

LIBS += -L$(PACE_ROOT)/pace -lPACE

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   Stdio_Test \
        Posix_SP_Test

#### If the PACE library wasn't built with all components, don't
#### try to build certain tests.
PACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/pace_components --pace)

PSRC=$(addsuffix .c,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
ACELIB =
INCLDIRS += -I$(PACE_ROOT)
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifndef SOEXT
  SOEXT=so
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

include $(ACE_ROOT)/PACE/include/makeinclude/rules.common.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
