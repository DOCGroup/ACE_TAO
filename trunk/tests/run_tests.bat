@echo off
rem $Id$

rem    To run this script, the first argument must be either bor
rem    or msc.  This will determine if the borland tests are run, or
rem    msvc tests are run.
rem    You can provide an optional second parameter, the name of
rem    the test to be run.
rem    An optional first argument of "purify" can be provided as well.

setlocal

set arg=
set deco=
set exedir=
set dopure=

set run_cmd=%0

if "%1" == "help" goto usage
if "%1" == "?" goto usage
if "%1" == "/?" goto usage
if "%1" == "purify" goto setuppurify
if "%1" == "bor" goto setupbor_arg
if "%1" == "msc" goto setupmsc_arg
goto setupmsc

:setuppurify
shift
set dopure=purify
if "%1" == "bor" goto setupbor_arg
if "%1" == "msc" goto setupmsc_arg
goto setupmsc

:setupbor_arg
shift
:setupbor
set arg=%1
set exedir=.\bor\bin\
rem Set deco to whatever decoration you have for the executables
set deco=
set platform=bor
goto endsetup

:setupmsc_arg
shift
:setupmsc
set arg=%1
set exedir=
set deco=
set platform=
goto endsetup

:usage
echo "usage: run_tests [purify] bor|msc [program-name]"
goto done

:endsetup

if not "%arg%" == "" goto runtest

:runall

call %run_cmd% %dopure% %platform% Basic_Types_Test
call %run_cmd% %dopure% %platform% Env_Value_Test
call %run_cmd% %dopure% %platform% Atomic_Op_Test
call %run_cmd% %dopure% %platform% CDR_Test
call %run_cmd% %dopure% %platform% Semaphore_Test
call %run_cmd% %dopure% %platform% TSS_Test
call %run_cmd% %dopure% %platform% Timeprobe_Test
call %run_cmd% %dopure% %platform% Time_Value_Test
call %run_cmd% %dopure% %platform% High_Res_Timer_Test
call %run_cmd% %dopure% %platform% SString_Test
call %run_cmd% %dopure% %platform% Collection_Test
call %run_cmd% %dopure% %platform% DLL_Test
call %run_cmd% %dopure% %platform% Naming_Test
call %run_cmd% %dopure% %platform% Handle_Set_Test
call %run_cmd% %dopure% %platform% OrdMultiSet_Test
call %run_cmd% %dopure% %platform% Mem_Map_Test
call %run_cmd% %dopure% %platform% SV_Shared_Memory_Test
call %run_cmd% %dopure% %platform% MM_Shared_Memory_Test
call %run_cmd% %dopure% %platform% Sigset_Ops_Test
call %run_cmd% %dopure% %platform% Timer_Queue_Test
call %run_cmd% %dopure% %platform% MT_Reactor_Timer_Test
call %run_cmd% %dopure% %platform% SOCK_Connector_Test
call %run_cmd% %dopure% %platform% SOCK_Send_Recv_Test
call %run_cmd% %dopure% %platform% Task_Test
call %run_cmd% %dopure% %platform% Thread_Manager_Test
call %run_cmd% %dopure% %platform% Thread_Pool_Test
call %run_cmd% %dopure% %platform% Future_Test
call %run_cmd% %dopure% %platform% RB_Tree_Test
call %run_cmd% %dopure% %platform% Reactors_Test
call %run_cmd% %dopure% %platform% Reactor_Exceptions_Test
call %run_cmd% %dopure% %platform% Reactor_Notify_Test
call %run_cmd% %dopure% %platform% Reactor_Timer_Test
call %run_cmd% %dopure% %platform% Thread_Pool_Reactor_Test
call %run_cmd% %dopure% %platform% Reactor_Performance_Test
call %run_cmd% %dopure% %platform% Notify_Performance_Test
call %run_cmd% %dopure% %platform% Reader_Writer_Test
call %run_cmd% %dopure% %platform% Priority_Reactor_Test
call %run_cmd% %dopure% %platform% SOCK_Test
call %run_cmd% %dopure% %platform% MT_SOCK_Test
call %run_cmd% %dopure% %platform% SPIPE_Test
call %run_cmd% %dopure% %platform% UPIPE_SAP_Test
call %run_cmd% %dopure% %platform% Barrier_Test
call %run_cmd% %dopure% %platform% Svc_Handler_Test
call %run_cmd% %dopure% %platform% Buffer_Stream_Test
call %run_cmd% %dopure% %platform% Priority_Buffer_Test
call %run_cmd% %dopure% %platform% Dynamic_Priority_Test
call %run_cmd% %dopure% %platform% Recursive_Mutex_Test
call %run_cmd% %dopure% %platform% Reverse_Lock_Test
if exist ..\netsvcs\servers\main.exe call %run_cmd% %dopure% %platform% Time_Service_Test
call %run_cmd% %dopure% %platform% Tokens_Test
call %run_cmd% %dopure% %platform% Map_Test
call %run_cmd% %dopure% %platform% Message_Queue_Notifications_Test
call %run_cmd% %dopure% %platform% Message_Queue_Test
call %run_cmd% %dopure% %platform% Simple_Message_Block_Test
call %run_cmd% %dopure% %platform% Message_Block_Test
call %run_cmd% %dopure% %platform% Pipe_Test
call %run_cmd% %dopure% %platform% Process_Mutex_Test
call %run_cmd% %dopure% %platform% Thread_Mutex_Test
call %run_cmd% %dopure% %platform% Process_Strategy_Test
call %run_cmd% %dopure% %platform% Service_Config_Test
call %run_cmd% %dopure% %platform% Priority_Task_Test
call %run_cmd% %dopure% %platform% IOStream_Test
call %run_cmd% %dopure% %platform% Enum_Interfaces_Test
call %run_cmd% %dopure% %platform% Upgradable_RW_Test
call %run_cmd% %dopure% %platform% Conn_Test
call %run_cmd% %dopure% %platform% New_Fail_Test
call %run_cmd% %dopure% %platform% Hash_Map_Manager_Test
call %run_cmd% %dopure% %platform% DLList_Test



goto done

:runtest

if not "%dopure%"=="purify" goto justrun

if not exist purify_results mkdir purify_results

echo Purifying %arg%
purify /run /save-data=purify_results\%arg%.pfy /save-text-data=purify_results\%arg%.txt /AllocCallStackLength=20 /ErrorCallStackLength=20 /HandlesInUseAtExit /InUseAtExit /LeaksAtExit %arg%.exe

goto done

:justrun

set exefile=%exedir%%arg%%deco%
echo Running %exefile%
if not exist %exefile%.exe goto nofile

%exefile%.exe
if errorlevel 0 goto fine
echo.
echo %exefile% has FAILED!!!
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
echo %exefile%.exe not found
goto done

:fine

rem We should check the log files here to make sure the test ended correctly
rem type %temp%\log\%arg%.log | find "Ending"

:done

endlocal
