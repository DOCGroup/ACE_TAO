#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the Demux_Test test example
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

#override TAO_IDLFLAGS += \
#               -H linear_search \

LDLIBS   = -lTAO -lorbsvcs
LDFLAGS += -L$(TAO_ROOT)/tao

IDL_HDR = demux_testC.h demux_testS.h
IDL_SRC = demux_testC.cpp demux_testS.cpp

SVR_FILES =     demux_testC     \
                demux_testS     \
                demux_test_server \
                demux_test_i    \
                server

CLI_FILES =     demux_testC     \
                demux_testS     \
                demux_test_client \
                client

STAT_FILE =     demux_stats

DEMUX_TEST_SVR_OBJS = $(addsuffix .o,$(SVR_FILES))
DEMUX_TEST_CLT_OBJS = $(addsuffix .o,$(CLI_FILES))
DEMUX_TEST_STAT_OBJS = $(addsuffix .o,$(STAT_FILE))

SRC     = $(addsuffix .cpp,$(SVR_FILES)) \
          $(addsuffix .cpp,$(CLI_FILES)) \
          $(addsuffix .cpp,$(STAT_FILE))

BIN2 = server client
VLDLIBS = $(LDLIBS:%=%$(VAR))

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Naming

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

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)

.PRECIOUS: $(IDL_HDR) $(IDL_SRC)

server: $(addprefix $(VDIR),$(DEMUX_TEST_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(DEMUX_TEST_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

demux_stat: $(addprefix $(VDIR),$(DEMUX_TEST_STAT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) demux_testC.* demux_testS.* demux_testS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
