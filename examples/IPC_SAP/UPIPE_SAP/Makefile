#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the simple ACE_UPIPE client/server test file
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN   = ex1 \
        ex2 \
        ex3

LSRC   = $(addsuffix .cpp,$(BIN))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ex1.o .obj/ex1.so .shobj/ex1.o .shobj/ex1.so: ex1.cpp $(ACE_ROOT)/ace/Stream.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/UPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/UPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/UPIPE_Addr.h \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/UPIPE_Connector.h
.obj/ex2.o .obj/ex2.so .shobj/ex2.o .shobj/ex2.so: ex2.cpp $(ACE_ROOT)/ace/UPIPE_Connector.h \
 $(ACE_ROOT)/ace/UPIPE_Stream.h \
 $(ACE_ROOT)/ace/Stream.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/UPIPE_Addr.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/UPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/Auto_Ptr.h
.obj/ex3.o .obj/ex3.so .shobj/ex3.o .shobj/ex3.so: ex3.cpp $(ACE_ROOT)/ace/UPIPE_Connector.h \
 $(ACE_ROOT)/ace/UPIPE_Stream.h \
 $(ACE_ROOT)/ace/Stream.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/SPIPE.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SPIPE_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/SPIPE.i \
 $(ACE_ROOT)/ace/UPIPE_Addr.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.h \
 $(ACE_ROOT)/ace/SPIPE_Stream.i \
 $(ACE_ROOT)/ace/UPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/SPIPE_Acceptor.h \
 $(ACE_ROOT)/ace/Auto_Ptr.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
