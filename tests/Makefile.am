##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for all the ACE ``one-button'' tests
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## We can't add "CLASSIX" to the SUBDIRS list as this point since "CLASSIX"
## would be built before the "test" directory.  How do we get around this
## without moving directories around?
##                         -Ossama
## SUBDIRS = CLASSIX \
##           WinCE \
##           version_tests

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)


if BUILD_SSL
SSL_DIR = SSL
else
SSL_DIR =
endif

## List of conditionally built subdirectories.
##
## Note that the "." (current) directory in the below SUBDIRS list is
## required to make the ACE directory be built prior to the
## subdirectories.
SUBDIRS = \
  . \
  $(SSL_DIR)


## Local lib directory
libdir = @ACE_TESTS_DIR@

## LDFLAGS =
## Libtool will automatically link against the "proper" library.
## Do not change the "libACE.la."  It is not a typographical error!
##
## We override the "LIBS" variable provided by the configure script since
## we should only need to link against the ACE library for these tests.
LDADD = Main.o libTest_Output.la $(top_builddir)/ace/libACE.la
##LIBS = $(top_builddir)/ace/libACE.la

noinst_SCRIPTS = \
  run_tests.check \
  run_test.pl \
  run_tests_remote.sh


## @todo Do not build/run this test if running on LynxOS < 3.1.0.
CDR_ARRAY_TEST = CDR_Array_Test

## @todo Do not build these tests if their corresponding components in
##       ACE have not been enabled/built.
##if BUILD_CODECS_FILES
CODECS_TEST = Codecs_Test
##else
##CODECS_TEST =
##endif

##if BUILD_TOKEN_FILES
TOKENS_TEST = Tokens_Test
##else
##TOKENS_TEST =
##endif

if COMPILE_XTREACTOR_TEST
XTREACTOR_TEST = XtReactor_Test
else
XTREACTOR_TEST =
endif

## @todo Make the DLL_Test, Service_Config_Test and
##       Framework_Component tests conditional on shared library
##       builds.  Do not build in static library only builds.
FRAMEWORK_COMPONENT_TEST = Framework_Component_Test
## FRAMEWORK_COMPONENT_TEST =

## Build the shared libraries needed for the DLL_Test and the
## Service_Config_DLL test. 
## libdir = $(shell pwd)
lib_LTLIBRARIES = \
  libDLL_Test.la \
  libFramework_Component_DLL.la \
  libService_Config_DLL.la \
  libTest_Output.la

libTest_Output_la_SOURCES = \
  Test_Output.cpp Test_Output_Export.h test_config.h
libTest_Output_la_LIBADD = $(top_builddir)/ace/libACE.la

libDLL_Test_la_SOURCES = DLL_Test_Impl.cpp DLL_Test_Impl.h
libDLL_Test_la_LDFLAGS = -module

libFramework_Component_DLL_la_SOURCES = \
  Framework_Component_DLL.cpp \
  Framework_Component_DLL.h \
  Framework_Component_DLL_Export.h
libFramework_Component_DLL_la_LDFLAGS = -module
libFramework_Component_DLL_la_LIBADD = $(top_builddir)/ace/libACE.la

libService_Config_DLL_la_SOURCES = \
  Service_Config_DLL.cpp \
  Service_Config_DLL.h \
  Service_Config_DLL_Export.h
libService_Config_DLL_la_LDFLAGS = -module
libService_Config_DLL_la_LIBADD = $(top_builddir)/ace/libACE.la


