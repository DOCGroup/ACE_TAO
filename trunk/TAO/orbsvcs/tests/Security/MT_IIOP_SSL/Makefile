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

IDL_FILES = test
IDL_SRC = testC.cpp testS.cpp
BIN_UNCHECKED = client server

SRC = $(addsuffix .cpp, $(BIN) test_i Client_Worker Server_Worker) $(IDL_SRC)


CLIENT_OBJS = Client_Worker.o client.o testC.o
SERVER_OBJS = Server_Worker.o server.o test_i.o $(IDL_SRC:.cpp=.o)

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (SSLIOP,$(findstring SSLIOP,$(TAO_ORBSVCS)))
    BIN = $(BIN_UNCHECKED)
    CPPFLAGS += $(PLATFORM_SSL_CPPFLAGS)
    LDFLAGS += $(PLATFORM_SSL_LDFLAGS)
endif #SSLIOP
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

CLNT_LIBS = -lTAO_SSLIOP $(TAO_CLNT_LIBS)
SRVR_LIBS = -lTAO_SSLIOP -lACE_SSL $(PLATFORM_SSL_LIBS) $(TAO_SRVR_LIBS)

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(SRVR_LIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(CLNT_LIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

