@echo off
rem run_tests.bat,v 4.71 1999/11/08 05:39:06 nanbor Exp

rem
rem    Note this batch file will not work on Win9X, so we suggest that
rem    you install/use perl on Win9x and run the run_test.pl file.
rem

rem    To run this script, the first argument must be either bor
rem    or msc.  This will determine if the borland tests are run, or
rem    msvc tests are run.
rem    You can provide an optional second parameter, the name of
rem    the test to be run.
rem    An optional first argument of "purify" can be provided as well.
rem    If you specify that the borland tests are to run you must also
rem    specify the build configuration to be tested. There are three
rem    options that may be set: DEBUG, STATIC, and PASCAL. Each is Off
rem    by default. These options may be switched on by using environment
rem    variables, for example: set DEBUG=1 and turned off again:
rem    set DEBUG=.
rem      * DEBUG - if defined means run tests built with debug info
rem      * STATIC - if defined means run statically linked tests
rem      * PASCAL - if defined means run the VCL-compatible runtime
rem                 library tests

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

set STATIC_DIR=Dynamic
if not "%STATIC%" == "" set STATIC_DIR=Static
set DEBUG_DIR=Release
if not "%DEBUG%" == "" set DEBUG_DIR=Debug
set PASCAL_DIR=.
if not "%PASCAL%" == "" set PASCAL_DIR=Pascal
if "%STATIC_DIR%" == "Static" goto continue_setupbor
set corebindir=%ACE_ROOT%\bin\%STATIC_DIR%\%DEBUG_DIR%\%PASCAL_DIR%
set PATH=%PATH%;%corebindir%

:continue_setupbor
set exedir=.\%STATIC_DIR%\%DEBUG_DIR%\%PASCAL_DIR%\

set PATH=%PATH%;%exedir%
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

