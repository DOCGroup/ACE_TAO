# $Id$

BIN = Event_Latency

BUILD = $(BIN)

EVENT_LATENCY_SRCS= \
	Event_Latency.cpp

LSRC= \
	$(EVENT_LATENCY_SRCS) \

EVENT_LATENCY_OBJS = $(EVENT_LATENCY_SRCS:.cpp=.o)

LDLIBS= -lorbsvcs -lTAO

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifdef quantify
  CCFLAGS += -Dquantify
  CPPFLAGS += -I/pkg/purify/quantify-2.1-solaris2
endif # quantify

#### Local rules and variables...

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

# Leave the scheduler output out if this is a config run.
ifeq ($(runtime),1)
EVENT_LATENCY_CONFIG_OBJS=Event_Latency_Scheduler_Runtime.o
endif # runtime

Event_Latency: $(addprefix $(VDIR),$(EVENT_LATENCY_OBJS) $(EVENT_LATENCY_CONFIG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
