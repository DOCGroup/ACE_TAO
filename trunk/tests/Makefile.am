##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for all the ACE ``one-button'' tests
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## We can't add "CLASSIX" to the SUBDIRS list as this point since "CLASSIX"
## would be built before the "test" directory.  How do we get around this
## without moving directories around?
##                         -Ossama
## SUBDIRS = CLASSIX \
##           WinCE \
##           version_tests

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

## LDFLAGS = 
## Libtool will automatically link against the "proper" library.
## Do not change the "libACE.la."  It is not a typographical error!
##
## We override the "LIBS" variable provided by the configure script since
## we should only need to link against the ACE library for these tests.
LDADD = $(top_builddir)/ace/libACE.la
##LIBS = $(top_builddir)/ace/libACE.la

noinst_SCRIPTS = run_tests.sh \
                 run_tests.psosim \
                 run_tests.check \
                 run_tests.vxworks \
                 run_tests.bat

if COMPILE_XTREACTOR_TEST
XTREACTOR_TEST = XtReactor_Test
else
XTREACTOR_TEST = 
endif

## Build the shared library needed for the DLL_Test
noinst_LTLIBRARIES = libDLL_Test.la
libDLL_Test_la_LDFLAGS = -module
libDLL_Test_la_SOURCES = $(DLL_Test_SOURCES)

## Build the test programs
noinst_PROGRAMS = \
	Aio_Platform_Test \
	Atomic_Op_Test \
	Barrier_Test \
	Basic_Types_Test \
	Buffer_Stream_Test \
        CDR_Test \
	Conn_Test \
        DLL_Test \
        DLList_Test \
	Dynamic_Priority_Test \
	Enum_Interfaces_Test \
	Env_Value_Test \
	Future_Test \
	Handle_Set_Test \
	Hash_Map_Manager_Test \
        High_Res_Timer_Test \
	IOStream_Test \
	MM_Shared_Memory_Test \
	MT_Reactor_Timer_Test \
	MT_SOCK_Test \
	Map_Manager_Test \
	Mem_Map_Test \
	Message_Block_Test \
	Message_Queue_Notifications_Test \
	Message_Queue_Test \
	Naming_Test \
	Notify_Performance_Test \
	OrdMultiSet_Test \
	Pipe_Test \
	Priority_Buffer_Test \
	Priority_Reactor_Test \
	Priority_Task_Test \
	Process_Mutex_Test \
	Process_Strategy_Test \
	RB_Tree_Test \
	Reactor_Exceptions_Test \
	Reactor_Notify_Test \
	Reactor_Performance_Test \
	Reactor_Timer_Test \
	Reactors_Test \
	Reader_Writer_Test \
	Recursive_Mutex_Test \
	SOCK_Connector_Test \
	SOCK_Test \
	SPIPE_Test \
	SString_Test \
	SV_Shared_Memory_Test \
	Semaphore_Test \
	Service_Config_Test \
	Sigset_Ops_Test \
	Simple_Message_Block_Test \
	TSS_Test \
	Task_Test \
	Thread_Manager_Test \
	Thread_Mutex_Test \
        Thread_Pool_Reactor_Test \
	Thread_Pool_Test \
	Time_Service_Test \
	Time_Value_Test \
	Timeprobe_Test \
	Timer_Queue_Test \
	Tokens_Test \
	UPIPE_SAP_Test \
        Upgradable_RW_Test $(XTREACTOR_TEST)

Aio_Platform_Test_SOURCES = Aio_Platform_Test.cpp

Atomic_Op_Test_SOURCES = Atomic_Op_Test.cpp

Barrier_Test_SOURCES = Barrier_Test.cpp

Basic_Types_Test_SOURCES = Basic_Types_Test.cpp

Buffer_Stream_Test_SOURCES = Buffer_Stream_Test.cpp

CDR_Test_SOURCES = CDR_Test.cpp

Conn_Test_SOURCES = Conn_Test.cpp Conn_Test.h

DLL_Test_SOURCES = DLL_Test.cpp DLL_Test.h

DLList_Test_SOURCES = DLList_Test.cpp

Dynamic_Priority_Test_SOURCES = Dynamic_Priority_Test.cpp

Enum_Interfaces_Test_SOURCES = Enum_Interfaces_Test.cpp

Env_Value_Test_SOURCES = Env_Value_Test.cpp

Future_Test_SOURCES = Future_Test.cpp

Handle_Set_Test_SOURCES = Handle_Set_Test.cpp

Hash_Map_Manager_Test_SOURCES = Hash_Map_Manager_Test.cpp

High_Res_Timer_Test_SOURCES = High_Res_Timer_Test.cpp

IOStream_Test_SOURCES = IOStream_Test.cpp

MM_Shared_Memory_Test_SOURCES = MM_Shared_Memory_Test.cpp

MT_Reactor_Timer_Test_SOURCES = MT_Reactor_Timer_Test.cpp \
                                MT_Reactor_Timer_Test.h

MT_SOCK_Test_SOURCES = MT_SOCK_Test.cpp

