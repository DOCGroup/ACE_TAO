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

BIN_UNCHECKED = Destroy_Thread_Pool

SRC = $(addsuffix .cpp, $(BIN)) 

DESTROY_THREAD_POOL_OBJS = $(SRC:.cpp=.o)

BUILD = $(BIN)
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(rt_corba),1)
BIN=$(BIN_UNCHECKED)
endif # rt_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

Destroy_Thread_Pool: $(addprefix $(VDIR),$(DESTROY_THREAD_POOL_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(RT_TAO_CLNT_LIBS) $(POSTLINK)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
