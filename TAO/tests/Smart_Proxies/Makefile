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
LDFLAGS += -L$(ACE_ROOT)/bin

IDLFILES = testC testS
BIN_UNCHECKED = client server
DIRS=On_Demand
FILES = Smart_Proxy_Impl

SRC = $(addsuffix .cpp, $(BIN) $(IDLFILES) $(FILES))

CLIENT_OBJS = client.o testC.o Smart_Proxy_Impl.o
SERVER_OBJS = server.o testC.o testS.o

TAO_IDLFLAGS += -Ge 1 -Gsp

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(smart_proxies),1)
BIN=$(BIN_UNCHECKED)
endif # smart_proxies

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS +=

.PRECIOUS: testC.h testC.i testC.cpp
.PRECIOUS: testS.h testS.i testS.cpp

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
