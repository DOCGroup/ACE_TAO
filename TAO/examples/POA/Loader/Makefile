#
# $Id$
#

BIN = server

MYFILES = Server_Manager Servant_Activator Servant_Locator 

SRC = $(addsuffix .cpp,$(MYFILES)) $(addsuffix .cpp, $(BIN))
OBJ = $(addsuffix .o,$(MYFILES))

CPPFLAGS += -I$(TAO_ROOT) 

LDLIBS = -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

INSTALL =

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
