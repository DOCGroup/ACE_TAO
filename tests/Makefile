#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the ACE ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   Aio_Platform_Test \
        Atomic_Op_Test \
        Barrier_Test \
        Basic_Types_Test \
        Buffer_Stream_Test \
        Cache_Map_Manager_Test \
        Cached_Accept_Conn_Test \
        Cached_Conn_Test \
        CDR_Test \
        Collection_Test \
        Conn_Test \
        DLList_Test \
        DLL_Test\
        Enum_Interfaces_Test \
        Env_Value_Test \
        Future_Test \
        Future_Set_Test \
        Handle_Set_Test \
        High_Res_Timer_Test \
        OrdMultiSet_Test \
        Hash_Map_Manager_Test \
        Hash_Map_Bucket_Iterator_Test \
        IOStream_Test \
        Malloc_Test \
        Map_Test \
        Map_Manager_Test \
        Mem_Map_Test \
        MM_Shared_Memory_Test \
        MT_Reactor_Timer_Test \
        MT_SOCK_Test \
        Message_Block_Test \
        Message_Queue_Test \
        Message_Queue_Notifications_Test \
        New_Fail_Test \
        Notify_Performance_Test \
        Object_Manager_Test \
        Process_Mutex_Test \
        Process_Strategy_Test \
        Priority_Buffer_Test \
        Dynamic_Priority_Test \
        Priority_Task_Test \
        Priority_Reactor_Test \
        Process_Manager_Test \
        Pipe_Test \
        RB_Tree_Test \
        Reactors_Test \
        Reactor_Exceptions_Test \
        Reactor_Notify_Test \
        Reactor_Performance_Test \
        Reactor_Timer_Test \
        Reader_Writer_Test \
        Recursive_Mutex_Test \
        Reverse_Lock_Test \
        Semaphore_Test \
        Service_Config_Test \
	Signal_Test \
        Sigset_Ops_Test \
        Simple_Message_Block_Test \
        SOCK_Test \
        SOCK_Connector_Test \
        SOCK_Send_Recv_Test \
        SPIPE_Test \
        SString_Test \
        SV_Shared_Memory_Test \
        Svc_Handler_Test \
        Task_Test \
        Thread_Manager_Test \
        Thread_Mutex_Test \
        Thread_Pool_Test \
        Timeprobe_Test \
        Time_Service_Test \
        Time_Value_Test \
        Timer_Queue_Test \
        TSS_Test \
        UPIPE_SAP_Test \
        Upgradable_RW_Test

BIN2 =  Naming_Test \
        Thread_Pool_Reactor_Test \
        XtReactor_Test \
        FlReactor_Test \
        TkReactor_Test

#### If the ACE library wasn't built with all components, don't
#### try to build certain tests.
ACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
  BIN += $(BIN2)
endif # ! Other
ifeq (Token,$(findstring Token,$(ACE_BUILD_COMPONENTS)))
  BIN += Tokens_Test
else
  #### Ensure that Tokens_Test gets cleaned up, even if the ace
  #### directory was realcleaned first (and ACE_BUILD_COMPONENTS
  #### is empty).
  BIN2 += Tokens_Test
endif # ! Token

PSRC=$(addsuffix .cpp,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifndef SOEXT
  SOEXT=so
endif
ifdef shared_libs
  ifndef VXWORKS
    BUILD += libDLL_Test.$(SOEXT)
  endif # ! VXWORKS
endif # shared_libs

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

libDLL_Test.$(SOEXT): DLL_Test.cpp
ifdef TEMPINCDIR
	$(RM) .obj/DLL_Test.o
endif
	@$(MAKE) -f Makefile.DLL

realclean: clean
	-$(RM) libDLL_Test.$(SOEXT) log/compilations.log*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
