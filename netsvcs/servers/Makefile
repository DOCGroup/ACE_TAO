#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

LDLIBS  = -lnetsvcs
VLDLIBS = $(LDLIBS:%=%$(VAR))
LSRC = $(addsuffix .cpp,$(BIN))

BIN2 = main

#### If the ACE library wasn't built with sufficient components, don't
#### try to build here.
ACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
  ifeq (Token,$(findstring Token,$(ACE_BUILD_COMPONENTS)))
    BIN = $(BIN2)
  endif # ! Token
endif # ! Other

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -I$(ACE_ROOT)/netsvcs/lib

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