## Build the following test programs when a `make check' is
## issued by the user.
##check_PROGRAMS =
noinst_PROGRAMS = \
  ACE_Test \
  Aio_Platform_Test \
  Arg_Shifter_Test \
  ARGV_Test \
  Atomic_Op_Test \
  Auto_IncDec_Test \
  Barrier_Test \
  Basic_Types_Test \
  Bound_Ptr_Test \
  Buffer_Stream_Test \
  Bug_1576_Regression_Test \
  Cache_Map_Manager_Test \
  Cached_Accept_Conn_Test \
  Cached_Allocator_Test \
  Cached_Conn_Test \
  Capabilities_Test \
  CDR_File_Test \
  CDR_Test \
  Collection_Test \
  Config_Test \
  Conn_Test \
  Date_Time_Test \
  Dev_Poll_Reactor_Test \
  Dirent_Test \
  DLL_Test\
  DLList_Test \
  Dynamic_Priority_Test \
  Enum_Interfaces_Test \
  Env_Value_Test \
  FIFO_Test \
  Future_Set_Test \
  Future_Test \
  Get_Opt_Test \
  Handle_Set_Test \
  Hash_Map_Bucket_Iterator_Test \
  Hash_Map_Manager_Test \
  High_Res_Timer_Test \
  INET_Addr_Test \
  IOStream_Test \
  Lazy_Map_Manager_Test \
  Log_Msg_Test \
  Logging_Strategy_Test \
  Malloc_Test \
  Map_Manager_Test \
  Map_Test \
  Max_Default_Port_Test \
  Mem_Map_Test \
  MEM_Stream_Test \
  Message_Block_Test \
  Message_Queue_Notifications_Test \
  Message_Queue_Test \
  Message_Queue_Test_Ex \
  MM_Shared_Memory_Test \
  MT_Reactor_Timer_Test \
  MT_Reactor_Upcall_Test \
  MT_Reference_Counted_Event_Handler_Test \
  MT_Reference_Counted_Notify_Test \
  MT_SOCK_Test \
  Multicast_Test \
  Multihomed_INET_Addr_Test \
  Network_Adapters_Test \
  NonBlocking_Conn_Test \
  Notify_Performance_Test \
  Object_Manager_Test \
  Obstack_Test \
  OrdMultiSet_Test \
  OS_Test \
  Pipe_Test \
  Priority_Buffer_Test \
  Priority_Reactor_Test \
  Priority_Task_Test \
  Proactor_Scatter_Gather_Test \
  Proactor_Test \
  Proactor_Timer_Test \
  Process_Manager_Test \
  Process_Mutex_Test \
  Process_Strategy_Test \
  RB_Tree_Test \
  Reactor_Dispatch_Order_Test \
  Reactor_Exceptions_Test \
  Reactor_Notify_Test \
  Reactor_Performance_Test \
  Reactor_Registration_Test \
  Reactor_Timer_Test \
  Reactors_Test \
  Reader_Writer_Test \
  Recursive_Condition_Test \
  Recursive_Mutex_Test \
  Refcounted_Auto_Ptr_Test \
  Reference_Counted_Event_Handler_Test \
  Reverse_Lock_Test \
  Semaphore_Test \
  Service_Config_Test \
  Signal_Test \
  Sigset_Ops_Test \
  Simple_Message_Block_Test \
  SOCK_Connector_Test \
  SOCK_Send_Recv_Test \
  SOCK_Test \
  SPIPE_Test \
  SString_Test \
  SV_Shared_Memory_Test \
  Svc_Handler_Test \
  Task_Test \
  Task_Ex_Test \
  Thread_Manager_Test \
  Thread_Mutex_Test \
  Thread_Pool_Test \
  Time_Service_Test \
  Time_Value_Test \
  Timeprobe_Test \
  Timer_Cancellation_Test \
  Timer_Queue_Reference_Counting_Test \
  Timer_Queue_Test \
  Token_Strategy_Test \
  TP_Reactor_Test \
  TSS_Test \
  TSS_Static_Test \
  Unbounded_Set_Test_Ex \
  Upgradable_RW_Test \
  UPIPE_SAP_Test \
  Vector_Test \
  WFMO_Reactor_Test \
  \
  Naming_Test \
  \
  FlReactor_Test \
  Thread_Pool_Reactor_Test \
  Thread_Pool_Reactor_Resume_Test \
  TkReactor_Test \
  UUIDTest \
  $(CDR_ARRAY_TEST) \
  $(CODECS_TEST) \
  $(FRAMEWORK_COMPONENT_TEST) \
  $(TOKENS_TEST) \
  $(XTREACTOR_TEST)

