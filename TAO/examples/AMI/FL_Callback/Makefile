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

LDLIBS = -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO

IDL_FILES = test
IDL_SRC = testC.cpp testS.cpp
BIN_UNCHECKED = progress peer

SRC = $(addsuffix .cpp, $(BIN) Peer_i Progress_i) \
	$(IDL_SRC)

PROGRESS_OBJS = progress.o Progress_i.o $(IDL_SRC:.cpp=.o)
PEER_OBJS = peer.o Peer_i.o $(IDL_SRC:.cpp=.o)

BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

TAO_IDLFLAGS += -Ge 1 -GC
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(fl_reactor),1)
  ifeq ($(ami),1)
    BIN=$(BIN_UNCHECKED)
  endif # ami
endif # fl_reactor

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: testC.h testC.i testC.cpp
.PRECIOUS: testS.h testS.i testS.cpp
.PRECIOUS: testS_T.h testS_T.i testS_T.cpp

progress: $(addprefix $(VDIR),$(PROGRESS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

peer: $(addprefix $(VDIR),$(PEER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) testC.* testS.* testS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
