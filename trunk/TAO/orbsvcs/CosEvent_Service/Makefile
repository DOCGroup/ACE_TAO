#
# $Id$
#

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = CosEvent_Service.cpp \

ES_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lTAO_CosEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO

BIN2 = CosEvent_Service

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  ifeq (Event2,$(findstring Event2,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # Event2
endif # CosEvent

ifeq (NativeCosEvent,$(findstring NativeCosEvent,$(TAO_ORBSVCS)))
  BIN += CosEvent_Service_Native
endif # CosEventNative

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