## Conditionally built IPv6 tests.
if BUILD_IPV6
noinst_PROGRAMS += \
  INET_Addr_Test_IPV6 \
  Max_Default_Port_Test_IPV6 \
  Multicast_Test_IPV6 \
  Multihomed_INET_Addr_Test_IPV6 \
  Proactor_Test_IPV6 \
  SOCK_Send_Recv_Test_IPV6 \
  SOCK_Test_IPv6
endif

ACE_Test_SOURCES = ACE_Test.cpp

Aio_Platform_Test_SOURCES = Aio_Platform_Test.cpp

Arg_Shifter_Test_SOURCES = Arg_Shifter_Test.cpp

ARGV_Test_SOURCES = ARGV_Test.cpp

Atomic_Op_Test_SOURCES = Atomic_Op_Test.cpp

Auto_IncDec_Test_SOURCES = Auto_IncDec_Test.cpp

Barrier_Test_SOURCES = Barrier_Test.cpp

Basic_Types_Test_SOURCES = Basic_Types_Test.cpp

Bound_Ptr_Test_SOURCES = Bound_Ptr_Test.cpp Bound_Ptr_Test.h

Buffer_Stream_Test_SOURCES = Buffer_Stream_Test.cpp

Bug_1576_Regression_Test_SOURCES = Bug_1576_Regression_Test.cpp

CDR_Array_Test_SOURCES = CDR_Array_Test.cpp

CDR_Test_SOURCES = CDR_Test.cpp

CDR_File_Test_SOURCES = CDR_File_Test.cpp

Cached_Accept_Conn_Test_SOURCES = Cached_Accept_Conn_Test.cpp \
                                  Cached_Accept_Conn_Test.h

Cached_Allocator_Test_SOURCES = Cached_Allocator_Test.cpp

Cached_Conn_Test_SOURCES = Cached_Conn_Test.cpp \
                           Cached_Conn_Test.h

Cache_Map_Manager_Test_SOURCES = Cache_Map_Manager_Test.cpp \
                                 Cache_Map_Manager_Test.h

Capabilities_Test_SOURCES = Capabilities_Test.cpp

Codecs_Test_SOURCES = Codecs_Test.cpp

Collection_Test_SOURCES = Collection_Test.cpp Collection_Test.h

Config_Test_SOURCES = Config_Test.cpp Config_Test.h

Conn_Test_SOURCES = Conn_Test.cpp Conn_Test.h

DLList_Test_SOURCES = DLList_Test.cpp

DLL_Test_SOURCES = DLL_Test.cpp DLL_Test.h
DLL_Test_DEPENDENCIES = libDLL_Test.la libTest_Output.la

Date_Time_Test_SOURCES = Date_Time_Test.cpp

Dev_Poll_Reactor_Test_SOURCES = Dev_Poll_Reactor_Test.cpp

Dirent_Test_SOURCES = Dirent_Test.cpp

Dynamic_Priority_Test_SOURCES = Dynamic_Priority_Test.cpp

Enum_Interfaces_Test_SOURCES = Enum_Interfaces_Test.cpp

Env_Value_Test_SOURCES = Env_Value_Test.cpp

FIFO_Test_SOURCES = FIFO_Test.cpp

FlReactor_Test_SOURCES = FlReactor_Test.cpp

Framework_Component_Test_SOURCES = Framework_Component_Test.cpp \
                                   Framework_Component_Test.h
Framework_Component_Test_DEPENDENCIES = \
  libFramework_Component_DLL.la libTest_Output.la

Future_Test_SOURCES = Future_Test.cpp

Future_Set_Test_SOURCES = Future_Set_Test.cpp

Get_Opt_Test_SOURCES = Get_Opt_Test.cpp

