# $Id$

SRC = $(BIN:%=%$(VAR).cpp)
LDLIBS= -lTAO_RTOLDEvent -lTAO_RTSchedEvent -lTAO_RTSched -lTAO_RTEvent -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO

ifndef TAO_ROOT
 TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

BIN2 = EC_Multiple

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Event

PSRC=EC_Multiple.cpp

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
  # This test causes g++ 2.7.2.3 (the compiler) to core dump.
  all clean realclean:
	@echo orbsvcs/tests/EC_Multiple is not built with older versions of g++
else # ! ACE_HAS_GNUG_PRE_2_8
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # ! ACE_HAS_GNUG_PRE_2_8

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(LYNXTARGET),ppc)
#### EC_Multiple.cpp raises internal compiler error with LynxOS 3.0.0 g++,
#### but only when -O2 optimization is enabled.  Disable it . . .
.obj/EC_Multiple.o .obj/EC_Multiple.so .shobj/EC_Multiple.o .shobj/EC_Multiple.so: EC_Multiple.cpp
	@echo NOTE: compiling EC_Multiple.cpp with -O instead of -O2, see Makefile.
	$(subst $(OCFLAGS) ,-O ,$(COMPILE.cc)) -o $@ $<
endif # LYNXTARGET == ppc

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/EC_Multiple.o .obj/EC_Multiple.so .shobj/EC_Multiple.o .shobj/EC_Multiple.so: EC_Multiple.cpp EC_Multiple.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Base.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread_Adapter.h \
 $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Malloc_Allocator.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/varbase.h \
 $(TAO_ROOT)/tao/TAO_Export.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
 $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/NVList.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(TAO_ROOT)/tao/LocalObject.h \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/OctetSeqC.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Services.h \
 $(TAO_ROOT)/tao/CORBA_String.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/Encodable.h \
 $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/BoundsC.h \
 $(TAO_ROOT)/tao/ValueBase.h \
 $(TAO_ROOT)/tao/ValueFactory.h \
 $(TAO_ROOT)/tao/DomainC.h \
 $(TAO_ROOT)/tao/WrongTransactionC.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/ObjectIDList.h \
 $(TAO_ROOT)/tao/PortableInterceptor.h \
 $(TAO_ROOT)/tao/PortableInterceptorC.h \
 $(TAO_ROOT)/tao/DynamicC.h \
 $(TAO_ROOT)/tao/MessagingC.h \
 $(TAO_ROOT)/tao/TimeBaseC.h \
 $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
 $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
 $(TAO_ROOT)/tao/TAO_Singleton.h \
 $(TAO_ROOT)/tao/TAO_Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
 $(TAO_ROOT)/tao/TAO_Singleton.inl \
 $(TAO_ROOT)/tao/IOPC.h \
 $(TAO_ROOT)/tao/PollableC.h \
 $(TAO_ROOT)/tao/Interceptor_List.h \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(TAO_ROOT)/tao/StringSeqC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/event_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
 $(TAO_ROOT)/tao/TimeBaseS.h \
 $(TAO_ROOT)/tao/TimeBaseS_T.h \
 $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
 $(TAO_ROOT)/tao/TimeBaseS_T.i \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Gateway_Sched.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Gateway.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Test_and_Set.h \
 $(ACE_ROOT)/ace/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/sched_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/sched_event_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
 Scheduler_Runtime1.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
 Scheduler_Runtime2.h Scheduler_Runtime_Dynamic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Runtime_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Process_Mutex.h \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/old_event_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/ReactorTask.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Fast_Reactor.h \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Select_Reactor_T.h \
 $(ACE_ROOT)/ace/Select_Reactor_Base.h \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Select_Reactor_T.cpp \
 $(ACE_ROOT)/ace/Timer_Heap.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.cpp \
 $(ACE_ROOT)/ace/Select_Reactor_T.i \
 $(ACE_ROOT)/ace/Timer_List.h \
 $(ACE_ROOT)/ace/Timer_List_T.h \
 $(ACE_ROOT)/ace/Timer_List_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.h \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/Protocol_Factory.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/Adapter.h \
 $(TAO_ROOT)/tao/PolicyFactory_Registry.h \
 $(TAO_ROOT)/tao/Parser_Registry.h \
 $(TAO_ROOT)/tao/Service_Callbacks.h \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.h \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.h \
 $(TAO_ROOT)/tao/Object_Ref_Table.h \
 $(TAO_ROOT)/tao/RT_Policy_i.h \
 $(TAO_ROOT)/tao/Protocols_Hooks.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Sched_Params.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
