#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = liborbsvcs
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

# Uncomment out the following line (or invoke make with
# TAO_LACKS_EVENT_CHANNEL_ANY=1)
# if you don't want support for anys in the Event Channel.
# TAO_LACKS_EVENT_CHANNEL_ANY = 1
ifneq ($(TAO_LACKS_EVENT_CHANNEL_ANY),)
  override TAO_LACKS_EVENT_CHANNEL_ANY = -DTAO_LACKS_EVENT_CHANNEL_ANY
endif

# On non-Windows environment, we should at least define
# the export_include IDL flag.
override TAO_IDLFLAGS += \
        -I$(TAO_ROOT) -Ge 1\
        -Wb,export_macro=TAO_ORBSVCS_Export \
        -Wb,export_include=orbsvcs_export.h \
        $(TAO_LACKS_EVENT_CHANNEL_ANY)

VPATH=.:Naming:Time:Log:Concurrency:Property:Trader:Sched:Event:CosEvent:AV:

LIBS    += -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### Build customization.
####
ifndef TAO_ORBSVCS
  #### Please see docs/configurations.html#orbsvcs for documentation of
  #### TAO_ORBSVCS.
  TAO_ORBSVCS = Naming \
                ImplRepo \
                Time \
                Log \
                Concurrency \
                Property \
                Trader \
                LifeCycle \
                Sched \
                Event \
                CosEvent \
                Event2 \
                AV
endif # TAO_ORBSVCS

####
#### TAO_ORBSVCS support.
####
ifeq (,$(findstring Naming,$(TAO_ORBSVCS)))
  #### Many examples and tests use Naming, so always build it.
  TAO_ORBSVCS += Naming
endif # Naming

ifneq (,$(findstring AV,$(TAO_ORBSVCS)))
  IDL_SRCS += AVStreams Null_MediaCtrl sfp
  TAO_ORBSVCS_SRCS += \
        AV/AVStreams_i \
        AV/Endpoint_Strategy \
        AV/Endpoint_Strategy_T\
	AV/Transport\
        AV/sfp \
	AV/MCast\
	AV/RTP\
	AV/Policy\
	AV/Nil
endif # AV

ifneq (,$(findstring Event2,$(TAO_ORBSVCS)))
  ifeq (,$(findstring Event,$(TAO_ORBSVCS)))
    IDL_SRCS += RtecEventComm RtecEventChannelAdmin RtecUDPAdmin
  endif # ! Event
  TAO_ORBSVCS_SRCS += \
        Event/EC_Event_Channel \
        Event/EC_ConsumerAdmin \
        Event/EC_SupplierAdmin \
        Event/EC_ProxyConsumer \
        Event/EC_ProxySupplier \
        Event/EC_SupplierFiltering \
        Event/EC_Supplier_Filter_Builder \
        Event/EC_Trivial_Supplier_Filter \
        Event/EC_Filter \
        Event/EC_Filter_Builder \
        Event/EC_Dispatching \
        Event/EC_Factory \
        Event/EC_QOS_Info \
        Event/EC_Null_Factory \
        Event/EC_Disjunction_Filter \
        Event/EC_Conjunction_Filter \
        Event/EC_Type_Filter \
        Event/EC_Basic_Filter_Builder \
        Event/EC_Basic_Factory \
        Event/EC_Default_Factory \
        Event/EC_ObserverStrategy \
        Event/EC_ProxyPushSupplier_Set \
        Event/EC_Per_Supplier_Filter \
        Event/EC_Timeout_Filter \
        Event/EC_Timeout_Generator \
        Event/EC_Reactive_Timeout_Generator \
	Event/EC_Priority_Dispatching \
	Event/EC_MT_Dispatching \
	Event/EC_Dispatching_Task \
	Event/EC_Sched_Filter \
	Event/EC_Sched_Filter_Builder \
	Event/EC_Scheduling_Strategy \
	Event/EC_Null_Scheduling \
	Event/EC_Priority_Scheduling

  #### TAO's Event requires its Sched Service.
  ifeq (,$(findstring Sched,$(TAO_ORBSVCS)))
    TAO_ORBSVCS += Sched
  endif # ! Sched