if not "%platform%" == "bor" call %run_cmd% %dopure% %platform% ACE_Init_Test
call %run_cmd% %dopure% %platform% Atomic_Op_Test
call %run_cmd% %dopure% %platform% Auto_IncDec_Test
call %run_cmd% %dopure% %platform% Barrier_Test
call %run_cmd% %dopure% %platform% Basic_Types_Test
call %run_cmd% %dopure% %platform% Buffer_Stream_Test
call %run_cmd% %dopure% %platform% Cache_Map_Manager_Test
call %run_cmd% %dopure% %platform% Cached_Conn_Test
call %run_cmd% %dopure% %platform% Cached_Accept_Conn_Test
call %run_cmd% %dopure% %platform% Capabilities_Test
call %run_cmd% %dopure% %platform% CDR_Test
call %run_cmd% %dopure% %platform% CDR_File_Test
call %run_cmd% %dopure% %platform% CDR_Array_Test
call %run_cmd% %dopure% %platform% Collection_Test
call %run_cmd% %dopure% %platform% Conn_Test
call %run_cmd% %dopure% %platform% Dirent_Test
call %run_cmd% %dopure% %platform% DLL_Test
call %run_cmd% %dopure% %platform% DLList_Test
call %run_cmd% %dopure% %platform% Dynamic_Priority_Test
call %run_cmd% %dopure% %platform% Enum_Interfaces_Test
call %run_cmd% %dopure% %platform% Env_Value_Test
call %run_cmd% %dopure% %platform% Future_Test
call %run_cmd% %dopure% %platform% Future_Set_Test
call %run_cmd% %dopure% %platform% Handle_Set_Test
call %run_cmd% %dopure% %platform% Hash_Map_Manager_Test
call %run_cmd% %dopure% %platform% Hash_Map_Bucket_Iterator_Test
call %run_cmd% %dopure% %platform% High_Res_Timer_Test
call %run_cmd% %dopure% %platform% IOStream_Test
call %run_cmd% %dopure% %platform% Lazy_Map_Manager_Test
call %run_cmd% %dopure% %platform% Map_Manager_Test
call %run_cmd% %dopure% %platform% Map_Test
call %run_cmd% %dopure% %platform% Max_Default_Port_Test
call %run_cmd% %dopure% %platform% Mem_Map_Test
call %run_cmd% %dopure% %platform% MEM_Stream_Test
call %run_cmd% %dopure% %platform% Message_Block_Test
call %run_cmd% %dopure% %platform% Message_Queue_Notifications_Test
call %run_cmd% %dopure% %platform% Message_Queue_Test
call %run_cmd% %dopure% %platform% MM_Shared_Memory_Test
call %run_cmd% %dopure% %platform% MT_Reactor_Timer_Test
call %run_cmd% %dopure% %platform% MT_SOCK_Test
call %run_cmd% %dopure% %platform% Malloc_Test
call %run_cmd% %dopure% %platform% Naming_Test
rem call %run_cmd% %dopure% %platform% New_Fail_Test
call %run_cmd% %dopure% %platform% Notify_Performance_Test
call %run_cmd% %dopure% %platform% Object_Manager_Test
call %run_cmd% %dopure% %platform% OrdMultiSet_Test
call %run_cmd% %dopure% %platform% Pipe_Test
call %run_cmd% %dopure% %platform% Priority_Buffer_Test
call %run_cmd% %dopure% %platform% Priority_Reactor_Test
call %run_cmd% %dopure% %platform% Priority_Task_Test
call %run_cmd% %dopure% %platform% Process_Manager_Test
call %run_cmd% %dopure% %platform% Process_Mutex_Test
call %run_cmd% %dopure% %platform% Process_Strategy_Test
call %run_cmd% %dopure% %platform% RB_Tree_Test
call %run_cmd% %dopure% %platform% Reactors_Test
call %run_cmd% %dopure% %platform% Reactor_Exceptions_Test
call %run_cmd% %dopure% %platform% Reactor_Notify_Test
call %run_cmd% %dopure% %platform% Reactor_Performance_Test
call %run_cmd% %dopure% %platform% Reactor_Timer_Test
call %run_cmd% %dopure% %platform% Reader_Writer_Test
call %run_cmd% %dopure% %platform% Recursive_Mutex_Test
call %run_cmd% %dopure% %platform% Reverse_Lock_Test
call %run_cmd% %dopure% %platform% Semaphore_Test
call %run_cmd% %dopure% %platform% Service_Config_Test
call %run_cmd% %dopure% %platform% Sigset_Ops_Test
call %run_cmd% %dopure% %platform% Simple_Message_Block_Test
call %run_cmd% %dopure% %platform% Svc_Handler_Test
call %run_cmd% %dopure% %platform% SOCK_Test
call %run_cmd% %dopure% %platform% SOCK_Connector_Test
call %run_cmd% %dopure% %platform% SOCK_Send_Recv_Test
call %run_cmd% %dopure% %platform% SPIPE_Test
call %run_cmd% %dopure% %platform% SString_Test
call %run_cmd% %dopure% %platform% SV_Shared_Memory_Test
call %run_cmd% %dopure% %platform% Task_Test
call %run_cmd% %dopure% %platform% Thread_Manager_Test
call %run_cmd% %dopure% %platform% Thread_Mutex_Test
call %run_cmd% %dopure% %platform% Thread_Pool_Reactor_Test
call %run_cmd% %dopure% %platform% Thread_Pool_Test
call %run_cmd% %dopure% %platform% Timer_Queue_Test
call %run_cmd% %dopure% %platform% Timeprobe_Test
if exist ..\netsvcs\servers\main.exe call %run_cmd% %dopure% %platform% Time_Service_Test
call %run_cmd% %dopure% %platform% Time_Value_Test
call %run_cmd% %dopure% %platform% Tokens_Test
call %run_cmd% %dopure% %platform% TSS_Test
call %run_cmd% %dopure% %platform% UPIPE_SAP_Test
call %run_cmd% %dopure% %platform% Upgradable_RW_Test

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
type log\%arg%.log | find /I "LM_ERROR"
echo.

goto done

:nofile
echo %exefile%.exe not found
goto done

:fine

rem We should check the log files here to make sure the test ended correctly
rem type log\%arg%.log | find "Ending"

:done

endlocal
