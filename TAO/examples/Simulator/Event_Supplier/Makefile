#
# $Id$
#

BIN = Event_Sup Event_Con

BUILD = $(BIN)


IDL_SRC	= NavWeapC.cpp

EVENT_TEST_SRCS= \
	Event_Sup.cpp Event_Con.cpp $(IDL_SRC)

LSRC= \
	$(EVENT_TEST_SRCS) \

EVENT_SUP_OBJS = Event_Sup.o NavWeapC.o
EVENT_CON_OBJS = Event_Con.o NavWeapC.o

LDLIBS= -lorbsvcs -lTAO

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H


$(IDL_SRC): NavWeap.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl NavWeap.idl

Event_Sup: $(addprefix $(VDIR),$(EVENT_SUP_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Event_Con: $(addprefix $(VDIR),$(EVENT_CON_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