Handle_Set_Test_SOURCES = Handle_Set_Test.cpp

Hash_Map_Manager_Test_SOURCES = Hash_Map_Manager_Test.cpp

Hash_Map_Bucket_Iterator_Test_SOURCES = Hash_Map_Bucket_Iterator_Test.cpp

High_Res_Timer_Test_SOURCES = High_Res_Timer_Test.cpp

INET_Addr_Test_SOURCES = INET_Addr_Test.cpp

IOStream_Test_SOURCES = IOStream_Test.cpp

Lazy_Map_Manager_Test_SOURCES = Lazy_Map_Manager_Test.cpp

Log_Msg_Test_SOURCES = Log_Msg_Test.cpp

Logging_Strategy_Test_SOURCES = Logging_Strategy_Test.cpp

MEM_Stream_Test_SOURCES = MEM_Stream_Test.cpp MEM_Stream_Test.h

MM_Shared_Memory_Test_SOURCES = MM_Shared_Memory_Test.cpp

MT_Reactor_Timer_Test_SOURCES = MT_Reactor_Timer_Test.cpp \
                                MT_Reactor_Timer_Test.h

MT_Reactor_Upcall_Test_SOURCES = MT_Reactor_Upcall_Test.cpp

MT_Reference_Counted_Event_Handler_Test_SOURCES = \
  MT_Reference_Counted_Event_Handler_Test.cpp

MT_Reference_Counted_Notify_Test_SOURCES = \
  MT_Reference_Counted_Notify_Test.cpp

MT_SOCK_Test_SOURCES = MT_SOCK_Test.cpp

Malloc_Test_SOURCES = Malloc_Test.cpp Malloc_Test.h

Map_Manager_Test_SOURCES = Map_Manager_Test.cpp

Map_Test_SOURCES = Map_Test.cpp Map_Test.h

Max_Default_Port_Test_SOURCES = Max_Default_Port_Test.cpp \
                                Max_Default_Port_Test.h

Mem_Map_Test_SOURCES = Mem_Map_Test.cpp

Message_Block_Test_SOURCES = Message_Block_Test.cpp

Message_Queue_Notifications_Test_SOURCES = Message_Queue_Notifications_Test.cpp

Message_Queue_Test_SOURCES = Message_Queue_Test.cpp

Message_Queue_Test_Ex_SOURCES = Message_Queue_Test_Ex.cpp \
                                Message_Queue_Test_Ex.h

Multicast_Test_SOURCES = Multicast_Test.cpp

Multihomed_INET_Addr_Test_SOURCES = Multihomed_INET_Addr_Test.cpp

Naming_Test_SOURCES = Naming_Test.cpp

Network_Adapters_Test_SOURCES = \
  Network_Adapters_Test.cpp Network_Adapters_Test.h

NonBlocking_Conn_Test_SOURCES = \
  NonBlocking_Conn_Test.cpp NonBlocking_Conn_Test.h

Notify_Performance_Test_SOURCES = Notify_Performance_Test.cpp

OS_Test_SOURCES = OS_Test.cpp

Object_Manager_Test_SOURCES = Object_Manager_Test.cpp

Obstack_Test_SOURCES = Obstack_Test.cpp

OrdMultiSet_Test_SOURCES = OrdMultiSet_Test.cpp

Pipe_Test_SOURCES = Pipe_Test.cpp

Priority_Buffer_Test_SOURCES = Priority_Buffer_Test.cpp

Priority_Reactor_Test_SOURCES = Priority_Reactor_Test.cpp \
                                Priority_Reactor_Test.h

Priority_Task_Test_SOURCES = Priority_Task_Test.cpp

Proactor_Scatter_Gather_Test_SOURCES = Proactor_Scatter_Gather_Test.cpp

Proactor_Test_SOURCES = Proactor_Test.cpp

Proactor_Timer_Test_SOURCES = Proactor_Timer_Test.cpp

Process_Manager_Test_SOURCES = Process_Manager_Test.cpp

