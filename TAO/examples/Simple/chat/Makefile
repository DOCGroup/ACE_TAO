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
	Chat.cpp \
	Server.cpp


SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		ReceiverS.o \
		ReceiverC.o \
		BroadcasterC.o \
		BroadcasterS.o \
		Receiver_i.o \
		Chat_i.o \
		Chat.o

SIMPLE_SVR_OBJS = \
                ReceiverS.o \
		ReceiverC.o \
		BroadcasterC.o \
		BroadcasterS.o \
		Broadcaster_i.o \
		Server_i.o \
		Server.o

BIN = Server \
      Chat
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

.PRECIOUS: ReceiverC.cpp ReceiverC.i ReceiverC.h
.PRECIOUS: ReceiverS.cpp  ReceiverS.i ReceiverS.h
.PRECIOUS: ReceiverS_T.cpp ReceiverS_T.i ReceiverS_T.h
.PRECIOUS: BroadcasterC.cpp BroadcasterC.i BroadcasterC.h
.PRECIOUS: BroadcasterS.cpp  BroadcasterS.i BroadcasterS.h
.PRECIOUS: BroadcasterS_T.cpp BroadcasterS_T.i BroadcasterS_T.h

Server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Chat:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf ReceiverC.* ReceiverS.* ReceiverS_T.* BroadcasterC.* BroadcasterS.* BroadcasterS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
