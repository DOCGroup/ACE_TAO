@echo off
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

rem    Make sure to have a purify_results subdirectory

setlocal

rem    Add Purify to the path, if it isn't already

PATH=%PATH%;C:\Program Files\PureAtria\Purify

if not "%1" == "" goto runtest

call run_purify_tests Atomic_Op_Test
call run_purify_tests Barrier_Test
call run_purify_tests Buffer_Stream_Test
call run_purify_tests Conn_Test
call run_purify_tests Enum_Interfaces_Test
call run_purify_tests Future_Test
call run_purify_tests Handle_Set_Test
call run_purify_tests Hash_Map_Manager_Test
call run_purify_tests IOStream_Test
call run_purify_tests Map_Manager_Test
call run_purify_tests Mem_Map_Test
call run_purify_tests Message_Block_Test
call run_purify_tests Message_Queue_Notifications_Test
call run_purify_tests Message_Queue_Test
call run_purify_tests MM_Shared_Memory_Test
call run_purify_tests MT_SOCK_Test
call run_purify_tests Naming_Test
call run_purify_tests Notify_Performance_Test
call run_purify_tests Pipe_Test
call run_purify_tests Priority_Buffer_Test
call run_purify_tests Priority_Reactor_Test
call run_purify_tests Priority_Task_Test
call run_purify_tests Process_Mutex_Test
call run_purify_tests Process_Strategy_Test
call run_purify_tests Reactors_Test
call run_purify_tests Reactor_Exceptions_Test
call run_purify_tests Reactor_Notify_Test
call run_purify_tests Reactor_Performance_Test
call run_purify_tests Reactor_Timer_Test
call run_purify_tests Reader_Writer_Test
call run_purify_tests Recursive_Mutex_Test
call run_purify_tests Semaphore_Test
call run_purify_tests Service_Config_Test
call run_purify_tests Sigset_Ops_Test
call run_purify_tests Simple_Message_Block_Test
call run_purify_tests SOCK_Test
call run_purify_tests SPIPE_Test
call run_purify_tests SString_Test
call run_purify_tests SV_Shared_Memory_Test
call run_purify_tests Task_Test
call run_purify_tests Thread_Manager_Test
call run_purify_tests Thread_Mutex_Test
call run_purify_tests Thread_Pool_Test
call run_purify_tests Timer_Queue_Test
call run_purify_tests Time_Service_Test
call run_purify_tests Time_Value_Test
call run_purify_tests Tokens_Test
call run_purify_tests TSS_Test
call run_purify_tests UPIPE_SAP_Test

goto done

:runtest

echo Purifying %1
purify /run /save-data=purify_results\%1.pfy /save-text-data=purify_results\%1.txt /AllocCallStackLength=20 /ErrorCallStackLength=20 /HandlesInUseAtExit /InUseAtExit /LeaksAtExit %1.exe

:done

endlocal