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

#LDLIBS = -lTAO_CosTime -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO
LDLIBS = -lTAO_Metrics -lTAO_RTEvent -lTAO_RTSched -lTAO_RTCORBA -lTAO_PortableServer -lTAO

PROG_SRCS = \
	Metrics_Local_Cache_Test.cpp \
	Remote_Metrics_Logger.cpp
	
SRC = $(PROG_SRCS)

REMOTE_LOGGER_OBJS = Remote_Metrics_Logger.o
LOCAL_CACHE_OBJS = Metrics_Local_Cache_Test.o

BIN2 = Metrics_Local_Cache Remote_Logger

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Metrics,$(findstring Metrics,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Time

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------


#LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

#.PRECIOUS: TimeServiceC.cpp TimeServiceC.i TimeServiceC.h
#.PRECIOUS: TimeServiceS.cpp TimeServiceS.i TimeServiceS.h
#.PRECIOUS: TimeServiceS_T.cpp TimeServiceS_T.i TimeServiceS_T.h

Remote_Logger: $(addprefix $(VDIR),$(REMOTE_LOGGER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Metrics_Local_Cache: $(addprefix $(VDIR),$(LOCAL_CACHE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)
	
realclean: clean
	-$(RM) TimeServiceC.* TimeServiceS.* TimeServiceS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Metrics_Local_Cache_Test.o .obj/Metrics_Local_Cache_Test.o .obj/Metrics_Local_Cache_Test.o .obj/Metrics_Local_Cache_Test.o: Metrics_Local_Cache_Test.cpp \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/ace_wchar.h $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/ARGV.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/ARGV.i $(ACE_ROOT)/ace/Arg_Shifter.h \
  $(ACE_ROOT)/ace/Sched_Params.h \
  $(ACE_ROOT)/ace/Sched_Params.i $(TAO_ROOT)/tao/corba.h \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/orbconf.h $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/SString.h $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/AbstractBase.h \
  $(TAO_ROOT)/tao/AbstractBase.inl \
  $(TAO_ROOT)/tao/DomainC.h $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.h \
  $(TAO_ROOT)/tao/DynamicC.h \
  $(TAO_ROOT)/tao/DynamicC.i \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.h \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.i \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.h \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.i \
  $(TAO_ROOT)/tao/IOPC.h $(TAO_ROOT)/tao/IOPC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.i \
  $(TAO_ROOT)/tao/RTCORBA/Multi_Priority_Mapping.h \
  $(TAO_ROOT)/tao/RTCORBA/RTCORBA.h \
  $(TAO_ROOT)/tao/RTCORBA/rtcorba_export.h \
  $(TAO_ROOT)/tao/RTCORBA/RTCORBAC.h \
  $(TAO_ROOT)/tao/TimeBaseC.h \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
  $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
  $(TAO_ROOT)/tao/TAO_Singleton.h \
  $(TAO_ROOT)/tao/TAO_Singleton.inl \
  $(TAO_ROOT)/tao/TAO_Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.inl \
  $(TAO_ROOT)/tao/TimeBaseC.i \
  $(TAO_ROOT)/tao/RTCORBA/RTCORBAC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Logger.h \
  $(ACE_ROOT)/ace/Task.h $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
  $(ACE_ROOT)/ace/Containers.h $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Singleton.h $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
  $(ACE_ROOT)/ace/Thread_Manager.i $(ACE_ROOT)/ace/Task.i \
  $(ACE_ROOT)/ace/Task_T.h $(ACE_ROOT)/ace/Message_Queue.h \
  $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
  $(ACE_ROOT)/ace/Message_Queue_T.h \
  $(ACE_ROOT)/ace/Message_Queue_T.i \
  $(ACE_ROOT)/ace/Message_Queue_T.cpp \
  $(ACE_ROOT)/ace/Notification_Strategy.h \
  $(ACE_ROOT)/ace/Notification_Strategy.inl \
  $(ACE_ROOT)/ace/Message_Queue.i $(ACE_ROOT)/ace/Task_T.i \
  $(ACE_ROOT)/ace/Task_T.cpp $(ACE_ROOT)/ace/Module.h \
  $(ACE_ROOT)/ace/Module.i $(ACE_ROOT)/ace/Module.cpp \
  $(ACE_ROOT)/ace/Stream_Modules.h \
  $(ACE_ROOT)/ace/Stream_Modules.cpp \
  $(ACE_ROOT)/ace/Functor.h $(ACE_ROOT)/ace/Functor.i \
  $(ACE_ROOT)/ace/Functor_T.h $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(ACE_ROOT)/ace/Hash_Map_Manager.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Service_Types.h \
  $(ACE_ROOT)/ace/Service_Types.i \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.i \
  $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseS.i \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Utils.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Utils.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Rate_Group_Info.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Rate_Group_Info.i \
  Metrics_Local_Cache_Test.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_LocalCache.h \
  $(ACE_ROOT)/ace/Metrics_Cache.h \
  $(ACE_ROOT)/ace/Metrics_Cache.i \
  $(ACE_ROOT)/ace/Metrics_Cache_T.h \
  $(ACE_ROOT)/ace/Timeprobe.h $(ACE_ROOT)/ace/Timeprobe.i \
  $(ACE_ROOT)/ace/Timeprobe_T.h \
  $(ACE_ROOT)/ace/Timeprobe_T.cpp \
  $(ACE_ROOT)/ace/High_Res_Timer.h \
  $(ACE_ROOT)/ace/High_Res_Timer.i \
  $(ACE_ROOT)/ace/Metrics_Cache_T.i \
  $(ACE_ROOT)/ace/Metrics_Cache_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/sched_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_LocalCache.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_LocalCache_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_LocalCache_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_LocalCache_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_UpcallMonitor.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_FrameManager.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_FrameManager.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_FrameManager_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_FrameManager_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_FrameManager_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_UpcallMonitor.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_UpcallMonitor_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_UpcallMonitor_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_UpcallMonitor_T.i

.obj/Remote_Metrics_Logger.o .obj/Remote_Metrics_Logger.o .obj/Remote_Metrics_Logger.o .obj/Remote_Metrics_Logger.o: Remote_Metrics_Logger.cpp \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/ace_wchar.h $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Arg_Shifter.h \
  $(ACE_ROOT)/ace/ARGV.h $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/ARGV.i $(TAO_ROOT)/tao/corba.h \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/orbconf.h $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/SString.h $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/AbstractBase.h \
  $(TAO_ROOT)/tao/AbstractBase.inl \
  $(TAO_ROOT)/tao/DomainC.h $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.h \
  $(TAO_ROOT)/tao/DynamicC.h \
  $(TAO_ROOT)/tao/DynamicC.i \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.h \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.i \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.h \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.i \
  $(TAO_ROOT)/tao/IOPC.h $(TAO_ROOT)/tao/IOPC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.i \
  $(TAO_ROOT)/tao/TAO_Internal.h Remote_Metrics_Logger.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Logger.h \
  $(ACE_ROOT)/ace/Task.h $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
  $(ACE_ROOT)/ace/Containers.h $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Singleton.h $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
  $(ACE_ROOT)/ace/Thread_Manager.i $(ACE_ROOT)/ace/Task.i \
  $(ACE_ROOT)/ace/Task_T.h $(ACE_ROOT)/ace/Message_Queue.h \
  $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
  $(ACE_ROOT)/ace/Message_Queue_T.h \
  $(ACE_ROOT)/ace/Message_Queue_T.i \
  $(ACE_ROOT)/ace/Message_Queue_T.cpp \
  $(ACE_ROOT)/ace/Notification_Strategy.h \
  $(ACE_ROOT)/ace/Notification_Strategy.inl \
  $(ACE_ROOT)/ace/Message_Queue.i $(ACE_ROOT)/ace/Task_T.i \
  $(ACE_ROOT)/ace/Task_T.cpp $(ACE_ROOT)/ace/Module.h \
  $(ACE_ROOT)/ace/Module.i $(ACE_ROOT)/ace/Module.cpp \
  $(ACE_ROOT)/ace/Stream_Modules.h \
  $(ACE_ROOT)/ace/Stream_Modules.cpp \
  $(ACE_ROOT)/ace/Functor.h $(ACE_ROOT)/ace/Functor.i \
  $(ACE_ROOT)/ace/Functor_T.h $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(ACE_ROOT)/ace/Hash_Map_Manager.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Service_Types.h \
  $(ACE_ROOT)/ace/Service_Types.i \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/tao/TimeBaseC.h \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
  $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
  $(TAO_ROOT)/tao/TAO_Singleton.h \
  $(TAO_ROOT)/tao/TAO_Singleton.inl \
  $(TAO_ROOT)/tao/TAO_Singleton.cpp \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.inl \
  $(TAO_ROOT)/tao/TimeBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.i \
  $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseS.i \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/MetricsS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Utils.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Metrics/Metrics_Utils.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