Process_Mutex_Test_SOURCES = Process_Mutex_Test.cpp

Process_Strategy_Test_SOURCES = Process_Strategy_Test.cpp \
                                Process_Strategy_Test.h

RB_Tree_Test_SOURCES = RB_Tree_Test.cpp RB_Tree_Test.h

Reactor_Dispatch_Order_Test_SOURCES = Reactor_Dispatch_Order_Test.cpp

Reactor_Exceptions_Test_SOURCES = Reactor_Exceptions_Test.cpp

Reactor_Notify_Test_SOURCES = Reactor_Notify_Test.cpp

Reactor_Performance_Test_SOURCES = Reactor_Performance_Test.cpp \
                                   Reactor_Performance_Test.h

Reactor_Registration_Test_SOURCES = Reactor_Registration_Test.cpp

Reactor_Timer_Test_SOURCES = Reactor_Timer_Test.cpp

Reactors_Test_SOURCES = Reactors_Test.cpp

Reader_Writer_Test_SOURCES = Reader_Writer_Test.cpp

Recursive_Condition_Test_SOURCES = Recursive_Condition_Test.cpp

Recursive_Mutex_Test_SOURCES = Recursive_Mutex_Test.cpp

Refcounted_Auto_Ptr_Test_SOURCES = Refcounted_Auto_Ptr_Test.cpp \
                                   Refcounted_Auto_Ptr_Test.h

Reference_Counted_Event_Handler_Test_SOURCES = \
  Reference_Counted_Event_Handler_Test.cpp

Reverse_Lock_Test_SOURCES = Reverse_Lock_Test.cpp

SOCK_Connector_Test_SOURCES = SOCK_Connector_Test.cpp

SOCK_Send_Recv_Test_SOURCES = SOCK_Send_Recv_Test.cpp

SOCK_Test_SOURCES = SOCK_Test.cpp

SPIPE_Test_SOURCES = SPIPE_Test.cpp

SString_Test_SOURCES = SString_Test.cpp

SV_Shared_Memory_Test_SOURCES = SV_Shared_Memory_Test.cpp

Semaphore_Test_SOURCES = Semaphore_Test.cpp

Service_Config_Test_SOURCES = Service_Config_Test.cpp
Service_Config_Test_DEPENDENCIES = libService_Config_DLL.la libTest_Output.la

Signal_Test_SOURCES = Signal_Test.cpp

Sigset_Ops_Test_SOURCES = Sigset_Ops_Test.cpp

Simple_Message_Block_Test_SOURCES = Simple_Message_Block_Test.cpp

Svc_Handler_Test_SOURCES = Svc_Handler_Test.cpp

TP_Reactor_Test_SOURCES = TP_Reactor_Test.cpp TP_Reactor_Test.h

TSS_Test_SOURCES = TSS_Test.cpp \
                   TSS_Test_Errno.h

TSS_Static_Test_SOURCES = TSS_Static_Test.cpp

Task_Test_SOURCES = Task_Test.cpp

Task_Ex_Test_SOURCES = Task_Ex_Test.cpp Task_Ex_Test.h

Thread_Manager_Test_SOURCES = Thread_Manager_Test.cpp

Thread_Mutex_Test_SOURCES = Thread_Mutex_Test.cpp

Thread_Pool_Reactor_Test_SOURCES = \
  Thread_Pool_Reactor_Test.cpp \
  Thread_Pool_Reactor_Test.h

Thread_Pool_Reactor_Resume_Test_SOURCES = \
  Thread_Pool_Reactor_Resume_Test.cpp \
  Thread_Pool_Reactor_Resume_Test.h

Thread_Pool_Test_SOURCES = Thread_Pool_Test.cpp

Time_Service_Test_SOURCES = Time_Service_Test.cpp

Time_Value_Test_SOURCES = Time_Value_Test.cpp

Timeprobe_Test_SOURCES = Timeprobe_Test.cpp

