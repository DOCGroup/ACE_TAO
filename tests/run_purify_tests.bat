@echo off
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

rem    Make sure to have a purify_results subdirectory

mkdir  purify_results

setlocal

rem    Add Purify to the path, if it isn't already

PATH=%PATH%;C:\Program Files\PureAtria\Purify

if not "%1" == "" goto runtest

call %0 Atomic_Op_Test
call %0 Barrier_Test
call %0 Buffer_Stream_Test
call %0 Conn_Test
call %0 Enum_Interfaces_Test
call %0 Future_Test
call %0 Handle_Set_Test
call %0 Hash_Map_Manager_Test
call %0 IOStream_Test
call %0 Map_Manager_Test
call %0 Mem_Map_Test
call %0 Message_Block_Test
call %0 Message_Queue_Notifications_Test
call %0 Message_Queue_Test
call %0 MM_Shared_Memory_Test
call %0 MT_SOCK_Test
call %0 Naming_Test
call %0 Notify_Performance_Test
call %0 Pipe_Test
call %0 Priority_Buffer_Test
call %0 Priority_Reactor_Test
call %0 Priority_Task_Test
call %0 Process_Mutex_Test
call %0 Process_Strategy_Test
call %0 Reactors_Test
call %0 Reactor_Exceptions_Test
call %0 Reactor_Notify_Test
call %0 Reactor_Performance_Test
call %0 Reactor_Timer_Test
call %0 Reader_Writer_Test
call %0 Recursive_Mutex_Test
call %0 Semaphore_Test
call %0 Service_Config_Test
call %0 Sigset_Ops_Test
call %0 Simple_Message_Block_Test
call %0 SOCK_Test
call %0 SOCK_Connector_Test
call %0 SPIPE_Test
call %0 SString_Test
call %0 SV_Shared_Memory_Test
call %0 Task_Test
call %0 Thread_Manager_Test
call %0 Thread_Mutex_Test
call %0 Thread_Pool_Test
call %0 Timer_Queue_Test
call %0 Time_Service_Test
call %0 Time_Value_Test
call %0 Tokens_Test
call %0 TSS_Test
call %0 UPIPE_SAP_Test

goto done

:runtest

echo Purifying %1
purify /run /save-data=purify_results\%1.pfy /save-text-data=purify_results\%1.txt /AllocCallStackLength=20 /ErrorCallStackLength=20 /HandlesInUseAtExit /InUseAtExit /LeaksAtExit %1.exe

:done

endlocal