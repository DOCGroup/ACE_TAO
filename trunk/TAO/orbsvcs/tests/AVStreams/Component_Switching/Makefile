#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for Pluggable testing of AVStreams of TAO.
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     = -lTAO_AV -lTAO_CosProperty -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO_Strategies -lTAO


SENDER_OBJS = Connection_Manager.o \
	      sender.o
DISTRIBUTER_OBJS = Connection_Manager.o \
		distributer.o
RECEIVER_OBJS = Connection_Manager.o \
		receiver.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

FILES =  Connection_Manager sender distributer receiver
DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

BIN2 = sender distributer receiver

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # AV

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
  # This example does not compile with g++ 2.7.2.3
all clean realclean:
	@echo This cannot built with older versions of g++
else # ! ACE_HAS_GNUG_PRE_2_8
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
endif # ! ACE_HAS_GNUG_PRE_2_8

#DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs/AV \
            -I$(TAO_ROOT)

ifeq ($(rapi),1)
  LDLIBS += -lACE_QoS
endif # rapi

sender:$(addprefix $(VDIR),$(SENDER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

distributer:$(addprefix $(VDIR),$(DISTRIBUTER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

receiver:$(addprefix $(VDIR),$(RECEIVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
