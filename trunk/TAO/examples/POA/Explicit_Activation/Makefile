#
# $Id$
#

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

BIN_UNCHECKED = server

LSRC = 	$(addsuffix .cpp,$(BIN))

CPPFLAGS += -I$(TAO_ROOT) -I../Generic_Servant

LDLIBS   = -lGeneric_Servant -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao -L../Generic_Servant

INSTALL =
TAO_IDLFLAGS += -Ge 1

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)
  BIN = $(BIN_UNCHECKED)
endif # minimum_corba

include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
