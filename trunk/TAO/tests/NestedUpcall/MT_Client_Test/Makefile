#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC = MT_Client_TestC.cpp MT_Client_TestS.cpp
PROG_SRCS = $(IDL_SRC) server.cpp client.cpp \
        local_server.cpp MT_Object_i.cpp

SRC = $(PROG_SRCS)

CLIENT_OBJS = MT_Client_TestS.o MT_Client_TestC.o \
        MT_Object_i.o local_server.o client.o

SERVER_OBJS = MT_Client_TestS.o MT_Client_TestC.o server.o \
        MT_Object_i.o

VLDLIBS = $(LDLIBS:%=%$(VAR))
BIN = client server

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao

.PRECIOUS: MT_Client_TestC.h MT_Client_TestC.i MT_Client_TestC.cpp
.PRECIOUS: MT_Client_TestS.h MT_Client_TestS.i MT_Client_TestS.cpp
.PRECIOUS: MT_Client_TestS_T.h MT_Client_TestS_T.i MT_Client_TestS_T.cpp

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) MT_Client_Test[CS].* MT_Client_TestS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
