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
include $(ACE_ROOT)/include/makeinclude/macros.GNU
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
                Sched

  ifeq ($(corba_messaging),1)
    TAO_ORBSVCS += RTEvent \
                   NativeCosEvent \
                   CosEvent \
                   RTOLDEvent \
                   RTSchedEvent \
                   EventLog \
                   RTEventLog \
                   NotifyLog
  endif #corba_messaging

  TAO_ORBSVCS += Concurrency \
                 Trader \
                 AV \
                 Log

  ifeq ($(minimum_corba),0)
      ifneq (Compilers 4.2,$(findstring Compilers 4.2,$(CC_VERSION)))
        ifeq ($(LYNXTARGET),)
	  TAO_ORBSVCS += IFRService
	endif # LYNXTARGET
      endif # findstring Compilers 4.2
  endif # minimum_corba

  ifeq ($(minimum_corba),0)

    # Trader uses DynAny which is disabled when compiling with minimum
    # CORBA support.  LifeCycle, Log, AV and Notify depend on Trader
    # so don't build them either, with minimum CORBA.

    TAO_ORBSVCS += LifeCycle

    ifeq ($(corba_messaging),1)
      TAO_ORBSVCS += FaultTolerance
      TAO_ORBSVCS += FTORB
      TAO_ORBSVCS += RTCORBAEvent
    endif #corba_messaging

    # The PortableGroup library requires several methods in the POA,
    # which are disabled by Minimum CORBA. Further it also needs AMI
    # support
    ifeq ($(ami),1)
      TAO_ORBSVCS += PortableGroup
      ifeq ($(interceptors),1)
        # The Load Balancer requires AMI and Interceptor support.
        TAO_ORBSVCS += CosLoadBalancing
      endif #interceptors
    endif #ami

    TAO_ORBSVCS += ImplRepo

  endif # minimum_corba

  ifeq ($(ssl),1)
    ifeq ($(corba_messaging),1)
      ifeq ($(interceptors),1)
        TAO_ORBSVCS += Security SSLIOP
      endif # interceptors
      endif #corba_messaging
  endif # ssl

  TAO_ORBSVCS += Notify

  ifeq ($(rt_corba),1)
    TAO_ORBSVCS += RT_Notify
  endif #rt_corba

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
ifneq (,$(findstring  CosLoadBalancing, $(TAO_ORBSVCS)))
  MKLIST += \
	Makefile.CosNaming \
	Makefile.PortableGroup \
	Makefile.CosLoadBalancing
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
ifneq (,$(findstring  RTOLDEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTOLDEvent, $(MKLIST)))
    MKLIST += Makefile.RTOLDEvent
  endif
endif
ifneq (,$(findstring  RTSchedEvent, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTSchedEvent
endif
ifneq (,$(findstring  RTCORBAEvent, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTCORBAEvent
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
  MKLIST += Makefile.ETCL Makefile.Svc_Utils Makefile.DsLogAdmin
endif
ifneq (,$(findstring  EventLog, $(TAO_ORBSVCS)))
  # The EventLog service depends on the CosEvent service.
  ifeq (,$(findstring  CosEvent, $(TAO_ORBSVCS)))
    MKLIST += Makefile.CosEvent Makefile.DsLogAdmin
  endif
  MKLIST += Makefile.DsEventLogAdmin
endif
ifneq (,$(findstring  NotifyLog, $(TAO_ORBSVCS)))
  # The NotifyLog service depends on the Notification service.
  ifeq (,$(findstring  Notify, $(TAO_ORBSVCS)))
    MKLIST += \
      Makefile.ETCL \
      Makefile.CosNotification \
      Makefile.DsLogAdmin \
      Makefile.DsEventLogAdmin
  endif
  MKLIST += Makefile.DsNotifyLogAdmin
endif
ifneq (,$(findstring  RTEventLog, $(TAO_ORBSVCS)))
  # The RTEventLog service depends on the RTEvent service.
  ifeq (,$(findstring  RTEvent, $(TAO_ORBSVCS)))
    MKLIST += Makefile.RTEvent Makefile.DsLogAdmin
  endif
  MKLIST += Makefile.RTEventLogAdmin
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
  MKLIST += Makefile.CosNaming Makefile.PortableGroup
endif
ifneq (,$(findstring  IFRService, $(TAO_ORBSVCS)))
  MKLIST += Makefile.IFRService
endif
ifneq (,$(findstring  RT_Notify, $(TAO_ORBSVCS)))
  MKLIST += Makefile.ETCL Makefile.CosNotification Makefile.RT_Notification
endif
#----------------------------------------------------------------------------
#       Local rules
#----------------------------------------------------------------------------

## If parallel makes (-j) are invoked , we want to make sure that the order
## of the Makefiles invoked from this Makefile is preserved.
## Recursively invoked makes from this file will still be run in parallel.
.NOTPARALLEL:

all: ORBSVCS_COMPONENTS

realclean: ORBSVCS_COMPONENTS_realclean

## Makefile.Svc_Utils.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.Svc_Utils to be invoked
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)

# This rule invokes make again with the list of .mkfile targets as a
# parameter.  For example, if the all target is being made, make is invoked
# as follows:
#
# make -f Makefile MKFILE_TARGET=all Makefile.Svc_Utils.mkfile \
#                                    Makefile.CosNaming.mkfile .....
all debug profile install deinstall clean clobber depend rcs_info idl_stubs realclean:
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif

.PHONY: ORBSVCS_COMPONENTS
ORBSVCS_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --orbsvcs --set ' $(TAO_ORBSVCS) '

.PHONY: ORBSVCS_COMPONENTS_realclean
ORBSVCS_COMPONENTS_realclean:
	-$(RM) ORBSVCS_COMPONENTS.list

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
