# $Id$

BIN = EC_Multiple

BUILD = $(BIN)

EC_MULTIPLE_SRCS= \
	EC_Multiple.cpp

LSRC= \
	$(EC_MULTIPLE_SRCS) \

EC_MULTIPLE_OBJS = $(EC_MULTIPLE_SRCS:.cpp=.o)

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
DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

# Leave the scheduler output out if this is a config run.
ifeq ($(runtime),1)
EC_MULTIPLE_CONFIG_OBJS=EC_Multiple_Scheduler_Runtime.o
endif # runtime

ifeq ($(probe),1)
    CCFLAGS += -DACE_ENABLE_TIMEPROBES
endif # probe

EC_Multiple: $(addprefix $(VDIR),$(EC_MULTIPLE_OBJS) $(EC_MULTIPLE_CONFIG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
