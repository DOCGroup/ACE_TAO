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
 $(ACE_ROOT)/ace/Thread_Manager.i \
 Options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
