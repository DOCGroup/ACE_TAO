# $Id$
#
#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC	= ReceiverC.cpp ReceiverS.cpp BroadcasterC.cpp BroadcasterS.cpp

PROG_SRCS = \
	Receiver_i.cpp \
	Broadcaster_i.cpp \
	Client_i.cpp \
	Server_i.cpp \
	client.cpp \
	server.cpp


SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		ReceiverS.o \
		ReceiverC.o \
		BroadcasterC.o \
		BroadcasterS.o \
		Receiver_i.o \
		Client_i.o \
		client.o

SIMPLE_SVR_OBJS = \
                ReceiverS.o \
		ReceiverC.o \
		BroadcasterC.o \
		BroadcasterS.o \
		Broadcaster_i.o \
		Server_i.o \
		server.o

BIN = server \
      client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

LDFLAGS +=  -L$(TAO_ROOT)/tao
CPPFLAGS +=

$(foreach ext, $(IDL_EXT), Broadcaster$(ext)): Receiver.idl

.PRECIOUS: $(foreach file, Broadcaster Receiver, $(foreach ext, $(IDL_EXT), $(file)$(ext))))

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) ReceiverC.* ReceiverS.* ReceiverS_T.* BroadcasterC.* BroadcasterS.* BroadcasterS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
