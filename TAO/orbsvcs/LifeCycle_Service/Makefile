#
# $Id$
# 
# =TITLE 
#  Makefile to create the LifeCycle_Service


# needed for the trading service stuff

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

TAO_IDLFLAGS=-I$(TAO_ROOT)/orbsvcs/orbsvcs

ifdef trader

ifndef STL_ROOT
  STL_ROOT=/project/doc/irfan/ObjectSpace/SC4.0
endif # STL_ROOT

STD=$(STL_ROOT)/ospace/std
STL=$(STL_ROOT)/stl

include $(STL_ROOT)/config/local.cfg

CC_INCLUDES += -I$(STD)
CC_INCLUDES += -pta

CPPFLAGS += \
        -I$(TOOLKIT) \
        $(CC_INCLUDES) \
        $(CC_FLAGS) \
        $(CC_DEFINES) \
        $(CC_EH) \
        $(CC_MT)


ifndef exceptions
CPPFLAGS += -DOS_NO_EXCEPTION_SPECIFIER
else
CPPFLAGS += -DOS_STL_ASSERT
endif

LDFLAGS += -L$(STL_ROOT)/lib

endif # trader
# end of the trading service stuff

BIN = Life_Cycle_Service

# The following lines tell the souce code that the trading
# service is available.

ifdef trader

CPPFLAGS += -DTRADER_AVAILABLE

endif # trader

BUILD = $(BIN)


LIFE_CYCLE_SERVICE_SRCS = \
	Life_Cycle_Service.cpp Life_Cycle_Service_Impl.cpp \
	Factory_Trader.cpp Criteria_Evaluator.cpp


LSRC= \
	Life_Cycle_Service.cpp Life_Cycle_Service_Impl.cpp \
	Factory_Trader.cpp Criteria_Evaluator.cpp

LIFE_CYCLE_SERVICE_OBJS = Life_Cycle_Service.o Life_Cycle_Service_Impl.o \
		Factory_Trader.o Criteria_Evaluator.o

ifdef trader

LDLIBS = -lorbsvcs -lospace -lTAO

else

LDLIBS = -lorbsvcs -lTAO

endif

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



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
