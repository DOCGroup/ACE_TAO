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
  TAO_ORBSVCS = Property \
                Concurrency

  ifeq ($(corba_messaging),1)
  TAO_ORBSVCS += Naming \
                 Time \
                 RTSched

    TAO_ORBSVCS += RTEvent \
                   NativeCosEvent \
                   CosEvent \
                   RTOLDEvent \
                   RTSchedEvent \
		   RTKokyuEvent \
                   CosEventLog \
                   RTEventLog \
                   NotifyLog

  TAO_ORBSVCS += Trader \
                 AV \
                 Log
  endif #corba_messaging

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
    ifeq ($(corba_messaging),1)
      TAO_ORBSVCS += LifeCycle
      TAO_ORBSVCS += FaultTolerance
      TAO_ORBSVCS += FTORB
      TAO_ORBSVCS += RTCORBAEvent

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
    endif #corba_messaging

    TAO_ORBSVCS += ImplRepo # This is for Craig R

  endif # minimum_corba

  ifeq ($(interceptors),1)
      TAO_ORBSVCS += RTCosScheduling
  endif #RTCosScheduling

  ifeq ($(ssl),1)
    ifeq ($(corba_messaging),1)
      ifeq ($(interceptors),1)
        TAO_ORBSVCS += Security SSLIOP
      endif # interceptors
      endif #corba_messaging
  endif # ssl

   ifeq ($(corba_messaging),1)
     TAO_ORBSVCS += Notify
   endif # corba_messaging

  ifeq ($(rt_corba),1)
    TAO_ORBSVCS += RT_Notify
  endif #rt_corba

endif # TAO_ORBSVCS

# The order in this list is no longer important.  Inter=library dependencies
# are handled by the targets at the bottom of this file.
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
ifneq (,$(findstring  RTSched, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTSched, $(MKLIST)))
    MKLIST += Makefile.RTSched
  endif
endif
ifneq (,$(findstring  RTOLDEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTOLDEvent, $(MKLIST)))
    MKLIST += Makefile.RTOLDEvent
  endif
endif
ifneq (,$(findstring  RTSchedEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTSchedEvent, $(MKLIST)))
    MKLIST += Makefile.RTSchedEvent
  endif
endif
ifneq (,$(findstring  RTKokyuEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTKokyuEvent, $(MKLIST)))
    MKLIST += Makefile.RTKokyuEvent
  endif
endif
ifneq (,$(findstring  RTCosScheduling, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTCosScheduling, $(MKLIST)))
    MKLIST += Makefile.RTCosScheduling
  endif
endif
ifneq (,$(findstring  RTCORBAEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTCORBAEvent, $(MKLIST)))
    MKLIST += Makefile.RTCORBAEvent
  endif
endif
ifneq (,$(findstring  CosEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.CosEvent, $(MKLIST)))
    MKLIST += Makefile.CosEvent
  endif
endif
ifneq (,$(findstring  Trader, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.CosTrading, $(MKLIST)))
    MKLIST += Makefile.CosTrading
  endif
endif
ifneq (,$(findstring  Notify, $(TAO_ORBSVCS)))
  MKLIST += Makefile.ETCL Makefile.CosNotification
endif
ifneq (,$(findstring  Log, $(TAO_ORBSVCS)))
  MKLIST += Makefile.ETCL Makefile.Svc_Utils Makefile.DsLogAdmin
endif
ifneq (,$(findstring  CosEventLog, $(TAO_ORBSVCS)))
  # The CosEventLog service depends on the CosEvent service.
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
  MKLIST += Makefile.FTORB Makefile.PortableGroup
endif
ifneq (,$(findstring  FaultTolerance, $(TAO_ORBSVCS)))
  MKLIST += Makefile.FTORB Makefile.FaultTolerance Makefile.PortableGroup
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

# Since Makefiles can appear multiple times in MKLIST, we sort them to 
# remove duplicates and assign the list to "thelist".
thelist = $(sort $(MKLIST))
#----------------------------------------------------------------------------
#       Local rules
#----------------------------------------------------------------------------

# A list of all possible targets
targets = all debug profile install deinstall clean clobber depend rcs_info idl_stubs realclean

all: ORBSVCS_COMPONENTS

# This is the rule for all the standard targets listed above.  The dependencies
# are formed by adding "-<target name>" to each Makefile listed in MKLIST, e.g.,
# Makefile.CosNaming-all.
$(targets): %: $(addsuffix -%, $(thelist))

