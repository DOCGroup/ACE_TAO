#
# $Id$
#

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = Logging_Service.cpp

LOG_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lTAO_DsLogAdmin -lTAO_CosTrading -lTAO_CosNaming -lTAO_IORTable -lTAO_Svc_Utils -lTAO_DynamicAny -lTAO_PortableServer -lTAO

BIN2 = Logging_Service

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Log,$(findstring Log,$(TAO_ORBSVCS)))
  ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # Naming
endif # Log

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

Logging_Service: $(addprefix $(VDIR),$(LOG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
