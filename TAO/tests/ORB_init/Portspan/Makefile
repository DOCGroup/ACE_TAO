#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDLIBS = -lTAO -lACE
LDFLAGS += -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
VLDLIBS =  $(LDLIBS:%=%$(VAR))

SERVER_SRCS = server.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
SERVER_BIN  = server

BIN         = $(SERVER_BIN) 

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

#INCLDIRS += $(TAO_ROOT)

$(SERVER_BIN):	$(addprefix $(VDIR),$(SERVER_OBJS))
		$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean:
	$(RM) server
