#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS = -lTAO_AV -lTAO_CosProperty -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_Strategies -lTAO_IORTable -lTAO_PortableServer -lTAO

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

PSRC = $(PING_OBJS:.o=.cpp) $(PONG_OBJS:.o=.cpp) $(CONTROL_OBJS:.o=.cpp)

BIN2 = ping pong control

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # AV

PING_OBJS = ping.o
PONG_OBJS = pong.o
CONTROL_OBJS = control.o

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

ifeq ($(rapi),1)
  LDLIBS += -lACE_QoS 
endif # rapi

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs/AV

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
