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
endif

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = GridC.cpp GridS.cpp

PROG_SRCS = \
        client.cpp \
        server.cpp \
        Grid_i.cpp \
        Grid_Client_i.cpp \
        ../Simple_util.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
                GridC.o \
                GridS.o \
                client.o \
                ../Simple_util.o \
                Grid_Client_i.o
SIMPLE_SVR_OBJS = \
                GridC.o \
                GridS.o \
                Grid_i.o \
                server.o \
                ../Simple_util.o

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


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: GridC.cpp GridC.i GridC.h
.PRECIOUS: GridS.cpp GridS.i GridS.h
.PRECIOUS: GridS_T.cpp GridS_T.i GridS_T.h

server: $(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) GridC.* GridS.* GridS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
