#
# $Id$
# 
# =TITLE 
#  Makefile to create the LifeCycle_Service

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

TAO_IDLFLAGS=-I$(TAO_ROOT)/orbsvcs/orbsvcs


BIN = Life_Cycle_Service

# The following lines tell the souce code that the trading
# service is available.

BUILD = $(BIN)


LIFE_CYCLE_SERVICE_SRCS = \
	Life_Cycle_Service.cpp Life_Cycle_Service_Impl.cpp \
	Factory_Trader.cpp Criteria_Evaluator.cpp


LSRC= \
	Life_Cycle_Service.cpp Life_Cycle_Service_Impl.cpp \
	Factory_Trader.cpp Criteria_Evaluator.cpp

LIFE_CYCLE_SERVICE_OBJS = Life_Cycle_Service.o Life_Cycle_Service_Impl.o \
		Factory_Trader.o Criteria_Evaluator.o

LDLIBS = -lorbsvcs -lTAO

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs \
	-I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

realclean: clean

Life_Cycle_Service: $(addprefix $(VDIR),$(LIFE_CYCLE_SERVICE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

