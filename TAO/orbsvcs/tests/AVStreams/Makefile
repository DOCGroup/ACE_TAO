#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the AVStreams demo of the TAO ORB
#
#----------------------------------------------------------------------------

#---------------
#   STL (start)
#---------------

include $(STL_ROOT)/config/local.cfg

STD=$(STL_ROOT)/ospace/std
STL=$(STL_ROOT)/stl

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

#---------------
#   STL (end)
#---------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     = -lorbsvcs -lospace -lTAO -lrpcsvc

AVStreams_SERVER_OBJS = server.o Machine_Properties.o Property_Exporter.o
AVStreams_CLIENT_OBJS = client.o

COSPROPERTY_OBJS = main.o

BIN = server client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

server:$(addprefix $(VDIR),$(AVStreams_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:$(addprefix $(VDIR),$(AVStreams_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf 


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY






