# Directory: tests/
#  $Id$

MAK_LIST = Atomic_Op_Test \
        Barrier_Test \
        Basic_Types_Test \
        Buffer_Stream_Test \
        Conn_Test \
        Enum_Interfaces_Test \
        Env_Value_Test \
        Future_Test \
        Handle_Set_Test \
        Hash_Map_Manager_Test \
        IOStream_Test \
        Map_Manager_Test \
        Mem_Map_Test \
        Message_Block_Test \
        Message_Queue_Notifications_Test \
        Message_Queue_Test \
        MM_Shared_Memory_Test \
        MT_SOCK_Test \
        Naming_Test \
        Notify_Performance_Test \
        Pipe_Test \
        Priority_Buffer_Test \
        Priority_Reactor_Test \
        Priority_Task_Test \
        Process_Mutex_Test \
        Process_Strategy_Test \
        Reactors_Test \
        Reactor_Exceptions_Test \
        Reactor_Notify_Test \
        Reactor_Performance_Test \
        Reactor_Timer_Test \
        Reader_Writer_Test \
        Recursive_Mutex_Test \
        Semaphore_Test \
        Service_Config_Test \
        Sigset_Ops_Test \
        Simple_Message_Block_Test \
        SOCK_Connector_Test \
        SOCK_Test \
        SPIPE_Test \
        SString_Test \
        SV_Shared_Memory_Test \
        Task_Test \
        Thread_Manager_Test \
        Thread_Mutex_Test \
        Thread_Pool_Test \
        Timer_Queue_Test \
        Time_Service_Test \
        Time_Value_Test \
        Tokens_Test \
	TSS_Test \
	UPIPE_SAP_Test

!INCLUDE <$(ACE_ROOT)\include\makeinclude\VC50_DIR.MAK>
