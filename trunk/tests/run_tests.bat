@echo off
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

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