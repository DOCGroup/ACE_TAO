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




#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)

ifeq (CosLoadBalancing,$(findstring CosLoadBalancing,$(TAO_ORBSVCS)))

  BIN = LoadManager LoadMonitor

# The Load Balancer requires AMI.
  ifeq ($(ami),0)
    BIN =
  endif  # !ami

  ifeq ($(interceptors),0)
    BIN =
  endif  # !interceptors

endif  # CosLoadBalancing

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

all: $(BIN)

LoadManager:
	$(MAKE) -f Makefile.LoadManager

LoadMonitor:
	$(MAKE) -f Makefile.LoadMonitor

realclean depend:
	@for m in Makefile.LoadManager Makefile.LoadMonitor; do \
          $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
	done

.PHONY: LoadManager LoadMonitor

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
