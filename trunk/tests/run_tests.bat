@echo off
rem $Id$

rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

setlocal

if "%1" == "purify" goto setuppurify
set dopure=
set arg=%1
goto endsetup

:setuppurify
set dopure=purify
set arg=%2

:endsetup

if not "%arg%" == "" goto runtest

:runall

call %0 %dopure% Atomic_Op_Test
call %0 %dopure% Barrier_Test
call %0 %dopure% Basic_Types_Test
call %0 %dopure% Buffer_Stream_Test
call %0 %dopure% CDR_Test
call %0 %dopure% Collection_Test
call %0 %dopure% Conn_Test
call %0 %dopure% DLL_Test
call %0 %dopure% DLList_Test
call %0 %dopure% Enum_Interfaces_Test
call %0 %dopure% Env_Value_Test
call %0 %dopure% Future_Test
call %0 %dopure% Handle_Set_Test
call %0 %dopure% Hash_Map_Manager_Test
call %0 %dopure% High_Res_Timer_Test
call %0 %dopure% IOStream_Test
call %0 %dopure% Map_Manager_Test
call %0 %dopure% Map_Test
call %0 %dopure% Mem_Map_Test
call %0 %dopure% Message_Block_Test
call %0 %dopure% Message_Queue_Notifications_Test
call %0 %dopure% Message_Queue_Test
call %0 %dopure% MT_Reactor_Timer_Test
call %0 %dopure% MM_Shared_Memory_Test
call %0 %dopure% MT_SOCK_Test
call %0 %dopure% Naming_Test
call %0 %dopure% New_Fail_Test
call %0 %dopure% Notify_Performance_Test
call %0 %dopure% OrdMultiSet_Test
call %0 %dopure% Pipe_Test
call %0 %dopure% Priority_Buffer_Test
call %0 %dopure% Dynamic_Priority_Test
call %0 %dopure% Priority_Reactor_Test
call %0 %dopure% Priority_Task_Test
call %0 %dopure% Process_Mutex_Test
call %0 %dopure% Process_Strategy_Test
call %0 %dopure% RB_Tree_Test
call %0 %dopure% Reactors_Test
call %0 %dopure% Reactor_Exceptions_Test
call %0 %dopure% Reactor_Notify_Test
call %0 %dopure% Reactor_Performance_Test
call %0 %dopure% Reactor_Timer_Test
call %0 %dopure% Reader_Writer_Test
call %0 %dopure% Thread_Pool_Reactor_Test
call %0 %dopure% Recursive_Mutex_Test
call %0 %dopure% Reverse_Lock_Test
call %0 %dopure% Semaphore_Test
call %0 %dopure% Service_Config_Test
call %0 %dopure% Sigset_Ops_Test
call %0 %dopure% Simple_Message_Block_Test
call %0 %dopure% SOCK_Test
call %0 %dopure% SOCK_Connector_Test
call %0 %dopure% SPIPE_Test
call %0 %dopure% SString_Test
call %0 %dopure% SV_Shared_Memory_Test
call %0 %dopure% Task_Test
call %0 %dopure% Thread_Manager_Test
call %0 %dopure% Thread_Mutex_Test
call %0 %dopure% Thread_Pool_Test
call %0 %dopure% Timer_Queue_Test
call %0 %dopure% Timeprobe_Test
if exist ..\netsvcs\servers\main.exe call %0 %dopure% Time_Service_Test
call %0 %dopure% Time_Value_Test
call %0 %dopure% Tokens_Test
call %0 %dopure% TSS_Test
call %0 %dopure% UPIPE_SAP_Test
call %0 %dopure% Upgradable_RW_Test

goto done

:runtest

if not "%dopure%"=="purify" goto justrun

if not exist purify_results mkdir purify_results

echo Purifying %arg%
purify /run /save-data=purify_results\%arg%.pfy /save-text-data=purify_results\%arg%.txt /AllocCallStackLength=20 /ErrorCallStackLength=20 /HandlesInUseAtExit /InUseAtExit /LeaksAtExit %arg%.exe

goto done

:justrun

echo Running %arg%
if not exist %arg%.exe goto nofile

%arg%.exe
if errorlevel 0 goto fine
echo.
echo %arg% has FAILED!!!
echo.
type %temp%\log\%arg%.log | find /I "assertion failed"
type %temp%\log\%arg%.log | find /I "not supported"
type %temp%\log\%arg%.log | find /I "no such file or directory"
type %temp%\log\%arg%.log | find /I "invalid argument"
type %temp%\log\%arg%.log | find /I "timeout"
type %temp%\log\%arg%.log | find /I "bad file number"
type %temp%\log\%arg%.log | find /I "Win32 structured exception"
echo.

goto done

:nofile
echo %arg%.exe not found
goto done

:fine

rem We should check the log files here to make sure the test ended correctly
rem type %temp%\log\%arg%.log | find "Ending"

:done

endlocal
