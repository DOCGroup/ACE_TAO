#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for Pluggable testing of AVStreams of TAO.
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     = -lTAO_AV -lTAO_CosProperty -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO

RECEIVER_OBJS = receiver.o
SENDER_OBJS = sender.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

FILES = sender receiver
DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

BIN2 = receiver sender

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
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifeq ($(rapi),1)
  LDLIBS += -lACE_QoS
endif # rapi

#DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs/AV \
            -I$(TAO_ROOT)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