endif # Event2

ifneq (,$(findstring CosEvent,$(TAO_ORBSVCS)))
  IDL_SRCS += CosEventComm CosEventChannelAdmin
  TAO_ORBSVCS_SRCS += \
        CosEvent/ConsumerAdmin_i \
        CosEvent/SupplierAdmin_i \
        CosEvent/EventChannel_i \
        CosEvent/ProxyPushConsumer_i \
        CosEvent/ProxyPushSupplier_i \
	CosEvent_Utilities
endif # CosEvent

ifneq (,$(findstring Event,$(TAO_ORBSVCS)))
  IDL_SRCS += RtecEventComm RtecEventChannelAdmin RtecUDPAdmin
  TAO_ORBSVCS_SRCS += \
        Event_Utilities \
        Event/BCU \
        Event/Dispatching_Modules \
        Event/Event_Channel \
        Event/Event_Manip \
        Event/Local_ESTypes \
        Event/Memory_Pools \
        Event/RT_Task \
        Event/ReactorTask \
        Event/Timer_Module \
        Event/EC_Gateway \
        Event/EC_Gateway_UDP \
        Event/EC_UDP_Admin \
        Event/Module_Factory

  #### TAO's Event requires its Sched Service.
  ifeq (,$(findstring Sched,$(TAO_ORBSVCS)))
    TAO_ORBSVCS += Sched
  endif # ! Sched
endif # Event

ifneq (,$(findstring Sched,$(TAO_ORBSVCS)))
  IDL_SRCS += RtecScheduler
  TAO_ORBSVCS_SRCS += \
        Scheduler_Factory \
        Runtime_Scheduler \
        Scheduler_Utilities \
        Sched/Config_Scheduler \
        Sched/DynSched \
        Sched/SchedEntry \
        Sched/Scheduler \
        Sched/Scheduler_Generic \
        Sched/Strategy_Scheduler
endif # Sched

ifneq (,$(findstring LifeCycle,$(TAO_ORBSVCS)))
  IDL_SRCS += CosLifeCycle LifeCycleService
  TAO_ORBSVCS_SRCS +=

  ifeq (,$(findstring Trader,$(TAO_ORBSVCS)))
    #### LifeCycle needs Trader.
    TAO_ORBSVCS += Trader
  endif # ! Trader
endif # LifeCycle

ifneq (,$(findstring Trader,$(TAO_ORBSVCS)))
  IDL_SRCS += CosTrading CosTradingDynamic CosTradingRepos
  TAO_ORBSVCS_SRCS += \
        Trader/Constraint_Interpreter \
        Trader/Constraint_Nodes \
        Trader/Constraint_Visitors \
        Trader/Constraint_l \
        Trader/Constraint_y \
        Trader/Offer_Database \
        Trader/Offer_Iterators \
        Trader/Offer_Iterators_T \
        Trader/Service_Type_Repository \
        Trader/Trader \
        Trader/Trader_Interfaces \
        Trader/Trader_T \
        Trader/Trader_Utils

  ifeq (,$(findstring Naming,$(TAO_ORBSVCS)))
    #### The Trader needs IOR_Multicast, so add it in if the
    #### Naming Service isn't included.
    TAO_ORBSVCS_SRCS += IOR_Multicast
  endif # ! Naming
endif # Trader

ifneq (,$(findstring Property,$(TAO_ORBSVCS)))
  IDL_SRCS += CosPropertyService
  TAO_ORBSVCS_SRCS += \
        Property/CosPropertyService_i
endif # Property

ifneq (,$(findstring Concurrency,$(TAO_ORBSVCS)))
  IDL_SRCS += CosConcurrencyControl
  TAO_ORBSVCS_SRCS += \
        Concurrency/CC_Lock \
        Concurrency/CC_LockSet \
        Concurrency/CC_LockSetFactory \
        Concurrency/Concurrency_Utils
endif # Concurrency

