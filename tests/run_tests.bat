@echo off
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

if not "%1" == "" goto runtest

call run_tests Barrier_Test
call run_tests Buffer_Stream_Test
call run_tests Conn_Test
call run_tests Enum_Interfaces_Test
call run_tests Future_Test
call run_tests Handle_Set_Test
call run_tests Hash_Map_Manager_Test
call run_tests IOStream_Test
call run_tests Map_Manager_Test
call run_tests Mem_Map_Test
call run_tests Message_Block_Test
call run_tests Message_Queue_Notifications_Test
call run_tests Message_Queue_Test
call run_tests MM_Shared_Memory_Test
call run_tests Mutex_Test
call run_tests Naming_Test
call run_tests Pipe_Test
call run_tests Priority_Buffer_Test
call run_tests Priority_Task_Test
call run_tests Process_Mutex_Test
call run_tests Process_Strategy_Test
call run_tests Reactors_Test
call run_tests Reactor_Exceptions_Test
call run_tests Reactor_Notify_Test
call run_tests Reactor_Timer_Test
call run_tests Reader_Writer_Test
call run_tests Recursive_Mutex_Test
call run_tests Semaphore_Test
call run_tests Service_Config_Test
call run_tests Sigset_Ops_Test
call run_tests Simple_Message_Block_Test
call run_tests SOCK_Test
call run_tests SPIPE_Test
call run_tests SString_Test
call run_tests SV_Shared_Memory_Test
call run_tests Task_Test
call run_tests Thread_Manager_Test
call run_tests Thread_Pool_Test
call run_tests Timer_Queue_Test
call run_tests Time_Service_Test
call run_tests Time_Value_Test
call run_tests Tokens_Test
call run_tests TSS_Test
call run_tests UPIPE_SAP_Test

goto done

:runtest

echo Running %1
%1.exe
if errorlevel 0 goto fine
echo.
echo %1 has FAILED!!!
echo.
type %temp%\log\%1.log | find /I "assertion failed"
type %temp%\log\%1.log | find /I "not supported"
type %temp%\log\%1.log | find /I "no such file or directory"
type %temp%\log\%1.log | find /I "invalid argument"
type %temp%\log\%1.log | find /I "timeout"
type %temp%\log\%1.log | find /I "bad file number"
echo.

goto done
:fine

rem We should check the log files here to make sure the test ended correctly
rem type %temp%\log\%1.log | find "Ending"

:done