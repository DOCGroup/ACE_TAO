# $Id$

SRC = $(BIN:%=%$(VAR).cpp)
LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

BIN2 = EC_Basic

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Event

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

#### Local rules and variables...

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
