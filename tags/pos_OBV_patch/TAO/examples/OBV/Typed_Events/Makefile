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

IDL_SRC	= Event_TypesC.cpp Event_TypesS.cpp

PROG_SRCS = \
	client.cpp \
	server.cpp \
	Server_i.cpp \
	Client_i.cpp \
	Event_Types_impl.cpp \
	../Simple_util.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		Event_TypesC.o \
		Event_TypesS.o \
	        Client_i.o \
		Event_Types_impl.o \
		../Simple_util.o \
		client.o
SIMPLE_SVR_OBJS = \
		Event_TypesC.o \
		Event_TypesS.o \
		Server_i.o \
		Event_Types_impl.o \
		../Simple_util.o \
		server.o

BIN = server \
      client

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

LDFLAGS +=  -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs
override TAO_IDLFLAGS += -Gc -Gv

.PRECIOUS: Event_TypesC.cpp Event_TypesC.i Event_TypesC.h
.PRECIOUS: Event_TypesS.cpp Event_TypesS.i Event_TypesS.h
.PRECIOUS: Event_TypesS_T.cpp Event_TypesS_T.i Event_TypesS_T.h

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf Event_TypesC.* Event_TypesS.* Event_TypesS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

