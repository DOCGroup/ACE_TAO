#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

TAO_IFR_CLNT_LIBS = -lTAO_IFR_Client

IDL_FILES = test
IDL_SRC = testC.cpp testS.cpp
IDL_HDR = testC.h testS.h

CLNT_SRCS = client.cpp ifr_dii_client.cpp

SVR_SRCS = testC.cpp testS.cpp server.cpp test_i.cpp

SRC = $(PROG_SRCS) $(SVR_SRCS)

CLNT_TEST_OBJS = client.o ifr_dii_client.o

SVR_TEST_OBJS = testC.o testS.o server.o test_i.o

BIN = client server

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(corba_messaging),1)
BIN=$(BIN_UNCHECKED)
endif # corba_messaging

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao

.PRECIOUS: testC.cpp testS.cpp testC.h testS.h testC.i testS.i \
        testS_T.h testS_T.cpp testS_T.i

client: $(addprefix $(VDIR),$(CLNT_TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_IFR_CLNT_LIBS) $(TAO_DYNAMIC_LIBS) $(TAO_SRVR_LIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SVR_TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_IFR_CLNT_LIBS) $(TAO_SRVR_LIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
