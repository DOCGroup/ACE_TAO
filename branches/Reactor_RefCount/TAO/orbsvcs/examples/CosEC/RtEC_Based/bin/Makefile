#
# $Id$
#

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = RtEC_Based_CosEC.cpp

ES_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lTAO_RTEC_COSEC -lTAO_CosEvent -lTAO_RTEvent -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_Messaging -lTAO_IORTable -lTAO_PortableServer -lTAO

BIN2 = RtEC_Based_CosEC

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  ifeq (RTEvent,$(findstring RTEvent,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # RTEvent
endif # CosEvent

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

LDFLAGS += -L../lib -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I../lib -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/RtEC_Based_CosEC.o .obj/RtEC_Based_CosEC.so .shobj/RtEC_Based_CosEC.o .shobj/RtEC_Based_CosEC.so: RtEC_Based_CosEC.cpp RtEC_Based_CosEC.h \
  ../lib/CosEvent_Utilities.h \
  $(ACE_ROOT)/ace/pre.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
  $(TAO_ROOT)/tao/corba.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(TAO_ROOT)/tao/corbafwd.h \
  $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
  $(ACE_ROOT)/ace/Functor.h \
  $(ACE_ROOT)/ace/ACE.h \
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
  $(ACE_ROOT)/ace/Functor.i \
  $(ACE_ROOT)/ace/Functor_T.h \
  $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
  $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i \
  $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i \
  $(TAO_ROOT)/tao/Any_T.h \
  $(TAO_ROOT)/tao/Any.h \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Stream.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/CDR.h \
  $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/Object.i \
  $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/AbstractBase.h \
  $(TAO_ROOT)/tao/AbstractBase.inl \
  $(TAO_ROOT)/tao/Any_T.inl \
  $(TAO_ROOT)/tao/Any_T.cpp \
  $(TAO_ROOT)/tao/Marshal.h \
  $(TAO_ROOT)/tao/Marshal.i \
  $(TAO_ROOT)/tao/debug.h \
  $(TAO_ROOT)/tao/NVList.h \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i \
  $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/ServicesC.h \
  $(TAO_ROOT)/tao/ServicesC.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/ObjectIdListC.h \
  $(TAO_ROOT)/tao/ObjectIdListC.i \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i \
  $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.h \
  $(TAO_ROOT)/tao/DynamicC.h \
  $(TAO_ROOT)/tao/DynamicC.i \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.h \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.i \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.h \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.i \
  $(TAO_ROOT)/tao/IOPC.h \
  $(TAO_ROOT)/tao/IOPC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS.h \
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
  $(TAO_ROOT)/tao/TimeBaseS_T.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.i \
  $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.i \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/event_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEvent/event_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
  ../lib/rtec_cosec_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Default_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Defaults.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Default_Factory.i \
  $(ACE_ROOT)/ace/Get_Opt.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Get_Opt.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
