#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Define local macros
#---------------------------------------------------------------------------

ifndef TAO_ORBSVCS
  #### Please see docs/configurations.html#orbsvcs for documentation of
  #### TAO_ORBSVCS.
  TAO_ORBSVCS = Naming \
                Time \
                Property \
		RTEvent \
		NativeCosEvent \
                CosEvent \
                Sched \
                RTOLDEvent \
		RTSchedEvent \
                Concurrency \
		Trader \
		Log \
		AV \
		PortableGroup

  ifeq ($(minimum_corba),0)

    # Trader uses DynAny which is disabled when compiling with minimum
    # CORBA support.  LifeCycle, Log, AV and Notify depend on Trader
    # so don't build them either, with minimum CORBA.
    TAO_ORBSVCS +=

    ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
      TAO_ORBSVCS += LifeCycle

    ifeq ($(corba_messaging),1)
      TAO_ORBSVCS += FaultTolerance
      TAO_ORBSVCS += FTORB
    endif #corba_messaging
    endif # ACE_HAS_GNUG_PRE_2_8

    # The Load Balancer requires POA ServantLocator support, which
    # Minimum CORBA disables.
    TAO_ORBSVCS += LoadBalancing

  endif # minimum_corba

  ifeq ($(ssl),1)
    TAO_ORBSVCS += Security SSLIOP
  endif # ssl

  ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
    TAO_ORBSVCS += Notify
  endif # ACE_HAS_GNUG_PRE_2_8

endif # TAO_ORBSVCS

# The order in this list is important as some libraries depend on
# others, please keep that in mind when adding or changing libraries.
MKLIST = Makefile.Svc_Utils
ifneq (,$(findstring Naming,$(TAO_ORBSVCS)))
  MKLIST += Makefile.CosNaming
endif
ifneq (,$(findstring  Time, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosTime
endif
ifneq (,$(findstring  LoadBalancing, $(TAO_ORBSVCS)))
  MKLIST += Makefile.LoadBalancing
endif
ifneq (,$(findstring  Concurrency, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosConcurrency
endif
ifneq (,$(findstring  Property, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosProperty
endif
ifneq (,$(findstring  AV, $(TAO_ORBSVCS)))
  # AV service depends on CosNaming and CosProperty
  ifeq (,$(findstring Naming,$(TAO_ORBSVCS)))
    MKLIST += Makefile.CosNaming
    TAO_ORBSVCS += Naming
  endif
  ifeq (,$(findstring  Property, $(TAO_ORBSVCS)))
    MKLIST += Makefile.CosProperty
    TAO_ORBSVCS += Property
  endif

  MKLIST += Makefile.av
endif
ifneq (,$(findstring  LifeCycle, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosLifeCycle
endif
ifneq (,$(findstring  RTEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTEvent, $(MKLIST)))
    MKLIST += Makefile.RTEvent
  endif
endif
# Backwards compatibility, going away soon.
ifneq (,$(findstring  Event2, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTEvent, $(MKLIST)))
    MKLIST += Makefile.RTEvent
  endif
endif
ifneq (,$(findstring  Sched, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTSched
endif
ifneq (,$(findstring  Event, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTEvent, $(MKLIST)))
    MKLIST += Makefile.RTEvent
  endif
  ifeq (,$(findstring Makefile.RTSched, $(MKLIST)))
    MKLIST += Makefile.RTSched
  endif
  ifeq (,$(findstring Makefile.RTOLDEvent, $(MKLIST)))
    MKLIST += Makefile.RTOLDEvent
  endif
endif
ifneq (,$(findstring  Event, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTEvent, $(MKLIST)))
    MKLIST += Makefile.RTEvent
  endif
  ifeq (,$(findstring Makefile.RTSched, $(MKLIST)))
    MKLIST += Makefile.RTSched
  endif
  ifeq (,$(findstring Makefile.RTOLDEvent, $(MKLIST)))
    MKLIST += Makefile.RTOLDEvent
  endif
endif
ifneq (,$(findstring  RTOLDEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTOLDEvent, $(MKLIST)))
    MKLIST += Makefile.RTOLDEvent
  endif
endif
ifneq (,$(findstring  RTSchedEvent, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTSchedEvent
endif
ifneq (,$(findstring  CosEvent, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosEvent
endif
ifneq (,$(findstring  NativeCosEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.CosEvent, $(MKLIST)))
    MKLIST += Makefile.CosEvent
  endif
endif
ifneq (,$(findstring  Trader, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosTrading
endif
ifneq (,$(findstring  Notify, $(TAO_ORBSVCS)))
  MKLIST += Makefile.ETCL Makefile.CosNotification
endif
ifneq (,$(findstring  Log, $(TAO_ORBSVCS)))
  MKLIST += Makefile.DsLogAdmin
endif
ifneq (,$(findstring  Security, $(TAO_ORBSVCS)))
  MKLIST += Makefile.Security
endif
ifneq (,$(findstring  SSLIOP, $(TAO_ORBSVCS)))
  # SSLIOP depends on the Security Service library
  ifeq (,$(findstring  Security, $(TAO_ORBSVCS)))
    MKLIST += Makefile.Security
  endif
  MKLIST += Makefile.SSLIOP
endif
ifneq (,$(findstring  FTORB, $(TAO_ORBSVCS)))
  MKLIST += Makefile.FTORB
endif
ifneq (,$(findstring  FaultTolerance, $(TAO_ORBSVCS)))
  MKLIST += Makefile.FTORB Makefile.FaultTolerance
endif
ifneq (,$(findstring  PortableGroup, $(TAO_ORBSVCS)))
  MKLIST += Makefile.PortableGroup
endif

#----------------------------------------------------------------------------
#       Local rules
#----------------------------------------------------------------------------

all: ORBSVCS_COMPONENTS

realclean::
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST

all debug profile optimize install deinstall clean clobber depend rcs_info idl_stubs:
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST

.PHONY: ORBSVCS_COMPONENTS
ORBSVCS_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --orbsvcs --set ' $(TAO_ORBSVCS) '


realclean::
	-$(RM) ORBSVCS_COMPONENTS.list

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