Timer_Cancellation_Test_SOURCES = Timer_Cancellation_Test.cpp

Timer_Queue_Reference_Counting_Test_SOURCES = \
  Timer_Queue_Reference_Counting_Test.cpp

Timer_Queue_Test_SOURCES = Timer_Queue_Test.cpp

TkReactor_Test_SOURCES = TkReactor_Test.cpp

Token_Strategy_Test_SOURCES = Token_Strategy_Test.cpp

Tokens_Test_SOURCES = Tokens_Test.cpp

UPIPE_SAP_Test_SOURCES = UPIPE_SAP_Test.cpp

Upgradable_RW_Test_SOURCES = Upgradable_RW_Test.cpp \
                             Upgradable_RW_Test.h

Unbounded_Set_Test_Ex_SOURCES = \
  Unbounded_Set_Test_Ex.cpp \
  Unbounded_Set_Test_Ex.h

UUIDTest_SOURCES = \
  UUIDTest.cpp

Vector_Test_SOURCES = Vector_Test.cpp

WFMO_Reactor_Test_SOURCES = WFMO_Reactor_Test.cpp

XtReactor_Test_SOURCES = XtReactor_Test.cpp
XtReactor_Test_LDADD = $(top_builddir)/ace/libACE.la @XTREACTOR_TEST_XLIBS@

## Source for conditionally built IPv6 tests.
if BUILD_IPV6
INET_Addr_Test_IPV6_SOURCES = INET_Addr_Test_IPV6.cpp
Max_Default_Port_Test_IPV6_SOURCES = Max_Default_Port_Test_IPV6.cpp
Multicast_Test_IPV6_SOURCES = Multicast_Test_IPV6.cpp
Multihomed_INET_Addr_Test_IPV6_SOURCES = Multihomed_INET_Addr_Test_IPV6.cpp
Proactor_Test_IPV6_SOURCES = Proactor_Test_IPV6.cpp
SOCK_Send_Recv_Test_IPV6_SOURCES = SOCK_Send_Recv_Test_IPV6.cpp
SOCK_Test_IPv6_SOURCES = SOCK_Test_IPv6.cpp
endif



## Additional test header files
noinst_HEADERS = test_config.h

## Architecture independent data files
noinst_DATA = \
  run_test.lst \
  \
  Config_Test.ini \
  Config_Test_Import_1.ini \
  \
  TkReactor_Test.tcl \
  \
  Service_Config_Test.UTF-16.conf \
  Service_Config_Test.conf \
  UNIXserver.conf \
  UNIXclerk.conf \
  UNIXtokens.conf \
  Win32server.conf \
  Win32clerk.conf \
  Win32tokens.conf


## The tests we want to run are all of the test programs we are going to build.
## Do not run them if they are cross-compiled.
if ACE_CROSS_COMPILED
TESTS =
else
##TESTS = $(check_PROGRAMS)
TESTS = $(noinst_PROGRAMS)
endif

## Set up the test environment
##TESTS_ENVIRONMENT =

EXTRA_DIST = Main.cpp $(noinst_SCRIPTS) $(noinst_DATA)

##DISTCLEANFILES = \
##  ace_pipe_name pattern \
##  $(tmp)/ace_temp_file* \
##  $(tmp)/ace_test_file \
##  $(tmp)/Naming_Test*

## Automake recognized target.  Appends these rules to the "all"
## target.
all-local:
	for p in $(noinst_SCRIPTS) $(noinst_DATA); do \
	  test -f $$p \
	  || $(LN_S) $(srcdir)/$$p $$p; done
	touch tests-link-stamp

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core core.*
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf Templates.DB gcctemp.c gcctemp so_locations *.ics
	-rm -rf templateregistry SunWS_cache ir.out

## Clean up files and directories created by this Makefile.
distclean-local:
	-test -f tests-link-stamp \
	   && rm $(noinst_SCRIPTS) $(noinst_DATA) tests-link-stamp
	-rm -rf log
