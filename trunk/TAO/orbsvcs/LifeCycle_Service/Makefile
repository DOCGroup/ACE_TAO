#
# $Id$
#
# =TITLE
#  Makefile to create the LifeCycle_Service

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

TAO_IDLFLAGS=-I$(TAO_ROOT)/orbsvcs/orbsvcs

# The following lines tell the souce code that the trading
# service is available.

LIFECYCLE_SERVICE_SRCS = \
        LifeCycle_Service.cpp LifeCycle_Service_i.cpp \
        Factory_Trader.cpp Criteria_Evaluator.cpp

LSRC= \
        LifeCycle_Service.cpp LifeCycle_Service_i.cpp \
        Factory_Trader.cpp Criteria_Evaluator.cpp

LIFECYCLE_SERVICE_OBJS = LifeCycle_Service.o LifeCycle_Service_i.o \
        Factory_Trader.o Criteria_Evaluator.o

LDLIBS = -lorbsvcs -lTAO

BIN2 = LifeCycle_Service

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (LifeCycle,$(findstring LifeCycle,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # LifeCycle

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
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

LifeCycle_Service: $(addprefix $(VDIR),$(LIFECYCLE_SERVICE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
