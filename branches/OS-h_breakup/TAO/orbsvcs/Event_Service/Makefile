# $Id$

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = Event_Service.cpp
ES_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lTAO_RTOLDEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_Messaging -lTAO_PortableServer -lTAO

BIN2 = Event_Service

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (RTOLDEvent,$(findstring RTOLDEvent,$(TAO_ORBSVCS)))
  ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
    BIN = $(BIN2)
  endif # ACE_HAS_GNUG_PRE_2_8
endif # RTOLDEvent

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

Event_Service: $(addprefix $(VDIR),$(ES_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(LYNXTARGET),ppc)
#### Event_Service.cpp raises internal compiler error with LynxOS 3.0.0 g++,
#### but only when -O2 optimization is enabled.  Disable it . . .
.obj/Event_Service.o .obj/Event_Service.so .shobj/Event_Service.o .shobj/Event_Service.so: Event_Service.cpp
	@echo NOTE: compiling Event_Service.cpp with -O instead of -O2, see Makefile.
	$(subst $(OCFLAGS) ,-O ,$(COMPILE.cc)) -o $@ $<
endif # LYNXTARGET == ppc

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
