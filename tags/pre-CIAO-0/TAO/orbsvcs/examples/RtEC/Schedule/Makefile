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

BIN2 =   Service

#### If the orbsvcs library wasn't built with all components, don't
#### try to build certain tests.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  ifeq (Sched,$(findstring Sched,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # Sched
endif # Event

PSRC= Service.cpp Supplier.cpp Consumer.cpp
LDLIBS = -lTAO_RTOLDEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_Messaging -lTAO_PortableServer -lTAO

# The complete path to orbsvcs/orbsvcs/Sched is required for DU/CXX
# automatic template instantiation magic.
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs \
	-I$(TAO_ROOT)/orbsvcs/orbsvcs/Sched \
        $(foreach svc, $(TAO_ORBSVCS), -DTAO_ORBSVCS_HAS_$(svc))

Service_OBJS=$(addsuffix .o, Service Supplier Consumer)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
  # This example does not compile with g++ 2.7.2.3
all clean realclean:
	@echo orbsvcs/examples/RtEC/Schedule is not built with older versions of g++
else # ! ACE_HAS_GNUG_PRE_2_8
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
endif # ! ACE_HAS_GNUG_PRE_2_8

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

Service: $(addprefix $(VDIR),$(Service_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
