#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for all the ACE ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN =   Aio_Platform_Test \
	Atomic_Op_Test \
	Barrier_Test \
	Basic_Types_Test \
	Buffer_Stream_Test \
        CDR_Test \
	Collection_Test \
	Conn_Test \
	DLList_Test \
	DLL_Test\
	Enum_Interfaces_Test \
	Env_Value_Test \
	Future_Test \
	Handle_Set_Test \
	High_Res_Timer_Test \
	OrdMultiSet_Test \
	Hash_Map_Manager_Test \
	IOStream_Test \
	Map_Test \
	Map_Manager_Test \
	Mem_Map_Test \
	MM_Shared_Memory_Test \
	MT_Reactor_Timer_Test \
	MT_SOCK_Test \
	Message_Block_Test \
	Message_Queue_Test \
	Message_Queue_Notifications_Test \
	Naming_Test \
	Notify_Performance_Test \
	Process_Mutex_Test \
	Process_Strategy_Test \
	Priority_Buffer_Test \
	Dynamic_Priority_Test \
	Priority_Task_Test \
	Priority_Reactor_Test \
	Pipe_Test \
	RB_Tree_Test \
	Reactors_Test \
	Reactor_Exceptions_Test \
	Reactor_Notify_Test \
	Reactor_Performance_Test \
	Reactor_Timer_Test \
	Reader_Writer_Test \
	Recursive_Mutex_Test \
	Semaphore_Test \
	Service_Config_Test \
	Sigset_Ops_Test \
	Simple_Message_Block_Test \
	SOCK_Test \
	SOCK_Connector_Test \
	SPIPE_Test \
	SString_Test \
	SV_Shared_Memory_Test \
	Task_Test \
	Thread_Manager_Test \
	Thread_Mutex_Test \
	Thread_Pool_Reactor_Test \
	Thread_Pool_Test \
	Timeprobe_Test \
	Time_Service_Test \
	Time_Value_Test \
	Timer_Queue_Test \
	Tokens_Test \
	TSS_Test \
	UPIPE_SAP_Test \
	Upgradable_RW_Test \
	XtReactor_Test 

FILES = DLL_Test

LSRC   = $(addsuffix .cpp,$(BIN))
VLDLIBS = $(LDLIBS:%=%$(VAR))

#BUILD = $(VBIN) Had to remove this as it was necessary to
#                produce a libDLL_Test.so for DLL_Test

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
DLL_TEST:
	make -f Makefile.DLL

all: $(VBIN) DLL_TEST
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
