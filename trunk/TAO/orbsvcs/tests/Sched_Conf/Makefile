# $Id$

DUMP_SRCS = Sched_Conf.cpp
LSRC = $(DUMP_SRCS)

SCHEDULE_OBJS = $(SCHEDULE_SRCS:.cpp=.o)
DUMP_OBJS = $(DUMP_SRCS:.cpp=.o)

LDLIBS  = -lorbsvcs -lTAO
VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = Sched_Conf

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Sched,$(findstring Sched,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Sched

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
