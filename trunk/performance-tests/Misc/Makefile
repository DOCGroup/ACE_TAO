#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for tests of the miscellaneous ACE performance tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = basic_perf \
	  childbirth_time \
	  context_switch_time \
	  test_mutex \
	  test_singleton \
	  test_naming
# Only build static library for optimum run-time speed.
LIB	= libPerf.a

SRC   	= $(addsuffix .cpp,$(BIN))
OBJ   	= $(SRC:%.cpp=$(VDIR)%.o)
LSRC  	= basic_func.cpp
LDLIBS 	= -lPerf
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

basic_func.o: basic_func.cpp basic_func.h
basic_perf.o: basic_perf.cpp basic_func.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.i
childbirth_time.o: childbirth_time.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Process.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Process.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Time_Value.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.i
context_switch_time.o: context_switch_time.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Sched_Params.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Sched_Params.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Task.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Object.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Shared_Object.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Shared_Object.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Object.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread_Manager.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread_Manager.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Task.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Task_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Block.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Memory_Pool.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Signal.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Signal.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Mem_Map.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Mem_Map.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Memory_Pool.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Block.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IO_Cntl_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Config.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Reactor.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Handle_Set.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Handle_Set.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Time_Value.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Token.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Token.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Pipe.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Pipe.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Stream.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_IO.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Addr.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Addr.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IPC_SAP.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IPC_SAP.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_IO.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/INET_Addr.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/INET_Addr.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Stream.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Reactor.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Proactor.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Asynch_IO.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ReactorEx.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ReactorEx.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Svc_Conf_Tokens.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_Options.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Hash_Map_Manager.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Hash_Map_Manager.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Task_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Task_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Module.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Module.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Module.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Stream_Modules.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Stream_Modules.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Stream_Modules.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Get_Opt.i
test_mutex.o: test_mutex.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Time_Value.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.i
test_singleton.o: test_singleton.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Time_Value.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Singleton.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Singleton.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Singleton.cpp
test_naming.o: test_naming.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SString.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/stdcpp.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/OS.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Trace.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Priority.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Log_Record.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ACE.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SString.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Naming_Context.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Containers.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Object.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Shared_Object.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Shared_Object.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Event_Handler.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Object.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Name_Proxy.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/INET_Addr.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Addr.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Addr.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/INET_Addr.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Connector.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Stream.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_IO.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IPC_SAP.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IPC_SAP.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_IO.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Stream.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Time_Value.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SOCK_Connector.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Config.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread_Manager.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Simple.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/SV_Semaphore_Complex.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Thread_Manager.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Signal.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Signal.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Service_Config.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Reactor.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Handle_Set.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Handle_Set.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Free_List.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Queue_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Token.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Token.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Pipe.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Pipe.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Reactor.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Proactor.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Asynch_IO.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_List_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Heap_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Timer_Wheel_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/High_Res_Timer.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ReactorEx.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Block.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Malloc_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Memory_Pool.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Mem_Map.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Mem_Map.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Memory_Pool.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Block.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/IO_Cntl_Msg.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies_T.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Synch_Options.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Hash_Map_Manager.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Hash_Map_Manager.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Strategies_T.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Message_Queue.cpp \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/ReactorEx.i \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Svc_Conf_Tokens.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Name_Request_Reply.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Name_Space.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.h \
 /project/merengue/levine/ACE_wrappers/build/SunOS5_g++/ace/Profile_Timer.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
