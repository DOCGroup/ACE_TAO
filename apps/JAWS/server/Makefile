#----------------------------------------------------------------------------
#	%W%	%G%
#
#	Makefile for the ACE Adapter Web Server (JAWS)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= main

FILES  	= \
	HTTP_Server \
	HTTP_Handler \
	HTTP_Helpers \
	HTTP_Request \
	HTTP_Response \
	Parse_Headers \
	VFS \
	IO 

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

INCLDIRS += -I../..

LDLIBS	= $(addprefix .shobj/,$(SHOBJ))
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
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

.obj/HTTP_Server.o .shobj/HTTP_Server.so: HTTP_Server.cpp \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
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
 $(WRAPPER_ROOT)/ace/Get_Opt.i \
 $(WRAPPER_ROOT)/ace/Asynch_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
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
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 ../../JAWS/server/IO.h ../../JAWS/server/HTTP_Server.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
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
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 ../../JAWS/server/HTTP_Handler.h ../../JAWS/server/HTTP_Request.h \
 ../../JAWS/server/Parse_Headers.h ../../JAWS/server/HTTP_Response.h \
 ../../JAWS/server/HTTP_Server_T.h
.obj/HTTP_Handler.o .shobj/HTTP_Handler.so: HTTP_Handler.cpp \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
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
 ../../JAWS/server/HTTP_Handler.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 ../../JAWS/server/HTTP_Request.h ../../JAWS/server/Parse_Headers.h \
 ../../JAWS/server/HTTP_Response.h ../../JAWS/server/IO.h \
 ../../JAWS/server/HTTP_Helpers.h
.obj/HTTP_Helpers.o .shobj/HTTP_Helpers.so: HTTP_Helpers.cpp ../../JAWS/server/HTTP_Helpers.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
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
 $(WRAPPER_ROOT)/ace/Thread.i
.obj/HTTP_Request.o .shobj/HTTP_Request.so: HTTP_Request.cpp \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
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
 ../../JAWS/server/HTTP_Request.h ../../JAWS/server/Parse_Headers.h \
 ../../JAWS/server/HTTP_Helpers.h
.obj/HTTP_Response.o .shobj/HTTP_Response.so: HTTP_Response.cpp ../../JAWS/server/HTTP_Response.h \
 ../../JAWS/server/HTTP_Request.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 ../../JAWS/server/Parse_Headers.h ../../JAWS/server/HTTP_Helpers.h \
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
 ../../JAWS/server/IO.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h
.obj/Parse_Headers.o .shobj/Parse_Headers.so: Parse_Headers.cpp \
 $(WRAPPER_ROOT)/apps/JAWS/server/Parse_Headers.h
.obj/VFS.o .shobj/VFS.so: VFS.cpp ../../JAWS/server/VFS.h ../../JAWS/server/JXH_List.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
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
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
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
 ../../JAWS/server/VFS_T.h ../../JAWS/server/HTTP_Helpers.h
.obj/IO.o .shobj/IO.so: IO.cpp ../../JAWS/server/IO.h \
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
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 ../../JAWS/server/HTTP_Helpers.h \
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
 ../../JAWS/server/VFS.h ../../JAWS/server/JXH_List.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 ../../JAWS/server/VFS_T.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
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
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
