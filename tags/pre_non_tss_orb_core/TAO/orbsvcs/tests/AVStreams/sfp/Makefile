#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the SFP test of the TAO ORB
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

LDLIBS    =  -lorbsvcs -lTAO -lACE

PROG_SRCS =  client.cpp server.cpp

LSRC = $(PROG_SRCS)

AVStreams_SERVER_OBJS = server.o
AVStreams_CLIENT_OBJS = client.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  BIN = client server
endif # AV

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

server:$(addprefix $(VDIR),$(AVStreams_SERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@@ $^ $(VLDLIBS)

client:$(addprefix $(VDIR),$(AVStreams_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@@ $^ $(VLDLIBS)

#main: $(addprefix $(VDIR), $(COSPROPERTY_OBJS))
#	$(LINK.cc) $(LDFAGS) -o $@@ $^ $(VLDLIBS)

# realclean: clean
#	-$(RM)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
