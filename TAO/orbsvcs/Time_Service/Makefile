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
endif

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = TimeServiceC.cpp TimeServiceS.cpp

PROG_SRCS = \
	server.cpp \
	clerk.cpp \
	Clerk_i.cpp \
	Server_i.cpp \

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_SVR_OBJS = Server_i.o \
	server.o

SIMPLE_CRK_OBJS = Clerk_i.o \
	clerk.o \

BIN = server \
      clerk

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


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: TimeServiceC.cpp TimeServiceC.i TimeServiceC.h
.PRECIOUS: TimeServiceS.cpp TimeServiceS.i TimeServiceS.h
.PRECIOUS: TimeServiceS_T.cpp TimeServiceS_T.i TimeServiceS_T.h

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clerk:	$(addprefix $(VDIR),$(SIMPLE_CRK_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf TimeServiceC.* TimeServiceS.* TimeServiceS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
