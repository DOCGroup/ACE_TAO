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

IDL_SRC = Triangle_TestC.cpp Triangle_TestS.cpp
PROG_SRCS = $(IDL_SRC) server_A.cpp server_B.cpp \
        Object_A_i.cpp Object_B_i.cpp Initiator_i.cpp \
        initiator.cpp

LSRC = $(PROG_SRCS)

NESTEDUPCALLS_SVR_A_OBJS = Triangle_TestS.o Triangle_TestC.o server_A.o \
        Object_A_i.o

NESTEDUPCALLS_SVR_B_OBJS = Triangle_TestS.o Triangle_TestC.o server_B.o \
        Object_B_i.o

NESTEDUPCALLS_INITIATOR_OBJS = Triangle_TestS.o Triangle_TestC.o initiator.o \
        Initiator_i.o


BIN = server_A server_B initiator
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

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

.PRECIOUS: Trangle_TestC.h Triangle_TestC.i Triangle_TestC.cpp
.PRECIOUS: Triangle_TestS.h Triangle_TestS.i Triangle_TestS.cpp

server_A: $(addprefix $(VDIR),$(NESTEDUPCALLS_SVR_A_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server_B: $(addprefix $(VDIR),$(NESTEDUPCALLS_SVR_B_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

initiator: $(addprefix $(VDIR),$(NESTEDUPCALLS_INITIATOR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) Triangle_TestS.* Triangle_TestC.* Triangle_TestS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
