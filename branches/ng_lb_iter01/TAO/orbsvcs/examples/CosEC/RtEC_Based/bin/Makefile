#
# $Id$
#

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = RtEC_Based_CosEC.cpp

ES_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lTAO_RTEC_COSEC -lTAO_CosEvent -lTAO_RTEvent -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO

BIN2 = RtEC_Based_CosEC

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  ifeq (RTEvent,$(findstring RTEvent,$(TAO_ORBSVCS)))
    ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
      BIN = $(BIN2)
    endif # ACE_HAS_GNUG_PRE_2_8
  endif # RTEvent
endif # CosEvent

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

LDFLAGS += -L../lib -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I../lib -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
