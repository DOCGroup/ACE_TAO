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

LDLIBS = -lTAO

IDLFILES = testC testS
BIN_UNCHECKED = progress peer

SRC = $(addsuffix .cpp, $(BIN) $(IDLFILES) Peer_i Progress_i)

PROGRESS_OBJS = progress.o Progress_i.o $(addsuffix .o, $(IDLFILES))
PEER_OBJS = peer.o Peer_i.o $(addsuffix .o, $(IDLFILES))

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
