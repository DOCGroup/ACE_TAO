#----------------------------------------------------------------------------
#
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = MT_Client_TestC.cpp MT_Client_TestS.cpp
PROG_SRCS = $(IDL_SRC) server.cpp client.cpp \
	local_server.cpp MT_Object_Impl.cpp

LSRC = $(PROG_SRCS)

CLIENT_OBJS = MT_Client_TestS.o MT_Client_TestC.o \
	MT_Object_Impl.o local_server.o client.o

SERVER_OBJS = MT_Client_TestS.o MT_Client_TestC.o server.o \
	MT_Object_Impl.o

BIN = client server
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

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao 
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs 

.PRECIOUS: MT_Client_TestC.h MT_Client_TestC.i MT_Client_TestC.cpp
.PRECIOUS: MT_Client_TestS.h MT_Client_TestS.i MT_Client_TestS.cpp

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf Triangle_TestS.* Triangle_TestC.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
