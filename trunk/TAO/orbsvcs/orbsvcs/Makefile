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
        -I$(TAO_ROOT) \
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
        AV/AVStreams_i_T
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
	Event/EC_Dispatching_Task

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
        CosEvent/ProxyPushSupplier_i
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
        Property/CosPropertyService_i\
        Property/CosPropertyService_i_T
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
  IDL_SRCS += Ping ImplRepo
  TAO_ORBSVCS_SRCS +=
endif # ImplRepo

ifneq (,$(findstring Naming,$(TAO_ORBSVCS)))
  IDL_SRCS += CosNaming
  TAO_ORBSVCS_SRCS += \
        IOR_Multicast \
        Naming/Naming_Context \
        Naming/Hash_Naming_Context \
        Naming/Entries \
        Naming/Persistent_Naming_Context \
        Naming/Persistent_Entries \
        Naming/Persistent_Bindings_Map \
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
$(foreach ext, $(IDL_EXT), ImplRepo$(ext)): Ping.idl
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

.obj/EC_Factory.o .obj/EC_Factory.so .shobj/EC_Factory.o .shobj/EC_Factory.so: Event/EC_Factory.cpp Event/EC_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/IOR_LookupTable.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Context.h \
 $(TAO_ROOT)/tao/Context.i \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Key_Adapters.h \
 $(ACE_ROOT)/ace/Map.h \
 $(ACE_ROOT)/ace/Map.i \
 $(ACE_ROOT)/ace/Map_T.h \
 $(ACE_ROOT)/ace/Pair.h \
 $(ACE_ROOT)/ace/Pair.i \
 $(ACE_ROOT)/ace/Pair_T.h \
 $(ACE_ROOT)/ace/Pair_T.i \
 $(ACE_ROOT)/ace/Pair_T.cpp \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Active_Map_Manager.h \
 $(ACE_ROOT)/ace/Active_Map_Manager.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Map_T.i \
 $(ACE_ROOT)/ace/Map_T.cpp \
 $(TAO_ROOT)/tao/Key_Adapters.i \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POAManager.h \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/POAManager.i \
 $(TAO_ROOT)/tao/Object_Adapter.h \
 $(TAO_ROOT)/tao/Object_Adapter.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h \
 $(TAO_ROOT)/tao/ValueBase.h \
 $(TAO_ROOT)/tao/ValueBase.i \
 $(TAO_ROOT)/tao/ValueFactory.h \
 $(TAO_ROOT)/tao/ValueFactory.i \
 Event/EC_Factory.i
.obj/EC_QOS_Info.o .obj/EC_QOS_Info.so .shobj/EC_QOS_Info.o .shobj/EC_QOS_Info.so: Event/EC_QOS_Info.cpp Event/EC_QOS_Info.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Event/EC_QOS_Info.i
.obj/BCU.o .obj/BCU.so .shobj/BCU.o .shobj/BCU.so: Event/BCU.cpp \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Event/BCU.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h
.obj/IOR_Multicast.o .obj/IOR_Multicast.so .shobj/IOR_Multicast.o .shobj/IOR_Multicast.so: IOR_Multicast.cpp IOR_Multicast.h \
 $(TAO_ROOT)/tao/corba.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/IOR_LookupTable.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Context.h \
 $(TAO_ROOT)/tao/Context.i \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Key_Adapters.h \
 $(ACE_ROOT)/ace/Map.h \
 $(ACE_ROOT)/ace/Map.i \
 $(ACE_ROOT)/ace/Map_T.h \
 $(ACE_ROOT)/ace/Pair.h \
 $(ACE_ROOT)/ace/Pair.i \
 $(ACE_ROOT)/ace/Pair_T.h \
 $(ACE_ROOT)/ace/Pair_T.i \
 $(ACE_ROOT)/ace/Pair_T.cpp \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Active_Map_Manager.h \
 $(ACE_ROOT)/ace/Active_Map_Manager.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Map_T.i \
 $(ACE_ROOT)/ace/Map_T.cpp \
 $(TAO_ROOT)/tao/Key_Adapters.i \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POAManager.h \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/POAManager.i \
 $(TAO_ROOT)/tao/Object_Adapter.h \
 $(TAO_ROOT)/tao/Object_Adapter.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h \
 $(TAO_ROOT)/tao/ValueBase.h \
 $(TAO_ROOT)/tao/ValueBase.i \
 $(TAO_ROOT)/tao/ValueFactory.h \
 $(TAO_ROOT)/tao/ValueFactory.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
