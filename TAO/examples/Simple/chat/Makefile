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

.PRECIOUS: ReceiverC.cpp ReceiverC.i ReceiverC.h
.PRECIOUS: ReceiverS.cpp  ReceiverS.i ReceiverS.h
.PRECIOUS: ReceiverS_T.cpp ReceiverS_T.i ReceiverS_T.h
.PRECIOUS: BroadcasterC.cpp BroadcasterC.i BroadcasterC.h
.PRECIOUS: BroadcasterS.cpp  BroadcasterS.i BroadcasterS.h
.PRECIOUS: BroadcasterS_T.cpp BroadcasterS_T.i BroadcasterS_T.h

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf ReceiverC.* ReceiverS.* ReceiverS_T.* BroadcasterC.* BroadcasterS.* BroadcasterS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ReceiverC.o .obj/ReceiverC.so .shobj/ReceiverC.o .shobj/ReceiverC.so: ReceiverC.cpp ReceiverC.h ReceiverC.i ReceiverS.h \
 ReceiverS_T.h ReceiverS_T.i ReceiverS_T.cpp ReceiverS.i
.obj/ReceiverS.o .obj/ReceiverS.so .shobj/ReceiverS.o .shobj/ReceiverS.so: ReceiverS.cpp ReceiverS.h ReceiverC.h ReceiverC.i \
 ReceiverS_T.h ReceiverS_T.i ReceiverS_T.cpp ReceiverS.i
.obj/BroadcasterC.o .obj/BroadcasterC.so .shobj/BroadcasterC.o .shobj/BroadcasterC.so: BroadcasterC.cpp BroadcasterC.h ReceiverC.h \
 ReceiverC.i BroadcasterC.i BroadcasterS.h ReceiverS.h ReceiverS_T.h \
 ReceiverS_T.i ReceiverS_T.cpp ReceiverS.i BroadcasterS_T.h \
 BroadcasterS_T.i BroadcasterS_T.cpp BroadcasterS.i
.obj/BroadcasterS.o .obj/BroadcasterS.so .shobj/BroadcasterS.o .shobj/BroadcasterS.so: BroadcasterS.cpp BroadcasterS.h ReceiverS.h \
 ReceiverC.h ReceiverC.i ReceiverS_T.h ReceiverS_T.i ReceiverS_T.cpp \
 ReceiverS.i BroadcasterC.h BroadcasterC.i BroadcasterS_T.h \
 BroadcasterS_T.i BroadcasterS_T.cpp BroadcasterS.i
.obj/Receiver_i.o .obj/Receiver_i.so .shobj/Receiver_i.o .shobj/Receiver_i.so: Receiver_i.cpp Receiver_i.h ReceiverS.h ReceiverC.h \
 ReceiverC.i ReceiverS_T.h ReceiverS_T.i ReceiverS_T.cpp ReceiverS.i
.obj/Broadcaster_i.o .obj/Broadcaster_i.so .shobj/Broadcaster_i.o .shobj/Broadcaster_i.so: Broadcaster_i.cpp Broadcaster_i.h BroadcasterS.h \
 ReceiverS.h ReceiverC.h ReceiverC.i ReceiverS_T.h ReceiverS_T.i \
 ReceiverS_T.cpp ReceiverS.i BroadcasterC.h BroadcasterC.i \
 BroadcasterS_T.h BroadcasterS_T.i BroadcasterS_T.cpp BroadcasterS.i \
 $(TAO_ROOT)/tao/TAO.h
.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp Client_i.h Receiver_i.h ReceiverS.h ReceiverC.h \
 ReceiverC.i ReceiverS_T.h ReceiverS_T.i ReceiverS_T.cpp ReceiverS.i \
 BroadcasterC.h BroadcasterC.i \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(TAO_ROOT)/tao/TAO.h
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp Server_i.h Broadcaster_i.h BroadcasterS.h \
 ReceiverS.h ReceiverC.h ReceiverC.i ReceiverS_T.h ReceiverS_T.i \
 ReceiverS_T.cpp ReceiverS.i BroadcasterC.h BroadcasterC.i \
 BroadcasterS_T.h BroadcasterS_T.i BroadcasterS_T.cpp BroadcasterS.i \
 $(TAO_ROOT)/tao/TAO.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
