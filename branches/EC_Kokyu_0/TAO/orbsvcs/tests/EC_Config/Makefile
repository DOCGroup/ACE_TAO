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

BIN2 =   Test

#### If the orbsvcs library wasn't built with all components, don't
#### try to build certain tests.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  ifeq (Sched,$(findstring Sched,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # Sched
endif # Event

PSRC= Test.cpp Supplier.cpp Consumer.cpp ECConfig.cpp Service.cpp Config_Factory.cpp Test_Handler.cpp TestConfig.cpp
LDLIBS = -lTAO_RTOLDEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_Messaging -lTAO_PortableServer -lTAO -lTAO_RTKokyuEvent -lACEXML_Parser

# The complete path to orbsvcs/orbsvcs/Sched is required for DU/CXX
# automatic template instantiation magic.
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs \
	-I$(TAO_ROOT)/orbsvcs/orbsvcs/Sched \
        $(foreach svc, $(TAO_ORBSVCS), -DTAO_ORBSVCS_HAS_$(svc))

Test_OBJS=$(addsuffix .o, Test Supplier Consumer ECConfig Config_Factory Test_Handler TestConfig)
Service_OBJS=$(addsuffix .o, Service Supplier Consumer ECConfig TestConfig)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

Test: $(addprefix $(VDIR),$(Test_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Service: $(addprefix $(VDIR),$(Service_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
