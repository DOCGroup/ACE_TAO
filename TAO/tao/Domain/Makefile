#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = libTAO_Domain
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO_IFR_Client -lTAO_PortableServer -lTAO -lACE


CPP_SRCS += DomainS

FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/DomainS.o .obj/DomainS.so .shobj/DomainS.o .shobj/DomainS.so: DomainS.cpp DomainS.h \
 $(ACE_ROOT)/ace/pre.h \
 domain_export.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(TAO_ROOT)/tao/DomainC.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Base.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
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
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/Encodable.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/String_Base.h \
 $(ACE_ROOT)/ace/String_Base_Const.h \
 $(ACE_ROOT)/ace/String_Base.cpp \
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/String_Base.i \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
 $(TAO_ROOT)/tao/PortableServer/PolicyS.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(ACE_ROOT)/ace/Atomic_Op.h \
 $(ACE_ROOT)/ace/Atomic_Op.cpp \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/PolicyS_T.h \
 $(TAO_ROOT)/tao/PortableServer/PolicyS_T.cpp \
 $(TAO_ROOT)/tao/PortableServer/PolicyS_T.i \
 DomainS_T.h DomainS_T.cpp DomainS_T.i \
 $(TAO_ROOT)/tao/PortableServer/Object_Adapter.h \
 $(TAO_ROOT)/tao/PortableServer/Key_Adapters.h \
 $(ACE_ROOT)/ace/Map.h \
 $(ACE_ROOT)/ace/Map_T.h \
 $(ACE_ROOT)/ace/Pair.h \
 $(ACE_ROOT)/ace/Pair_T.h \
 $(ACE_ROOT)/ace/Pair_T.cpp \
 $(ACE_ROOT)/ace/Pair_T.i \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
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
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Active_Map_Manager.h \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Map_T.cpp \
 $(ACE_ROOT)/ace/Map_T.i \
 $(TAO_ROOT)/tao/PortableServer/poa_macros.h \
 $(TAO_ROOT)/tao/PortableServer/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Adapter.h \
 $(TAO_ROOT)/tao/LocalObject.h \
 $(TAO_ROOT)/tao/PortableServer/Default_Policy_Validator.h \
 $(TAO_ROOT)/tao/PortableServer/Policy_Validator.h \
 $(TAO_ROOT)/tao/PortableServer/POA_Policy_Set.h \
 $(TAO_ROOT)/tao/PortableServer/POA_Policies.h \
 $(TAO_ROOT)/tao/Policy_Set.h \
 $(TAO_ROOT)/tao/PortableServer/Operation_Table.h \
 $(TAO_ROOT)/tao/TAO_Singleton.h \
 $(TAO_ROOT)/tao/TAO_Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
 $(TAO_ROOT)/tao/TAO_Singleton.inl \
 $(TAO_ROOT)/tao/TAO_Server_Request.h \
 $(TAO_ROOT)/tao/Tagged_Profile.h \
 $(TAO_ROOT)/tao/IOPC.h \
 $(TAO_ROOT)/tao/GIOPC.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/OctetSeqC.h \
 $(TAO_ROOT)/tao/Service_Context.h \
 $(TAO_ROOT)/tao/PICurrent.h \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(TAO_ROOT)/tao/PortableInterceptorC.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Services.h \
 $(TAO_ROOT)/tao/CORBA_String.h \
 $(TAO_ROOT)/tao/BoundsC.h \
 $(TAO_ROOT)/tao/ValueBase.h \
 $(TAO_ROOT)/tao/ValueFactory.h \
 $(TAO_ROOT)/tao/WrongTransactionC.h \
 $(TAO_ROOT)/tao/ObjectIDList.h \
 $(TAO_ROOT)/tao/StringSeqC.h \
 $(TAO_ROOT)/tao/DynamicC.h \
 $(TAO_ROOT)/tao/MessagingC.h \
 $(TAO_ROOT)/tao/TimeBaseC.h \
 $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
 $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
 $(TAO_ROOT)/tao/PollableC.h \
 $(TAO_ROOT)/tao/TAO_AMH_Response_Handler.h \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/Protocol_Factory.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/PolicyFactory_Registry.h \
 $(TAO_ROOT)/tao/Parser_Registry.h \
 $(TAO_ROOT)/tao/Service_Callbacks.h \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.h \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.h \
 $(TAO_ROOT)/tao/Object_Ref_Table.h \
 $(TAO_ROOT)/tao/Interceptor_List.h \
 $(TAO_ROOT)/tao/Protocols_Hooks.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/Profile.h \
 $(TAO_ROOT)/tao/Tagged_Components.h \
 $(TAO_ROOT)/tao/CONV_FRAMEC.h \
 $(TAO_ROOT)/tao/GIOP_Message_Version.h \
 $(TAO_ROOT)/tao/IFR_Client_Adapter.h \
 $(TAO_ROOT)/tao/PortableInterceptor.h \
 $(TAO_ROOT)/tao/RequestInfo_Util.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.h \
 $(TAO_ROOT)/tao/PortableServer/ServerInterceptorAdapter.h \
 $(TAO_ROOT)/tao/IFR_Client/IFR_ExtendedC.h \
 $(TAO_ROOT)/tao/IFR_Client/IFR_BasicC.h \
 $(TAO_ROOT)/tao/IFR_Client/IFR_BaseC.h \
 $(TAO_ROOT)/tao/IFR_Client/ifr_client_export.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Synch_Reply_Dispatcher.h \
 $(TAO_ROOT)/tao/Reply_Dispatcher.h \
 $(TAO_ROOT)/tao/LF_Event.h \
 $(TAO_ROOT)/tao/TAOC.h \
 $(TAO_ROOT)/tao/operation_details.h \
 $(TAO_ROOT)/tao/target_specification.h \
 $(TAO_ROOT)/tao/Invocation_Endpoint_Selectors.h \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service_Base.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(ACE_ROOT)/ace/Dynamic_Service.i \
 DomainS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
