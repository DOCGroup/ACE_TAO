#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = libTAO_SmartProxies
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO -lACE


CPP_SRCS += Smart_Proxies

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


.obj/Smart_Proxies.o .obj/Smart_Proxies.SO .shobj/Smart_Proxies.o .shobj/Smart_Proxies.SO: Smart_Proxies.cpp Smart_Proxies.h \
  $(ACE_ROOT)/ace/pre.h smartproxies_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/corbafwd.h \
  $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(TAO_ROOT)/tao/orbconf.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
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
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/SStringfwd.h \
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
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/CDR.h \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i \
  $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/IOP_IORC.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/IOP_IORC.i \
  $(TAO_ROOT)/tao/Object.i \
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
  Smart_Proxies.inl

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
