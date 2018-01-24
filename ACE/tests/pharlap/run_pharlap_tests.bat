@echo off
rem    This file runs all the tests on PharLap ETS.
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

setlocal

set arg=%1

if not "%1" == "" goto runtest

:runall

rem call %0 ACE_Init_Test
call %0 Atomic_Op_Test
call %0 Barrier_Test
call %0 Basic_Types_Test
call %0 Buffer_Stream_Test
call %0 CDR_Test
call %0 Collection_Test
call %0 Conn_Test
call %0 Cached_Conn_Test
call %0 Cached_Accept_Conn_Test
call %0 DLL_Test
call %0 DLList_Test
call %0 Enum_Interfaces_Test
call %0 Env_Value_Test
call %0 Future_Test
call %0 Handle_Set_Test
call %0 Hash_Map_Manager_Test
call %0 Lazy_Map_Manager_Test
call %0 Hash_Map_Bucket_Iterator_Test
call %0 High_Res_Timer_Test
call %0 IOStream_Test
call %0 Map_Manager_Test
call %0 Cache_Map_Manager_Test
call %0 Map_Test
rem call %0 Mem_Map_Test            not supported
call %0 Message_Block_Test
call %0 Message_Queue_Notifications_Test
call %0 Message_Queue_Test
call %0 MT_Reactor_Timer_Test
rem call %0 MM_Shared_Memory_Test   not supported
call %0 MT_SOCK_Test
call %0 Naming_Test
call %0 New_Fail_Test
call %0 Notify_Performance_Test
call %0 Object_Manager_Test
call %0 OrdMultiSet_Test
call %0 Pipe_Test
call %0 Priority_Buffer_Test
call %0 Dynamic_Priority_Test
call %0 Priority_Reactor_Test
call %0 Priority_Task_Test
call %0 Process_Mutex_Test
call %0 Process_Strategy_Test
call %0 RB_Tree_Test
call %0 Reactors_Test
call %0 Reactor_Exceptions_Test
call %0 Reactor_Notify_Test
call %0 Reactor_Performance_Test
call %0 Reactor_Timer_Test
call %0 Reader_Writer_Test
call %0 Thread_Pool_Reactor_Test
call %0 Recursive_Mutex_Test
call %0 Reverse_Lock_Test
call %0 Semaphore_Test
call %0 Service_Config_Test
call %0 Sigset_Ops_Test
call %0 Simple_Message_Block_Test
call %0 Svc_Handler_Test
call %0 SOCK_Test
call %0 SOCK_Connector_Test
call %0 SOCK_Send_Recv_Test
rem call %0 SPIPE_Test             not supported
call %0 SString_Test
call %0 SV_Shared_Memory_Test
call %0 Task_Test
call %0 Thread_Manager_Test
call %0 Thread_Mutex_Test
call %0 Thread_Pool_Test
call %0 Timer_Queue_Test
call %0 Timeprobe_Test
if exist ..\netsvcs\servers\main.exe call %0 Time_Service_Test
call %0 Time_Value_Test
call %0 Tokens_Test
call %0 TSS_Test
call %0 UPIPE_SAP_Test
call %0 Upgradable_RW_Test
goto done

:runtest

echo Running %arg%
if not exist %arg%_ETS.exe goto nofile

RUNEMB -LOGHOST -NODIALOG %arg%_ETS.exe > log\%arg%.log
if errorlevel 0 goto fine
echo.
echo %arg% has FAILED!!!
echo.
type log\%arg%.log | find /I "Abnormal program termination"
type log/%arg%.log | find /I "target halted"
type log/%arg%.log | find /I "Fatal error"
type log\%arg%.log | find /I "assertion failed"
type log\%arg%.log | find /I "not supported"
type log\%arg%.log | find /I "no such file or directory"
type log\%arg%.log | find /I "invalid argument"
type log\%arg%.log | find /I "timeout"
type log\%arg%.log | find /I "bad file number"
type log\%arg%.log | find /I "Win32 structured exception"
echo.

goto done

:nofile
echo %arg%.exe not found
goto done

:fine

rem We should check the log files here to make sure the test ended correctly
rem type log\%arg%.log | find "Ending"

:done

endlocal
