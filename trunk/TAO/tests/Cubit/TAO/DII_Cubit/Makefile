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

DIRS    = ../IDL_Cubit

LDLIBS = ../IDL_Cubit/.obj/cubitC.o \
	 ../IDL_Cubit/.obj/cubitS.o \
	 -lorbsvcs -lTAO

PROG_SRCS = client.cpp

LSRC = $(PROG_SRCS)

CUBIT_CLT_OBJS = client.o 

CFLAGS += -I../IDL_Cubit

BIN = client
BUILD = $(BIN) ../IDL_Cubit/server
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

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs \
	   -L$(TAO_ROOT)/tao \
	   -L$(TAO_ROOT)/orbsvcs/Naming_Service

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: cubitC.cpp cubitS.cpp cubitC.h cubitS.h

client:	$(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

../IDL_Cubit/server:
	cd ../IDL_Cubit; $(MAKE) BUILD=server
	
realclean: clean
	-/bin/rm -rf cubitC.* cubitS.* cubitS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
