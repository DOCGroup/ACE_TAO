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

LDLIBS = -lTAO_CosLifeCycle -lTAO_CosTrading -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO

BIN2 = LifeCycle_Service

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (LifeCycle,$(findstring LifeCycle,$(TAO_ORBSVCS)))
  ifeq ($(minimum_corba),0)
    BIN = $(BIN2)
  endif # minimum_corba
endif # LifeCycle

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs \
        -I$(TAO_ROOT)

LifeCycle_Service: $(addprefix $(VDIR),$(LIFECYCLE_SERVICE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
