#----------------------------------------------------------------------------
#
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_FILES = Reactor
IDL_SRC = $(IDL_FILES:%=%.idl)
IDL_GENERATED = $(foreach idlfile,$(IDL_FILES),$(addprefix $(idlfile),C.cpp S.cpp))
PROG_SRCS = $(IDL_GENERATED) server.cpp client.cpp \
            reactor_i.cpp eh_i.cpp tmplinst.cpp

LSRC = $(PROG_SRCS)

SVR_OBJS = $(IDL_GENERATED:%.cpp=%.o) server.o reactor_i.o #tmplinst.o
CLT_OBJS = $(IDL_GENERATED:%.cpp=%.o) client.o eh_i.o #tmplinst.o

BIN = server client
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

DCFLAGS = -g

server:	$(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

.PRECIOUS:	$(IDL_GENERATED)

realclean: clean
	-/bin/rm -rf $(IDL_GENERATED)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
