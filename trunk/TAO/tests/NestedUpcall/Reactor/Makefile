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

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = ReactorC.cpp ReactorS.cpp
PROG_SRCS = $(IDL_SRC) server.cpp client.cpp #tmplinst.cpp

NESTEDUPCALLS_SVR_OBJS = ReactorS.o ReactorC.o server.o reactor_i.o
NESTEDUPCALLS_CLT_OBJS = ReactorS.o ReactorC.o client.o eh_i.o

SRC = $(sort $(NESTEDUPCALLS_SVR_OBJS:.o=.cpp) \
             $(NESTEDUPCALLS_CLT_OBJS:.o=.cpp))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = server client

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

.PRECIOUS: ReactorC.h ReactorC.i ReactorC.cpp
.PRECIOUS: ReactorS.h ReactorS.i ReactorS.cpp

server: $(addprefix $(VDIR),$(NESTEDUPCALLS_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(NESTEDUPCALLS_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) ReactorS.* ReactorC.* ReactorS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
