#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC	= PP_TestC.cpp PP_TestS.cpp
PROG_SRCS = PP_Test_Server.cpp \
	    server.cpp \
            PP_Test_Client.cpp \
	    client.cpp \
	    PP_Test_i.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

PP_TEST_SVR_OBJS = PP_TestC.o \
		 PP_TestS.o \
		 PP_Test_Server.o \
		 PP_Test_i.o \
		 server.o
PP_TEST_CLT_OBJS = PP_TestC.o \
		 PP_TestS.o \
		 PP_Test_Client.o \
		 client.o

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

LDFLAGS += -L$(TAO_ROOT)/tao

.PRECIOUS: PP_TestC.cpp PP_TestS.cpp PP_TestC.h PP_TestS.h
#$(IDL_SRC): PP_Test.idl
#	$(TAO_ROOT)/TAO_IDL/tao_idl PP_Test.idl

server:	$(addprefix $(VDIR),$(PP_TEST_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(PP_TEST_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf PP_TestC.* PP_TestS.* PP_TestS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

