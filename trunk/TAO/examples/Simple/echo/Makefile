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

LDLIBS = -lTAO -lorbsvcs

IDL_SRC = EchoC.cpp EchoS.cpp

PROG_SRCS = \
        client.cpp \
        server.cpp \
        Echo_i.cpp \
        Echo_Client_i.cpp \
        ../Simple_util.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
                EchoC.o \
                EchoS.o \
                Echo_Client_i.o \
                ../Simple_util.o \
                client.o
SIMPLE_SVR_OBJS = \
                EchoC.o \
                EchoS.o \
                Echo_i.o \
                ../Simple_util.o \
                server.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = server \
       client

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Naming

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

LDFLAGS +=  -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: EchoC.cpp EchoC.i EchoC.h
.PRECIOUS: EchoS.cpp EchoS.i EchoS.h
.PRECIOUS: EchoS_T.cpp EchoS_T.i EchoS_T.h

server: $(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) -rf EchoC.* EchoS.* EchoS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
