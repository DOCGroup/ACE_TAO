#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the simple ACE_UPIPE client/server test file
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   = ex1 \
        ex2 \
        ex3

LSRC   = $(addsuffix .cpp,$(BIN))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ex1.o .shobj/ex1.so: ex1.cpp \
 $(WRAPPER_ROOT)/ace/Stream.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Module.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Module.i \
 $(WRAPPER_ROOT)/ace/Stream.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.i \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.i
.obj/ex2.o .shobj/ex2.so: ex2.cpp \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/Stream.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Module.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Module.i \
 $(WRAPPER_ROOT)/ace/Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.i \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.i \
 auto_builtin_ptr.h
.obj/ex3.o .shobj/ex3.so: ex3.cpp \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/Stream.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Module.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Module.i \
 $(WRAPPER_ROOT)/ace/Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Addr.i \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/SPIPE.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Addr.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Stream.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Connector.i \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SPIPE_Acceptor.h \
 $(WRAPPER_ROOT)/ace/UPIPE_Acceptor.i \
 auto_builtin_ptr.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
