#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the Solaris 2.x synchronization benchmarks
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN	= synch_driver
LIB	= libSynch_Tests.a
SHLIB	= libSynch_Tests.so

FILES	= mutex_test \
	  recursive_lock_test \
	  sema_test \
	  sysvsema_test \
	  rwrd_test \
	  rwwr_test \
	  context_test \
	  condb_test \
	  conds_test \
	  memory_test \
	  pipe_thr_test \
	  pipe_proc_test \
	  Options \
	  Benchmark

SRC   	= $(addsuffix .cpp,$(BIN))
OBJ   	= $(SRC:%.cpp=$(VDIR)%.o)

LSRC	= $(addsuffix .cpp,$(FILES)) 
LOBJ   	= $(LSRC:%.cpp=$(VDIR)%.o)
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS 	= -lSynch_Tests

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VSHLIB) $(SHLIBA) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/mutex_test.o .shobj/mutex_test.so: mutex_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/recursive_lock_test.o .shobj/recursive_lock_test.so: recursive_lock_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/sema_test.o .shobj/sema_test.so: sema_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/sysvsema_test.o .shobj/sysvsema_test.so: sysvsema_test.cpp \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 Options.h $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/rwrd_test.o .shobj/rwrd_test.so: rwrd_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/rwwr_test.o .shobj/rwwr_test.so: rwwr_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/context_test.o .shobj/context_test.so: context_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/condb_test.o .shobj/condb_test.so: condb_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/conds_test.o .shobj/conds_test.so: conds_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/memory_test.o .shobj/memory_test.so: memory_test.cpp \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/pipe_thr_test.o .shobj/pipe_thr_test.so: pipe_thr_test.cpp \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/pipe_proc_test.o .shobj/pipe_proc_test.so: pipe_proc_test.cpp Options.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/Options.o .shobj/Options.so: Options.cpp Options.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/Benchmark.o .shobj/Benchmark.so: Benchmark.cpp Benchmark.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i
.obj/synch_driver.o .shobj/synch_driver.so: synch_driver.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/stdcpp.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 Options.h $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i Benchmark.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
