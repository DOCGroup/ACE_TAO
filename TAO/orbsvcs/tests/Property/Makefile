#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the CosPropertyService Demo.
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

LDLIBS     = -lorbsvcs -lTAO

CosProperty_SERVER_OBJS = server.o
CosProperty_CLIENT_OBJS = client.o
SRC=server.cpp client.cpp

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = server client

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Property,$(findstring Property,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Property

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

server:$(addprefix $(VDIR),$(CosProperty_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:$(addprefix $(VDIR),$(CosProperty_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
