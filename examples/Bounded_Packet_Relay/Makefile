#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO    = README

LIB     = libBPRD.a
SHLIB   = libBPRD.$(SOEXT)

BIN     = bpr_thread

PSRC   	= $(addsuffix .cpp,$(BIN))

LSRC  	= Thread_Bounded_Packet_Relay.cpp \
	  BPR_Drivers.cpp

LDLIBS 	= -lBPRD

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VSHLIB) $(SHLIBA) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include         $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

Thread_Bounded_Packet_Relay.o: Thread_Bounded_Packet_Relay.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/inc_user_config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config-g++-common.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/streams.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Trace.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Priority.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Atomic_Op.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_Base.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/IO_Cntl_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor_Impl.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Svc_Conf_Tokens.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Heap_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Heap_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.cpp \
 Thread_Bounded_Packet_Relay.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.cpp \
 BPR_Drivers.h BPR_Drivers_T.h BPR_Drivers_T.cpp
BPR_Drivers.o: BPR_Drivers.cpp BPR_Drivers.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/inc_user_config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config-g++-common.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/streams.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Trace.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Priority.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Atomic_Op.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_Base.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/IO_Cntl_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor_Impl.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Svc_Conf_Tokens.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.cpp \
 BPR_Drivers_T.h BPR_Drivers_T.cpp
bpr_thread.o: bpr_thread.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Auto_Ptr.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/inc_user_config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/config-g++-common.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/streams.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Basic_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/OS.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Trace.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/ACE.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Priority.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Log_Record.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Auto_Ptr.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Auto_Ptr.cpp \
 Thread_Bounded_Packet_Relay.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Functor_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Shared_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Event_Handler.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Simple.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SV_Semaphore_Complex.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Atomic_Op.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Containers_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_Base.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Free_List.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Malloc_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Signal.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Object_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Managed_Object.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Mem_Map.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Memory_Pool.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Thread_Manager.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Block.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/IO_Cntl_Msg.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/SString.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Config.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Handle_Set.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Reactor_Impl.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Svc_Conf_Tokens.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Synch_Options.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Hash_Map_Manager.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Types.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Service_Repository.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/WFMO_Reactor.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Strategies.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Message_Queue.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Task_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Module.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Stream_Modules.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Heap_T.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Heap_T.cpp \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.h \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.i \
 /project/doc/cdgill/ACE_wrappers/build/sunc++/ace/Timer_Queue_Adapters.cpp \
 BPR_Drivers.h BPR_Drivers_T.h BPR_Drivers_T.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
