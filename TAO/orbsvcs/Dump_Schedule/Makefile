#
# $Id$
#

BIN = Dump_Schedule
BUILD = $(BIN)

DUMP_SRCS = \
	Dump_Schedule.cpp

LSRC = 	$(DUMP_SRCS)

SCHEDULE_OBJS=$(SCHEDULE_SRCS:.cpp=.o)
DUMP_OBJS=$(DUMP_SRCS:.cpp=.o)

LDLIBS     =  -lorbsvcs -lTAO
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

Dump_Schedule: $(addprefix $(VDIR),$(DUMP_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
