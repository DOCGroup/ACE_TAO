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

BIN_UNCHECKED = server client

CLIENT_OBJS = 	client.o
SERVER_OBJS = 	server.o

BUILD = $(BIN)
VBIN = $(BIN:%=%$(VAR))
SRC=$(CLIENT_OBJS:.o=.cpp) $(SERVER_OBJS:.o=.cpp)
CPPFLAGS += -I../Common

TAO_IDL_FLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)
  ifeq ($(rt_corba),1)
    BIN=$(BIN_UNCHECKED)
  endif # rt_corba
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

RT_TAO_SRVR_LIBS := -lRTCORBA_Common -lTAO_Strategies $(RT_TAO_SRVR_LIBS)
RT_TAO_CLNT_LIBS := -lRTCORBA_Common -lTAO_Strategies $(RT_TAO_CLNT_LIBS)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(RT_TAO_SRVR_LIBS) $(LDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(RT_TAO_CLNT_LIBS) $(LDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
