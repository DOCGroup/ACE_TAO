@echo off
@echo This is the NT version of the one-button ACE tests.
@echo on
@echo Starting tests...

Barrier_Test
Buffer_Stream_Test
Future_Test
Handle_Set_Test
Map_Manager_Test
Mem_Map_Test
Message_Queue_Test
MM_Shared_Memory_Test
@echo The following test runs for about 1 minute
Mutex_Test
Naming_Test
Pipe_Test
Priority_Buffer_Test
Process_Mutex_Test
Reactor_Timer_Test
Reactors_Test
Reader_Writer_Test
Recursive_Mutex_Test
Service_Config_Test
SOCK_Test
SPIPE_Test
SString_Test
SV_Shared_Memory_Test
Service_Config_Test
TSS_Test
Task_Test
Thread_Manager_Test
Thread_Pool_Test
Time_Service_Test
Time_Value_Test
Timer_Queue_Test
Tokens_Test
UPIPE_SAP_Test

@echo Tests complete...
