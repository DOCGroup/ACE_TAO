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

IDL_SRC	= GridC.cpp GridS.cpp

PROG_SRCS = \
	client.cpp \
	Client_i.cpp \
	server.cpp \
	Server_i.cpp \
	Grid_i.cpp \

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		GridC.o \
		GridS.o \
		Client_i.o \
		client.o
SIMPLE_SVR_OBJS = \
		GridC.o \
		GridS.o \
		Grid_i.o \
		Server_i.o \
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

LDFLAGS +=  -L$(TAO_ROOT)/tao
CPPFLAGS +=

.PRECIOUS: GridC.cpp GridC.i GridC.h
.PRECIOUS: GridS.cpp GridS.i GridS.h
.PRECIOUS: GridS_T.cpp GridS_T.i GridS_T.h

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf GridC.* GridS.* GridS_T.* 

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