# This is the rule that matches targets like Makefile.CosNaming-all.  We use
# the subst function to split it into two pieces on the '-'.  The first part
# is the name of the Makefile we want to run, and the second is the name
# of the target we pass. e.g., make -f Makefile.CosNaming all
Makefile.%: 
	@echo $(MAKE) -f $(word 1,$(subst -, ,$@)) $(word 2,$(subst -, ,$@))
	@$(MAKE) -f $(word 1,$(subst -, ,$@)) $(word 2,$(subst -, ,$@))

realclean: ORBSVCS_COMPONENTS_realclean

.PHONY: ORBSVCS_COMPONENTS
ORBSVCS_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --orbsvcs --set ' $(TAO_ORBSVCS) '

.PHONY: ORBSVCS_COMPONENTS_realclean
ORBSVCS_COMPONENTS_realclean:
	-$(RM) ORBSVCS_COMPONENTS.list

# This list of targets maintains the inter-library dependencies.  The rule is
# a list of all possible Makefile-target combinations.  The part of the
# target matches the standard target and uses it to construct a list of
# dependencies that must be built first.  Follow this pattern when adding
# new services.
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.av-)): \
  Makefile.av-%: \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosEvent-)): \
  Makefile.CosEvent-%: \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosLifeCycle-)): \
  Makefile.CosLifeCycle-%: \
    Makefile.CosNaming-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosLoadBalancing-)): \
  Makefile.CosLoadBalancing-%: \
    Makefile.CosNaming-% \
    Makefile.PortableGroup-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosNaming-)): \
  Makefile.CosNaming-%: \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosNotification-)): \
  Makefile.CosNotification-%: \
    Makefile.Svc_Utils-% \
    Makefile.ETCL-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosTime-)): \
  Makefile.CosTime-%: \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.CosTrading-)): \
  Makefile.CosTrading-%: \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.DsEventLogAdmin-)): \
  Makefile.DsEventLogAdmin-%: \
    Makefile.DsLogAdmin-% \
    Makefile.CosEvent-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.DsLogAdmin-)): \
  Makefile.DsLogAdmin-%: \
    Makefile.ETCL-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.DsNotifyLogAdmin-)): \
  Makefile.DsNotifyLogAdmin-%: \
    Makefile.DsEventLogAdmin-% \
    Makefile.DsLogAdmin-% \
    Makefile.CosNotification-% \
    Makefile.CosNaming-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.FTORB-)): \
  Makefile.FTORB-%: \
    Makefile.PortableGroup-% \
    Makefile.CosNaming-% \
    Makefile.CosNotification-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.FaultTolerance-)): \
  Makefile.FaultTolerance-%: \
    Makefile.FTORB-% \
    Makefile.PortableGroup-% \
    Makefile.CosNaming-% \
    Makefile.CosNotification-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.IFRService-)): \
  Makefile.IFRService-%: \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.Metrics-)): \
  Makefile.Metrics-%: \
    Makefile.RTSched-% \
    Makefile.CosTime-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.PortableGroup-)): \
  Makefile.PortableGroup-%: \
    Makefile.CosNaming-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTCORBAEvent-)): \
  Makefile.RTCORBAEvent-%: \
    Makefile.RTEvent-% \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTEvent-)): \
  Makefile.RTEvent-%: \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTEventLogAdmin-)): \
  Makefile.RTEventLogAdmin-%: \
    Makefile.RTEvent-% \
    Makefile.DsLogAdmin-% \
    Makefile.CosEvent-% \
    Makefile.CosNaming-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTKokyuEvent-)): \
  Makefile.RTKokyuEvent-%: \
    Makefile.RTEvent-% \
    Makefile.RTSched-% \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RT_Notification-)): \
  Makefile.RT_Notification-%: \
    Makefile.CosNotification-% \
    Makefile.Svc_Utils-% \
    Makefile.ETCL-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTOLDEvent-)): \
  Makefile.RTOLDEvent-%: \
    Makefile.RTEvent-% \
    Makefile.RTSched-% \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTSched-)): \
  Makefile.RTSched-%: \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.RTSchedEvent-)): \
  Makefile.RTSchedEvent-%: \
    Makefile.RTEvent-% \
    Makefile.RTSched-% \
    Makefile.CosNaming-% \
    Makefile.Svc_Utils-%
$(foreach ext, $(targets), $(addsuffix $(ext),Makefile.SSLIOP-)): \
  Makefile.SSLIOP-%: \
    Makefile.Security-%