ifneq (,$(findstring Log,$(TAO_ORBSVCS)))
  IDL_SRCS += Logger
  TAO_ORBSVCS_SRCS += \
        Log/Logger_i
endif # Log

ifneq (,$(findstring Time,$(TAO_ORBSVCS)))
  IDL_SRCS += TimeBase TimeService
  TAO_ORBSVCS_SRCS += \
        Time_Utilities \
        Time/TAO_Time_Service_Server \
        Time/TAO_Time_Service_Clerk \
        Time/TAO_UTO \
        Time/TAO_TIO \
        Time/Timer_Helper

  #### TAO's Time Service requires its ImplRepo Service.
  ifeq (,$(findstring ImplRepo,$(TAO_ORBSVCS)))
    #### Add ImplRepo
    TAO_ORBSVCS += ImplRepo
  endif # ! ImplRepo
endif # Time

ifneq (,$(findstring ImplRepo,$(TAO_ORBSVCS)))
  IDL_SRCS += ImplRepo
  TAO_ORBSVCS_SRCS += IR_Helper
endif # ImplRepo

ifneq (,$(findstring Naming,$(TAO_ORBSVCS)))
  IDL_SRCS += CosNaming
  TAO_ORBSVCS_SRCS += \
        IOR_Multicast \
        Naming/Naming_Context \
        Naming/Hash_Naming_Context \
        Naming/Persistent_Naming_Context \
	Naming/Transient_Naming_Context \
        Naming/Persistent_Entries \
        Naming/Entries \
        Naming/Persistent_Context_Index \
        Naming/Naming_Utils
endif # Naming

IDL_FILES = \
        $(addsuffix S, $(IDL_SRCS)) \
        $(addsuffix C, $(IDL_SRCS))
FILES = $(IDL_FILES) $(TAO_ORBSVCS_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))
BUILD += ORBSVCS_COMPONENTS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG) \
        $(foreach svc, $(TAO_ORBSVCS), -DTAO_ORBSVCS_HAS_$(svc))

#
# Extra dependencies not caught by make depend.
#
$(foreach ext, $(IDL_EXT), TimeService$(ext)): TimeBase.idl
$(foreach ext, $(IDL_EXT), RtecScheduler$(ext)): TimeBase.idl
$(foreach ext, $(IDL_EXT), RtecEventComm$(ext)): TimeBase.idl
$(foreach ext, $(IDL_EXT), RtecUDPAdmin(ext)): RtecEventComm.idl
$(foreach ext, $(IDL_EXT), RtecEventChannelAdmin$(ext)): TimeBase.idl
$(foreach ext, $(IDL_EXT), RtecEventChannelAdmin$(ext)): RtecScheduler.idl
$(foreach ext, $(IDL_EXT), RtecEventChannelAdmin$(ext)): RtecEventComm.idl
$(foreach ext, $(IDL_EXT), CosEventChannelAdmin$(ext)): CosEventComm.idl
$(foreach ext, $(IDL_EXT), LifeCycleService$(ext)): CosLifeCycle.idl
$(foreach ext, $(IDL_EXT), CosLifeCycle$(ext)): CosNaming.idl
$(foreach ext, $(IDL_EXT), AVStreams_Full$(ext)): AVStream.idl
$(foreach ext, $(IDL_EXT), AVStreams$(ext)): CosPropertyService.idl
#$(foreach ext, $(IDL_EXT), CosConcurrencyControl$(ext)): CosTransactions.idl

.PRECIOUS: $(foreach file, $(IDL_SRCS), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

idl_stubs: $(addsuffix .h, $(IDL_FILES))

realclean: clean
	-$(RM) $(foreach file, $(IDL_SRCS), $(foreach ext, $(IDL_EXT), $(file)$(ext)))
	@$(ACE_ROOT)/bin/ace_components --orbsvcs --remove

.PHONY: ORBSVCS_COMPONENTS
ORBSVCS_COMPONENTS:
	@$(ACE_ROOT)/bin/ace_components --orbsvcs --set " $(TAO_ORBSVCS) "

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

