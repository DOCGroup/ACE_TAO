#
# $Id$
#

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

SRC = Notify_Server.cpp Notify_Service.cpp

LDLIBS = -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils \
         -lTAO_DynamicAny -lTAO_IORTable -lTAO_PortableServer -lTAO

BIN2 = Notify_Service

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Notify,$(findstring Notify,$(TAO_ORBSVCS)))
  ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # Naming
endif # Notify

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs/ETCL -L$(TAO_ROOT)/orbsvcs/orbsvcs \
           -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

NOTIFY_OBJS = Notify_Service.o Notify_Server.o

Notify_Service: $(addprefix $(VDIR),$(NOTIFY_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
