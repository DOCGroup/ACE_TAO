# $Id$
#
# needed for the trading service stuff

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

override TAO_IDLFLAGS += -I$(TAO_ROOT)/orbsvcs/orbsvcs

# The following lines tell the souce code that the trading
# service is available.

ifdef trader
  CPPFLAGS += -DTRADER_AVAILABLE
endif # trader

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = \
        QuoterC.cpp QuoterS.cpp

CLIENT_SRCS = \
        client.cpp

SERVER_SRCS = \
        server.cpp Quoter_i.cpp

FACTORY_FINDER_SRCS = \
        Factory_Finder.cpp Factory_Finder_i.cpp

GENERIC_FACTORY_SRCS = \
        Generic_Factory.cpp Generic_Factory_i.cpp


LSRC= \
        $(IDL_SRC) $(CLIENT_SRCS) $(SERVER_SRCS) \
        $(FACTORY_FINDER_SRCS) $(GENERIC_FACTORY_SRCS)

COMMON_OBJS = QuoterC.o QuoterS.o
CLIENT_OBJS = $(COMMON_OBJS) client.o
SERVER_OBJS = $(COMMON_OBJS) server.o Quoter_i.o
FACTORY_FINDER_OBJS = $(COMMON_OBJS) Factory_Finder.o Factory_Finder_i.o
GENERIC_FACTORY_OBJS = $(COMMON_OBJS) Generic_Factory.o Generic_Factory_i.o

BIN2 = client server Factory_Finder Generic_Factory

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  ifeq (LifeCycle,$(findstring LifeCycle,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # LifeCycle
endif # Naming

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)

  # Since older g++ cannot handle nested classes properly, this test
  # will not build on g++ prior to 2.8.0.
  all clean realclean:
	@echo examples/Quoter is not built with older versions of g++

else # ! ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs \
        -I$(TAO_ROOT) $(TSS_ORB_FLAG)
TAO_IDLFLAGS +=  -I$(TAO_ROOT)/orbsvcs/orbsvcs

realclean: clean
	-$(RM) *C.* *S.* *S_T.*

.PRECIOUS: $(IDL_SRC)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Factory_Finder: $(addprefix $(VDIR),$(FACTORY_FINDER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Generic_Factory: $(addprefix $(VDIR),$(GENERIC_FACTORY_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

endif # ! ACE_HAS_GNUG_PRE_2_8

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
