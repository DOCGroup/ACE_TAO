#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the PACE tests
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/pace
endif # ! PACE_ROOT

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
include $(ACE_ROOT)/include/makeinclude/macros.GNU

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

#INCLDIRS += -I$(PACE_ROOT)
INCLDIRS = -I$(PACE_ROOT)

#LDFLAGS += -L$(PACE_ROOT)/pace -lPACE
#LDFLAGS = -L$(PACE_ROOT)/pace -lPACE
#LDFLAGS -= -L$(ACE_ROOT)/ace -lACE
#LDFLAGS =
#VLDLIBS -= -L$(ACE_ROOT)/ace -lACE
#VLDLIBS = -L$(PACE_ROOT)/pace -lPACE
VLDLIBS =

ACELIB=

ifndef SOEXT
  SOEXT=so
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

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
