#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the ACE client-side Name_Server test
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

FILES   = Client_Test

PSRC    = $(addsuffix .cpp,$(BIN))
OBJ     = $(SRC:%.cpp=$(VDIR)%.o)

LSRC    = $(addsuffix .cpp,$(FILES))

# This is needed to add ACE to the libs for the shared library.
ACE_SHLIBS    += $(ACELIB)

#### If the ACE library wasn't built with sufficient components, don't
#### try to build here.
ACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
  BIN = main
  LIB = libClient_Test.a
  SHLIB = libClient_Test.$(SOEXT)
  BUILD = $(VSHLIB) $(VLIB) $(VBIN)
endif # Other

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ifneq ($(BIN),)
  ifeq ($(SHLIB), $(findstring $(SHLIB), $(BUILD)))
    $(BIN): $(SHLIB)
  endif
  ifeq ($(LIB), $(findstring $(LIB), $(BUILD)))
    $(BIN): $(LIB)
  endif
endif # BIN

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Client_Test.o .obj/Client_Test.so .shobj/Client_Test.o .shobj/Client_Test.so: Client_Test.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
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
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i \
  $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
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
  $(ACE_ROOT)/ace/Naming_Context.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Name_Proxy.h \
  $(ACE_ROOT)/ace/INET_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Connector.h \
  $(ACE_ROOT)/ace/SOCK_Stream.h \
  $(ACE_ROOT)/ace/SOCK_IO.h \
  $(ACE_ROOT)/ace/SOCK.h \
  $(ACE_ROOT)/ace/IPC_SAP.h \
  $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i \
  $(ACE_ROOT)/ace/SOCK_IO.i \
  $(ACE_ROOT)/ace/SOCK_Stream.i \
  $(ACE_ROOT)/ace/SOCK_Connector.i \
  $(ACE_ROOT)/ace/Synch_Options.h \
  $(ACE_ROOT)/ace/Synch_Options.i \
  $(ACE_ROOT)/ace/Name_Request_Reply.h \
  $(ACE_ROOT)/ace/Name_Space.h \
  $(ACE_ROOT)/ace/Dynamic_Service.h \
  $(ACE_ROOT)/ace/Dynamic_Service_Base.h \
  $(ACE_ROOT)/ace/Dynamic_Service.i \
  $(ACE_ROOT)/ace/Dynamic_Service.cpp \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
  $(ACE_ROOT)/ace/Singleton.h \
  $(ACE_ROOT)/ace/Singleton.i \
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
  $(ACE_ROOT)/ace/Thread_Manager.i Client_Test.h \
  $(ACE_ROOT)/ace/svc_export.h

.obj/main.o .obj/main.so .shobj/main.o .shobj/main.so: main.cpp $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
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
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i \
  $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
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
  $(ACE_ROOT)/ace/Naming_Context.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Name_Proxy.h \
  $(ACE_ROOT)/ace/INET_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Connector.h \
  $(ACE_ROOT)/ace/SOCK_Stream.h \
  $(ACE_ROOT)/ace/SOCK_IO.h \
  $(ACE_ROOT)/ace/SOCK.h \
  $(ACE_ROOT)/ace/IPC_SAP.h \
  $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i \
  $(ACE_ROOT)/ace/SOCK_IO.i \
  $(ACE_ROOT)/ace/SOCK_Stream.i \
  $(ACE_ROOT)/ace/SOCK_Connector.i \
  $(ACE_ROOT)/ace/Synch_Options.h \
  $(ACE_ROOT)/ace/Synch_Options.i \
  $(ACE_ROOT)/ace/Name_Request_Reply.h \
  $(ACE_ROOT)/ace/Name_Space.h \
  $(ACE_ROOT)/ace/ARGV.h $(ACE_ROOT)/ace/ARGV.i \
  Client_Test.h $(ACE_ROOT)/ace/svc_export.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
