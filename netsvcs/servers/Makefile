#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile 
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= main 
LSRC   	= $(addsuffix .cpp,$(BIN))

LDLIBS 	= -lnetsvcs
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -I$(WRAPPER_ROOT)/netsvcs/lib

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/main.o .shobj/main.so: main.cpp \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
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
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Set.h \
 $(WRAPPER_ROOT)/ace/Set.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
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
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/netsvcs/lib/TS_Clerk_Handler.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Time_Request_Reply.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/netsvcs/lib/TS_Server_Handler.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/netsvcs/lib/Client_Logging_Handler.h \
 $(WRAPPER_ROOT)/ace/FIFO_Recv_Msg.h \
 $(WRAPPER_ROOT)/ace/FIFO_Recv.h \
 $(WRAPPER_ROOT)/ace/FIFO.h \
 $(WRAPPER_ROOT)/ace/FIFO.i \
 $(WRAPPER_ROOT)/ace/FIFO_Recv.i \
 $(WRAPPER_ROOT)/ace/FIFO_Recv_Msg.i \
 $(WRAPPER_ROOT)/netsvcs/lib/Name_Handler.h \
 $(WRAPPER_ROOT)/ace/Naming_Context.h \
 $(WRAPPER_ROOT)/ace/Name_Proxy.h \
 $(WRAPPER_ROOT)/ace/Name_Request_Reply.h \
 $(WRAPPER_ROOT)/ace/Name_Space.h \
 $(WRAPPER_ROOT)/netsvcs/lib/Token_Handler.h \
 $(WRAPPER_ROOT)/ace/Local_Tokens.h \
 $(WRAPPER_ROOT)/ace/Stack.h \
 $(WRAPPER_ROOT)/ace/Stack.i \
 $(WRAPPER_ROOT)/ace/Local_Tokens.i \
 $(WRAPPER_ROOT)/ace/Token_Collection.h \
 $(WRAPPER_ROOT)/ace/Token_Collection.i \
 $(WRAPPER_ROOT)/ace/Token_Request_Reply.h \
 $(WRAPPER_ROOT)/ace/Token_Request_Reply.i \
 $(WRAPPER_ROOT)/netsvcs/lib/Server_Logging_Handler.h \
 $(WRAPPER_ROOT)/ace/TLI_Acceptor.h \
 $(WRAPPER_ROOT)/ace/TLI.h \
 $(WRAPPER_ROOT)/ace/TLI.i \
 $(WRAPPER_ROOT)/ace/TLI_Stream.h \
 $(WRAPPER_ROOT)/ace/TLI_Stream.i \
 $(WRAPPER_ROOT)/ace/TLI_Acceptor.i \
 $(WRAPPER_ROOT)/netsvcs/lib/Logging_Strategy.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
