# $Id$

SRC = $(BIN:%=%$(VAR).cpp)
LDLIBS= -lTAO_RTEvent -lTAO_Svc_Utils -lTAO_PortableServer -lTAO

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

BIN2 = EC_Mcast

PSRC=EC_Mcast.cpp

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (RTEvent,$(findstring RTEvent,$(TAO_ORBSVCS)))
  ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
    BIN = $(BIN2)
  endif # ACE_HAS_GNUG_PRE_2_8
endif # RTEvent

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
