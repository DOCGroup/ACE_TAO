#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the ACE ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   Aio_Platform_Test \
        ARGV_Test \
        Atomic_Op_Test \
        Auto_IncDec_Test \
        Barrier_Test \
        Basic_Types_Test \
        Buffer_Stream_Test \
        Cache_Map_Manager_Test \
        Cached_Accept_Conn_Test \
        Cached_Conn_Test \
        Capabilities_Test \
        CDR_File_Test \
        CDR_Test \
        Collection_Test \
	Config_Test \
        Conn_Test \
	Dirent_Test \
        DLList_Test \
        DLL_Test\
        Enum_Interfaces_Test \
        Env_Value_Test \
        Future_Test \
        Future_Set_Test \
        Handle_Set_Test \
        High_Res_Timer_Test \
        OrdMultiSet_Test \
        Hash_Map_Manager_Test \
        Hash_Map_Bucket_Iterator_Test \
        IOStream_Test \
        Lazy_Map_Manager_Test \
        Log_Msg_Test \
	Logging_Strategy_Test \
        Malloc_Test \
        Map_Test \
        Map_Manager_Test \
        Max_Default_Port_Test \
	MEM_Stream_Test \
        Mem_Map_Test \
        MM_Shared_Memory_Test \
        MT_Reactor_Timer_Test \
        MT_SOCK_Test \
        Message_Block_Test \
        Message_Queue_Test \
        Message_Queue_Test_Ex \
        Message_Queue_Notifications_Test \
        Notify_Performance_Test \
        Object_Manager_Test \
        OS_Test \
        Process_Mutex_Test \
        Process_Strategy_Test \
        Priority_Buffer_Test \
        Dynamic_Priority_Test \
        Priority_Task_Test \
        Priority_Reactor_Test \
        Process_Manager_Test \
        Pipe_Test \
        RB_Tree_Test \
        Reactors_Test \
        Reactor_Exceptions_Test \
        Reactor_Notify_Test \
        Reactor_Performance_Test \
        Reactor_Timer_Test \
        Reader_Writer_Test \
        Recursive_Mutex_Test \
	Refcounted_Auto_Ptr_Test \
        Reverse_Lock_Test \
        Semaphore_Test \
        Service_Config_Test \
        Signal_Test \
        Sigset_Ops_Test \
        Simple_Message_Block_Test \
        SOCK_Test \
        SOCK_Connector_Test \
        SOCK_Send_Recv_Test \
        SPIPE_Test \
        SString_Test \
        SV_Shared_Memory_Test \
        Svc_Handler_Test \
        Task_Test \
        Thread_Manager_Test \
        Thread_Mutex_Test \
        Thread_Pool_Test \
        Timeprobe_Test \
        Time_Service_Test \
        Time_Value_Test \
        Timer_Queue_Test \
        TSS_Test \
        UPIPE_SAP_Test \
        Upgradable_RW_Test

BIN2 =  Naming_Test \
        Thread_Pool_Reactor_Test \
        Thread_Pool_Reactor_Resume_Test \
        XtReactor_Test \
        FlReactor_Test \
        TkReactor_Test

#### If the ACE library wasn't built with all components, don't
#### try to build certain tests.
ACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
  BIN += $(BIN2)
endif # ! Other
ifeq (Token,$(findstring Token,$(ACE_BUILD_COMPONENTS)))
  BIN += Tokens_Test
else
  #### Ensure that Tokens_Test gets cleaned up, even if the ace
  #### directory was realcleaned first (and ACE_BUILD_COMPONENTS
  #### is empty).
  BIN2 += Tokens_Test
endif # ! Token

PSRC=$(addsuffix .cpp,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
  ifeq ($(LYNXTARGET),)
    BIN += CDR_Array_Test
    BUILD += CDR_Array_Test
  else
    #### LynxOS:  but only with 3.1.0 or later.  (Pre-3.0.0 has
    #### ACE_GNUG_PRE_2_8 == 1.)
    ifneq ($(VERSION),3.0.0)
      BIN += CDR_Array_Test
      BUILD += CDR_Array_Test
    endif # > 3.0.0
  endif
endif

ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
  ifneq ($(rmcast),0)
    DIRS += RMCast
  endif # rmcast
endif # ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifdef purify
  #### SIGINT is used by Thread_Manager_Test.
  #### SIGHUP and SIGTERM are used by Signal_Test.
  PURELINK += -ignore-signals=SIGHUP,SIGINT,SIGTERM
endif # purify

ifndef SOEXT
  SOEXT=so
endif
ifdef shared_libs
  ifndef VXWORKS
    #### Build libDLL_Test and libService_Config_DLL libraries first,
    #### so that it doesn't see the template instantiations from all
    #### of the tests. 
    BUILD := libDLL_Test.$(SOEXT) libService_Config_DLL.$(SOEXT) $(BUILD)
  endif # ! VXWORKS
endif # shared_libs

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

libDLL_Test.$(SOEXT): DLL_Test_Impl.cpp DLL_Test_Impl.h
	@$(MAKE) -f Makefile.DLL_Test

Service_Config_Test : libService_Config_DLL.$(SOEXT)
libService_Config_DLL.$(SOEXT): Service_Config_DLL.cpp Service_Config_DLL.h
	@$(MAKE) -f Makefile.Service_Config_DLL

realclean: clean
	-$(RM) libDLL_Test.$(SOEXT) libService_Config_DLL.*
	-$(RM) log/compilations.log*


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Service_Config_DLL.o .obj/Service_Config_DLL.so .shobj/Service_Config_DLL.o .shobj/Service_Config_DLL.so: Service_Config_DLL.cpp Service_Config_DLL.h \
 $(ACE_ROOT)/ace/pre.h \
 Service_Config_DLL_Export.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
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
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread_Adapter.h \
 $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_Allocator.h \
 $(ACE_ROOT)/ace/Malloc_Allocator.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
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
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Process_Mutex.h \
 $(ACE_ROOT)/ace/Process_Mutex.inl \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