Map_Manager_Test_SOURCES = Map_Manager_Test.cpp

Mem_Map_Test_SOURCES = Mem_Map_Test.cpp

Message_Block_Test_SOURCES = Message_Block_Test.cpp

Message_Queue_Notifications_Test_SOURCES = Message_Queue_Notifications_Test.cpp

Message_Queue_Test_SOURCES = Message_Queue_Test.cpp

Naming_Test_SOURCES = Naming_Test.cpp

Notify_Performance_Test_SOURCES = Notify_Performance_Test.cpp

OrdMultiSet_Test_SOURCES = OrdMultiSet_Test.cpp

Pipe_Test_SOURCES = Pipe_Test.cpp

Priority_Buffer_Test_SOURCES = Priority_Buffer_Test.cpp

Priority_Reactor_Test_SOURCES = Priority_Reactor_Test.cpp \
                                Priority_Reactor_Test.h

Priority_Task_Test_SOURCES = Priority_Task_Test.cpp

Process_Mutex_Test_SOURCES = Process_Mutex_Test.cpp

Process_Strategy_Test_SOURCES = Process_Strategy_Test.cpp \
                                Process_Strategy_Test.h

RB_Tree_Test_SOURCES = RB_Tree_Test.cpp

Reactor_Exceptions_Test_SOURCES = Reactor_Exceptions_Test.cpp

Reactor_Notify_Test_SOURCES = Reactor_Notify_Test.cpp

Reactor_Performance_Test_SOURCES = Reactor_Performance_Test.cpp \
                                   Reactor_Performance_Test.h

Reactor_Timer_Test_SOURCES = Reactor_Timer_Test.cpp

Reactors_Test_SOURCES = Reactors_Test.cpp

Reader_Writer_Test_SOURCES = Reader_Writer_Test.cpp

Recursive_Mutex_Test_SOURCES = Recursive_Mutex_Test.cpp

SOCK_Connector_Test_SOURCES = SOCK_Connector_Test.cpp

SOCK_Test_SOURCES = SOCK_Test.cpp

SPIPE_Test_SOURCES = SPIPE_Test.cpp

SString_Test_SOURCES = SString_Test.cpp

SV_Shared_Memory_Test_SOURCES = SV_Shared_Memory_Test.cpp

Semaphore_Test_SOURCES = Semaphore_Test.cpp

Service_Config_Test_SOURCES = Service_Config_Test.cpp

Sigset_Ops_Test_SOURCES = Sigset_Ops_Test.cpp

Simple_Message_Block_Test_SOURCES = Simple_Message_Block_Test.cpp

TSS_Test_SOURCES = TSS_Test.cpp \
                   TSS_Test_Errno.h

Task_Test_SOURCES = Task_Test.cpp

Thread_Manager_Test_SOURCES = Thread_Manager_Test.cpp \
                              Thread_Manager_Test.h

Thread_Mutex_Test_SOURCES = Thread_Mutex_Test.cpp

Thread_Pool_Reactor_Test_SOURCES = Thread_Pool_Reactor_Test.cpp \
                                   Thread_Pool_Reactor_Test.h

Thread_Pool_Test_SOURCES = Thread_Pool_Test.cpp

Time_Service_Test_SOURCES = Time_Service_Test.cpp

Time_Value_Test_SOURCES = Time_Value_Test.cpp

Timeprobe_Test_SOURCES = Timeprobe_Test.cpp

Timer_Queue_Test_SOURCES = Timer_Queue_Test.cpp

Tokens_Test_SOURCES = Tokens_Test.cpp

UPIPE_SAP_Test_SOURCES = UPIPE_SAP_Test.cpp

Upgradable_RW_Test_SOURCES = Upgradable_RW_Test.cpp \
                             Upgradable_RW_Test.h

XtReactor_Test_SOURCES = XtReactor_Test.cpp
XtReactor_Test_LDADD = $(top_builddir)/ace/libACE.la @XTREACTOR_TEST_XLIBS@

## Additional test header files
noinst_HEADERS = test_config.h

## Architecture independent data files
noinst_DATA = UNIXserver.conf \
              UNIXclerk.conf \
              UNIXtokens.conf \
              Win32server.conf \
              Win32clerk.conf \
              Win32tokens.conf


## The tests we want to run are all of the test program we are going to build.
## Do not run them if they are cross-compiled.
if ACE_CROSS_COMPILED
TESTS =
else
TESTS = $(noinst_PROGRAMS)
endif

## Set up the test environment
##TESTS_ENVIRONMENT = 

EXTRA_DIST = $(noinst_SCRIPTS) $(noinst_DATA)

##DISTCLEANFILES = ace_pipe_name pattern \
##                 $(tmp)/ace_temp_file* \
##                 $(tmp)/ace_test_file \
##                 $(tmp)/Naming_Test*

## Run local targets
all-local: log_dir

## Create tests `log' directory
log_dir:
	if test -d log; then : else mkdir log; fi

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

## Clean up `log' directory , etc.
distclean-local:
	-rm -rf log
