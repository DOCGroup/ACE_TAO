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

LDLIBS = -lTAO_CosEvent -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO

IDL_FILES = CosEventChannelFactory
IDL_SRC = CosEventChannelFactoryC.cpp CosEventChannelFactoryS.cpp

PROG_SRCS = \
	CosEventChannelFactory_i.cpp \
	FactoryClient.cpp \
	FactoryDriver.cpp \
	main.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

FACTORY_OBJS = \
	 CosEventChannelFactoryC.o \
	 CosEventChannelFactoryS.o \
	 CosEventChannelFactory_i.o \
	 FactoryDriver.o \
	 main.o

CLIENT_OBJS = \
	FactoryClient.o \
        CosEventChannelFactoryC.o \
	CosEventChannelFactoryS.o

BIN2 = factory \
	factoryclient

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
    ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
      BIN = $(BIN2)
    endif # ACE_HAS_GNUG_PRE_2_8
  endif # Naming
endif # CosEvent

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

override TAO_IDLFLAGS += -I$(TAO_ROOT)/orbsvcs -Ge 1

factory: $(addprefix $(VDIR),$(FACTORY_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

factoryclient: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) -rf CosEventChannelFactoryC.* CosEventChannelFactoryS.* CosEventChannelFactoryS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
