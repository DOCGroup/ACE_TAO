##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the entire ACE release
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.5

## Disable building of CLASSIX library for now.
## SUBDIRS = CLASSIX

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

# Define any X libraries that ACE needs, if any.
ACE_XLIBS = @ACE_XLIBS@


# We only want `gethrtime.cpp' if building on Pentium(tm) with GNU C++.
if ACE_ON_PENTIUM
ACE_GETHRTIME_CPP = gethrtime.cpp
else
ACE_GETHRTIME_CPP =
endif

# Define which ACE components to build
if BUILD_OS_FILES
LIBACE_OS = libACE_OS.la
else
LIBACE_OS =
endif

if BUILD_UTILS_FILES
LIBACE_UTILS = libACE_Utils.la
else
LIBACE_UTILS =
endif

if BUILD_LOGGING_FILES
LIBACE_LOGGING = libACE_Logging.la
else
LIBACE_LOGGING =
endif


if BUILD_THREADS_FILES
LIBACE_THREADS = libACE_Threads.la
else
LIBACE_THREADS =
endif

if BUILD_DEMUX_FILES
LIBACE_DEMUX = libACE_Demux.la
else
LIBACE_DEMUX =
endif

if BUILD_CONNECTION_FILES
LIBACE_CONNECTION = libACE_Connection.la
else
LIBACE_CONNECTION =
endif

if BUILD_SOCKETS_FILES
LIBACE_SOCKETS = libACE_Sockets.la
else
LIBACE_SOCKETS =
endif

if BUILD_IPC_FILES
LIBACE_IPC = libACE_IPC.la
else
LIBACE_IPC =
endif

if BUILD_SVCCONF_FILES
LIBACE_SVCCONF = libACE_Svcconf.la
else
LIBACE_SVCCONF =
#AM_CPPFLAGS += -DACE_LACKS_ACE_SVCCONF
endif

if BUILD_STREAMS_FILES
LIBACE_STREAMS = libACE_Streams.la
else
LIBACE_STREAMS =
endif

if BUILD_MEMORY_FILES
LIBACE_MEMORY = libACE_Memory.la
else
LIBACE_MEMORY =
endif

if BUILD_TIMER_FILES
LIBACE_TIMER = libACE_Timer.la
else
LIBACE_TIMER =
endif

if BUILD_TOKEN_FILES
LIBACE_TOKEN = libACE_Token.la
else
LIBACE_TOKEN =
#AM_CPPFLAGS += -DACE_LACKS_ACE_TOKEN
endif

if BUILD_OTHER_FILES
LIBACE_OTHER = libACE_Other.la
else
LIBACE_OTHER =
#AM_CPPFLAGS += -DACE_LACKS_ACE_OTHER
endif

if BUILD_FULL_LIBRARY
LIBACE = libACE.la
else
LIBACE =
endif

# Build a libtool library, libACE.la for installation in libdir.
lib_LTLIBRARIES = \
  $(LIBACE) \
  $(LIBACE_OS) \
  $(LIBACE_UTILS) \
  $(LIBACE_LOGGING) \
  $(LIBACE_THREADS) \
  $(LIBACE_DEMUX) \
  $(LIBACE_CONNECTION) \
  $(LIBACE_SOCKETS) \
  $(LIBACE_IPC) \
  $(LIBACE_SVCCONF) \
  $(LIBACE_STREAMS) \
  $(LIBACE_MEMORY) \
  $(LIBACE_TIMER) \
  $(LIBACE_TOKEN) \
  $(LIBACE_OTHER)

libACE_OS_la_SOURCES = \
  Basic_Types.cpp \
  OS.cpp \
  OS_Dirent.cpp \
  OS_Errno.cpp \
  OS_Memory.cpp \
  OS_String.cpp \
  OS_TLI.cpp \
  OS_Errno.cpp \
  Base_Thread_Adapter.cpp \
  OS_Thread_Adapter.cpp \
  OS_Log_Msg_Attributes.cpp \
  Thread_Hook.cpp \
  Sched_Params.cpp \
  Handle_Set.cpp

libACE_Utils_la_SOURCES = \
  ACE.cpp \
  Active_Map_Manager.cpp \
  Arg_Shifter.cpp \
  ARGV.cpp \
  Capabilities.cpp \
  Configuration.cpp \
  Configuration_Import_Export.cpp \
  Containers.cpp \
  Copy_Disabled.cpp \
  Dirent.cpp \
  Dirent_Selector.cpp \
  Dynamic.cpp \
  Filecache.cpp \
  Flag_Manip.cpp \
  Functor.cpp \
  Get_Opt.cpp \
  Hash_Map_Manager.cpp \
  Handle_Ops.cpp \
  Init_ACE.cpp \
  Lib_Find.cpp \
  Message_Block.cpp \
  Method_Request.cpp \
  Object_Manager.cpp \
  Registry.cpp \
  String_Base_Const.cpp \
  SString.cpp \
  Stats.cpp \
  Sample_History.cpp

libACE_Logging_la_SOURCES = \
  Dump.cpp \
  Log_Msg.cpp \
  Log_Msg_Callback.cpp \
  Log_Msg_Backend.cpp \
  Log_Msg_IPC.cpp \
  Log_Msg_NT_Event_Log.cpp \
  Log_Msg_UNIX_Event_Syslog.cpp \
  Log_Record.cpp \
  Logging_Strategy.cpp \
  Trace.cpp

libACE_Threads_la_SOURCES = \
  Activation_Queue.cpp \
  Process.cpp \
  Process_Manager.cpp \
  Synch.cpp \
  Synch_Options.cpp \
  Process_Semaphore.cpp \
  Process_Mutex.cpp \
  RW_Process_Mutex.cpp \
  File_Lock.cpp \
  Thread.cpp \
  Thread_Adapter.cpp \
  Thread_Manager.cpp \
  Thread_Exit.cpp \
  Thread_Control.cpp \
  Token.cpp

libACE_Demux_la_SOURCES = \
  Event_Handler.cpp \
  FlReactor.cpp \
  Msg_WFMO_Reactor.cpp \
  POSIX_Proactor.cpp \
  WIN32_Proactor.cpp \
  Priority_Reactor.cpp \
  Proactor.cpp \
  Reactor.cpp \
  Select_Reactor.cpp \
  Select_Reactor_Base.cpp \
  SUN_Proactor.cpp \
  TP_Reactor.cpp \
  TkReactor.cpp \
  WFMO_Reactor.cpp \
  XtReactor.cpp \
  QtReactor.cpp

libACE_Connection_la_SOURCES = \
  Asynch_IO.cpp \
  Asynch_IO_Impl.cpp \
  POSIX_Asynch_IO.cpp \
  Strategies.cpp

libACE_Sockets_la_SOURCES = \
  Addr.cpp \
  INET_ddr.cpp
  IPC_SAP.cpp \
  LSOCK.cpp \
  LSOCK_Acceptor.cpp \
  LSOCK_CODgram.cpp \
  LSOCK_Connector.cpp \
  LSOCK_Dgram.cpp \
  LSOCK_Stream.cpp \
  SOCK.cpp \
  SOCK_Acceptor.cpp \
  SOCK_CODgram.cpp \
  Sock_Connect.cpp \
  SOCK_Connector.cpp \
  SOCK_Dgram.cpp \
  SOCK_Dgram_Bcast.cpp \
  SOCK_Dgram_Mcast.cpp \
  SOCK_IO.cpp \
  SOCK_Stream.cpp

libACE_IPC_la_SOURCES = \
  ATM_Addr.cpp \
  ATM_Acceptor.cpp \
  ATM_Connector.cpp \
  ATM_Params.cpp \
  ATM_QoS.cpp \
  ATM_Stream.cpp \
  DEV.cpp \
  DEV_Addr.cpp \
  DEV_Connector.cpp \
  DEV_IO.cpp \
  FIFO.cpp \
  FIFO_Recv.cpp \
  FIFO_Recv_Msg.cpp \
  FIFO_Send.cpp \
  FIFO_Send_Msg.cpp \
  FILE_Addr.cpp \
  FILE.cpp \
  FILE_Connector.cpp \
  FILE_IO.cpp \
  IO_SAP.cpp \
  IOStream.cpp \
  Pipe.cpp \
  Signal.cpp \
  SPIPE_Addr.cpp \
  SPIPE.cpp \
  SPIPE_Acceptor.cpp \
  SPIPE_Connector.cpp \
  SPIPE_Stream.cpp \
  SV_Message.cpp \
  SV_Message_Queue.cpp \
  SV_Semaphore_Complex.cpp \
  SV_Semaphore_Simple.cpp \
  SV_Shared_Memory.cpp \
  TLI.cpp \
  TLI_Acceptor.cpp \
  TLI_Connector.cpp \
  TLI_Stream.cpp \
  TTY_IO.cpp \
  UNIX_Addr.cpp \
  UPIPE_Acceptor.cpp \
  UPIPE_Connector.cpp \
  UPIPE_Stream.cpp \
  XTI_ATM_Mcast.cpp \
  MEM_Acceptor.cpp \
  MEM_Addr.cpp \
  MEM_Connector.cpp \
  MEM_IO.cpp \
  MEM_SAP.cpp \
  MEM_Stream.cpp

libACE_Svcconf_la_SOURCES = \
  DLL.cpp \
  Dynamic_Service_Base.cpp \
  Parse_Node.cpp \
  Service_Config.cpp \
  Service_Manager.cpp \
  Service_Object.cpp \
  Service_Repository.cpp \
  Service_Types.cpp \
  Service_Templates.cpp \
  Shared_Object.cpp \
  Svc_Conf_l.cpp \
  Svc_Conf_y.cpp \
  Svc_Conf_Lexer_Guard.cpp

libACE_Streams_la_SOURCES = \
  CDR_Base.cpp \
  CDR_Stream.cpp \
  Codeset_IBM1047.cpp \
  Message_Queue.cpp \
  Task.cpp

libACE_Memory_la_SOURCES = \
  Based_Pointer_Repository.cpp \
  Malloc.cpp \
  PI_Malloc.cpp \
  Malloc_Allocator.cpp \
  Malloc_Instantiations.cpp \
  Mem_Map.cpp \
  Memory_Pool.cpp \
  Obchunk.cpp \
  Obstack.cpp \
  Read_Buffer.cpp \
  Shared_Memory.cpp \
  Shared_Memory_MM.cpp \
  Shared_Memory_SV.cpp

libACE_Token_la_SOURCES = \
  Local_Tokens.cpp \
  Remote_Tokens.cpp \
  Token_Collection.cpp \
  Token_Invariants.cpp \
  Token_Manager.cpp \
  Token_Request_Reply.cpp

libACE_Timer_la_SOURCES = \
  Basic_Stats.cpp \
  High_Res_Timer.cpp \
  Profile_Timer.cpp \
  System_Time.cpp \
  Time_Request_Reply.cpp \
  Timeprobe.cpp \
  Timer_Hash.cpp \
  Timer_Heap.cpp \
  Timer_List.cpp \
  Timer_Queue.cpp \
  Timer_Wheel.cpp \
  $(ACE_GETHRTIME_CPP)

# We only want `gethrtime.cpp' if building on Pentium(tm) so we need
# to let Automake know that `gethrtime.cpp' is conditionally built.
EXTRA_libACE_Timer_la_SOURCES = gethrtime.cpp

libACE_Other_la_SOURCES = \
  Local_Name_Space.cpp \
  Name_Proxy.cpp \
  Name_Request_Reply.cpp \
  Name_Space.cpp \
  Naming_Context.cpp \
  Registry_Name_Space.cpp \
  Remote_Name_Space.cpp

libACE_la_SOURCES = \
  $(libACE_OS_la_SOURCES) \
  $(libACE_Utils_la_SOURCES) \
  $(libACE_Logging_la_SOURCES) \
  $(libACE_Threads_la_SOURCES) \
  $(libACE_Demux_la_SOURCES) \
  $(libACE_Connection_la_SOURCES) \
  $(libACE_Sockets_la_SOURCES) \
  $(libACE_IPC_la_SOURCES) \
  $(libACE_Svcconf_la_SOURCES) \
  $(libACE_Streams_la_SOURCES) \
  $(libACE_Memory_la_SOURCES) \
  $(libACE_Timer_la_SOURCES) \
  $(libACE_Token_la_SOURCES) \
  $(libACE_Other_la_SOURCES)


## The following is an excerpt from the "libtool" manual, written by
## Gordon Matzigkeit:
##
## This flag accepts an argument of the form `current[:revision[:age]]'. So,
## passing `-version-info 3:12:1' sets current to 3, revision to 12, and age
## to 1.
## If either revision or age are omitted, they default to 0. Also note that
## age must be less than or equal to the current interface number.
##
## Here are a set of rules to help you update your library version
## information:
##
##     1.Start with version information of `0:0:0' for each libtool library.
##     2.Update the version information only immediately before a public
##       release of your software. More frequent updates are unnecessary, and
##       only guarantee that the current interface number gets larger faster.
##     3.If the library source code has changed at all since the last update,
##       then increment revision (`c:r:a' becomes `c:r+1:a').
##     4.If any interfaces have been added, removed, or changed since the last
##       update, increment current, and set revision to 0.
##     5.If any interfaces have been added since the last public release, then
##       increment age.
##     6.If any interfaces have been removed since the last public release,
##       then set age to 0.
##
## Never try to set the interface numbers so that they correspond to the
## release number of your package. This is an abuse that only fosters
## misunderstanding of the purpose of library versions. Instead, use the
## `-release' flag (see section 6.4 Managing release information), but be
## warned that every release of your package will not be binary compatibility
## with any other release.

## The below "-version-info" isn't being used the way libtool intends
## it to be used but we do it that way to make the version numbers
## that ACE uses match those created by libtool from "-version-info."
##libACE_la_LDFLAGS = $(X_LIBS) \
##      -version-info 10:4:6   ## 10-6=4 --> libACE.so.4.6.4
libACE_la_LDFLAGS = $(X_LIBS) \
  -version-info @ACE_CURRENT@:@ACE_REVISION@:@ACE_AGE@

# We also add any X libraries to the list.
libACE_la_LIBADD = $(X_PRE_LIBS) $(ACE_XLIBS) $(X_EXTRA_LIBS)

## These are template source files.
TEMPLATE_FILES = \
  Acceptor.cpp \
  Active_Map_Manager_T.cpp \
  Array_Base.cpp \
  Node.cpp \
  Unbounded_Set.cpp \
  Unbounded_Queue.cpp \
  Asynch_Acceptor.cpp \
  Auto_IncDec_T.cpp \
  Auto_Ptr.cpp \
  Based_Pointer_T.cpp \
  Bound_Ptr.cpp \
  Connector.cpp \
  Containers_T.cpp \
  Cache_Map_Manager_T.cpp \
  Cached_Connect_Strategy_T.cpp \
  Caching_Strategies_T.cpp \
  Caching_Utility_T.cpp \
  Cleanup_Strategies_T.cpp \
  Dump_T.cpp \
  Dynamic_Service.cpp \
  Env_Value_T.cpp \
  Event_Handler_T.cpp \
  Free_List.cpp \
  Functor_T.cpp \
  Future.cpp \
  Future_Set.cpp \
  Hash_Map_Manager_T.cpp \
  Hash_Map_With_Allocator_T.cpp \
  Hash_Cache_Map_Manager_T.cpp \
  IOStream_T.cpp \
  Intrusive_List.cpp \
  Intrusive_List_Node.cpp \
  LOCK_SOCK_Acceptor.cpp \
  Local_Name_Space_T.cpp \
  Malloc_T.cpp \
  Managed_Object.cpp \
  Map_Manager.cpp \
  Map_T.cpp \
  Message_Block_T.cpp \
  Message_Queue_T.cpp \
  Module.cpp \
  Obstack_T.cpp \
  Pair_T.cpp \
  Refcounted_Auto_Ptr.cpp \
  RB_Tree.cpp \
  Select_Reactor_T.cpp \
  Singleton.cpp \
  Strategies_T.cpp \
  Stream.cpp \
  Stream_Modules.cpp \
  String_Base.cpp \
  Svc_Handler.cpp \
  Synch_T.cpp \
  Task_T.cpp \
  Template_Instantiations.cpp \
  Test_and_Set.cpp \
  Timeprobe_T.cpp \
  Timer_Hash_T.cpp \
  Timer_Heap_T.cpp \
  Timer_List_T.cpp \
  Timer_Queue_Adapters.cpp \
  Timer_Queue_T.cpp \
  Timer_Wheel_T.cpp \
  Typed_SV_Message.cpp \
  Typed_SV_Message_Queue.cpp

HEADER_FILES = \
  ACE.h \
  ACE_export.h \
  ARGV.h \
  ATM_Acceptor.h \
  ATM_Addr.h \
  ATM_Connector.h \
  ATM_Params.h \
  ATM_QoS.h \
  ATM_Stream.h \
  Acceptor.h \
  Activation_Queue.h \
  Active_Map_Manager.h \
  Active_Map_Manager_T.h \
  Addr.h \
  Arg_Shifter.h \
  Array.h \
  Array_Base.h \
  Asynch_Acceptor.h \
  Asynch_IO.h \
  Asynch_IO_Impl.h \
  Auto_IncDec_T.h \
  Auto_Ptr.h \
  Base_Thread_Adapter.h \
  Based_Pointer_Repository.h \
  Based_Pointer_T.h \
  Basic_Stats.h \
  Basic_Types.h \
  Bound_Ptr.h \
  CDR_Base.h \
  CDR_Stream.h \
  CORBA_macros.h \
  Cache_Map_Manager_T.h \
  Cached_Connect_Strategy_T.h \
  Caching_Strategies_T.h \
  Caching_Utility_T.h \
  Capabilities.h \
  Cleanup_Strategies_T.h \
  Codeset_IBM1047.h \
  Configuration.h \
  Configuration_Import_Export.h \
  Connector.h \
  Containers.h \
  Containers_T.h \
  Copy_Disabled.h \
  DEV.h \
  DEV_Addr.h \
  DEV_Connector.h \
  DEV_IO.h \
  DLL.h \
  Date_Time.h \
  Dirent.h \
  Dirent_Selector.h \
  Dump.h \
  Dump_T.h \
  Dynamic.h \
  Dynamic_Service.h \
  Dynamic_Service_Base.h \
  Env_Value_T.h \
  Event_Handler.h \
  Event_Handler_T.h \
  FIFO.h \
  FIFO_Recv.h \
  FIFO_Recv_Msg.h \
  FIFO_Send.h \
  FIFO_Send_Msg.h \
  FILE.h \
  FILE_Addr.h \
  FILE_Connector.h \
  FILE_IO.h \
  File_Lock.h \
  Filecache.h \
  FlReactor.h \
  Flag_Manip.h \
  Free_List.h \
  Functor.h \
  Functor_T.h \
  Future.h \
  Future_Set.h \
  Get_Opt.h \
  Handle_Gobbler.h \
  Handle_Ops.h \
  Handle_Set.h \
  Hash_Cache_Map_Manager_T.h \
  Hash_Map_Manager.h \
  Hash_Map_Manager_T.h \
  Hash_Map_With_Allocator_T.h \
  High_Res_Timer.h \
  INET_Addr.h \
  IOStream.h \
  IOStream_T.h \
  IO_Cntl_Msg.h \
  IO_SAP.h \
  IPC_SAP.h \
  Init_ACE.h \
  Intrusive_List.h \
  Intrusive_List_Node.h \
  LOCK_SOCK_Acceptor.h \
  LSOCK.h \
  LSOCK_Acceptor.h \
  LSOCK_CODgram.h \
  LSOCK_Connector.h \
  LSOCK_Dgram.h \
  LSOCK_Stream.h \
  Lib_Find.h \
  Local_Name_Space.h \
  Local_Name_Space_T.h \
  Local_Tokens.h \
  Log_Msg.h \
  Log_Msg_Backend.h \
  Log_Msg_Callback.h \
  Log_Msg_IPC.h \
  Log_Msg_NT_Event_Log.h \
  Log_Msg_UNIX_Syslog.h \
  Log_Priority.h \
  Log_Record.h \
  Logging_Strategy.h \
  MEM_Acceptor.h \
  MEM_Addr.h \
  MEM_Connector.h \
  MEM_IO.h \
  MEM_SAP.h \
  MEM_Stream.h \
  Malloc.h \
  Malloc_Allocator.h \
  Malloc_Base.h \
  Malloc_T.h \
  Managed_Object.h \
  Map.h \
  Map_Manager.h \
  Map_T.h \
  Mem_Map.h \
  Memory_Pool.h \
  Message_Block.h \
  Message_Block_T.h \
  Message_Queue.h \
  Message_Queue_T.h \
  Method_Object.h \
  Method_Request.h \
  Min_Max.h \
  Module.h \
  Msg_WFMO_Reactor.h \
  Multiplexor.h \
  NT_Service.h \
  Name_Proxy.h \
  Name_Request_Reply.h \
  Name_Space.h \
  Naming_Context.h \
  Node.h \
  OS.h \
  OS_Dirent.h \
  OS_Errno.h \
  OS_Export.h \
  OS_Log_Msg_Attributes.h \
  OS_Memory.h \
  OS_String.h \
  OS_TLI.h \
  OS_Thread_Adapter.h \
  Obchunk.h \
  Object_Manager.h \
  Obstack.h \
  Obstack_T.h \
  PI_Malloc.h \
  POSIX_Asynch_IO.h \
  POSIX_Proactor.h \
  Pair.h \
  Pair_T.h \
  Parse_Node.h \
  Pipe.h \
  Priority_Reactor.h \
  Proactor.h \
  Proactor_Impl.h \
  Process.h \
  Process_Manager.h \
  Process_Mutex.h \
  Process_Semaphore.h \
  Profile_Timer.h \
  QtReactor.h \
  RB_Tree.h \
  RW_Process_Mutex.h \
  Reactor.h \
  Reactor_Impl.h \
  Read_Buffer.h \
  Refcounted_Auto_Ptr.h \
  Registry.h \
  Registry_Name_Space.h \
  Remote_Name_Space.h \
  Remote_Tokens.h \
  SOCK.h \
  SOCK_Acceptor.h \
  SOCK_CODgram.h \
  SOCK_Connector.h \
  SOCK_Dgram.h \
  SOCK_Dgram_Bcast.h \
  SOCK_Dgram_Mcast.h \
  SOCK_IO.h \
  SOCK_Stream.h \
  SPIPE.h \
  SPIPE_Acceptor.h \
  SPIPE_Addr.h \
  SPIPE_Connector.h \
  SPIPE_Stream.h \
  String_Base_Const.h \
  String_Base.h \
  SString.h \
  SUN_Proactor.h \
  SV_Message.h \
  SV_Message_Queue.h \
  SV_Semaphore_Complex.h \
  SV_Semaphore_Simple.h \
  SV_Shared_Memory.h \
  Sample_History.h \
  Sched_Params.h \
  Select_Reactor.h \
  Select_Reactor_Base.h \
  Select_Reactor_T.h \
  Service_Config.h \
  Service_Loader.h \
  Service_Manager.h \
  Service_Object.h \
  Service_Repository.h \
  Service_Templates.h \
  Service_Types.h \
  Shared_Memory.h \
  Shared_Memory_MM.h \
  Shared_Memory_SV.h \
  Shared_Object.h \
  Signal.h \
  Singleton.h \
  Sock_Connect.h \
  Stats.h \
  Strategies.h \
  Strategies_T.h \
  Stream.h \
  Stream_Modules.h \
  Svc_Conf.h \
  Svc_Conf_Lexer_Guard.h \
  Svc_Conf_Tokens.h \
  Svc_Handler.h \
  Synch.h \
  Synch_Options.h \
  Synch_T.h \
  System_Time.h \
  TLI.h \
  TLI_Acceptor.h \
  TLI_Connector.h \
  TLI_Stream.h \
  TP_Reactor.h \
  TTY_IO.h \
  Task.h \
  Task_T.h \
  Test_and_Set.h \
  Thread.h \
  Thread_Adapter.h \
  Thread_Control.h \
  Thread_Exit.h \
  Thread_Hook.h \
  Thread_Manager.h \
  Time_Request_Reply.h \
  Time_Value.h \
  Timeprobe.h \
  Timeprobe_T.h \
  Timer_Hash.h \
  Timer_Hash_T.h \
  Timer_Heap.h \
  Timer_Heap_T.h \
  Timer_List.h \
  Timer_List_T.h \
  Timer_Queue.h \
  Timer_Queue_Adapters.h \
  Timer_Queue_T.h \
  Timer_Wheel.h \
  Timer_Wheel_T.h \
  TkReactor.h \
  Token.h \
  Token_Collection.h \
  Token_Invariants.h \
  Token_Manager.h \
  Token_Request_Reply.h \
  Trace.h \
  Typed_SV_Message.h \
  Typed_SV_Message_Queue.h \
  UNIX_Addr.h \
  UPIPE_Acceptor.h \
  UPIPE_Addr.h \
  UPIPE_Connector.h \
  UPIPE_Stream.h \
  Unbounded_Queue.h \
  Unbounded_Set.h \
  Version.h \
  WFMO_Reactor.h \
  WIN32_Asynch_IO.h \
  WIN32_Proactor.h \
  XTI_ATM_Mcast.h \
  XtReactor.h \
  ace_wchar.h \
  config-all.h \
  config.h \
  iosfwd.h \
  post.h \
  pre.h \
  streams.h \
  svc_export.h
## Make sure config.h and config-all.h are in the above header list!

INLINE_FILES = \
  ACE.i \
  ARGV.i \
  ATM_Acceptor.i \
  ATM_Addr.i \
  ATM_Connector.i \
  ATM_Params.i \
  ATM_QoS.i \
  ATM_Stream.i \
  Activation_Queue.i \
  Active_Map_Manager.i \
  Active_Map_Manager_T.i \
  Addr.i \
  Asynch_IO_Impl.i \
  Atomic_Op.i \
  Auto_IncDec_T.i \
  Auto_Ptr.i \
  Based_Pointer_T.i \
  Basic_Types.i \
  Bound_Ptr.i \
  CDR_Stream.i \
  Cache_Map_Manager_T.i \
  Caching_Strategies_T.i \
  Capabilities.i \
  Containers.i \
  Containers_T.i \
  DEV.i \
  DEV_Addr.i \
  DEV_Connector.i \
  DEV_IO.i \
  Date_Time.i \
  Dirent.i \
  Dynamic.i \
  Dynamic_Service.i \
  Dynamic_Service_Base.i \
  Env_Value_T.i \
  Event_Handler.i \
  Event_Handler_T.i \
  FIFO.i \
  FIFO_Recv.i \
  FIFO_Recv_Msg.i \
  FIFO_Send.i \
  FIFO_Send_Msg.i \
  FILE.i \
  FILE_Addr.i \
  FILE_Connector.i \
  FILE_IO.i \
  FlReactor.i \
  Flag_Manip.i \
  Free_List.i \
  Functor.i \
  Functor_T.i \
  Get_Opt.i \
  Handle_Gobbler.i \
  Handle_Ops.i \
  Handle_Set.i \
  Hash_Cache_Map_Manager_T.i \
  Hash_Map_Manager_T.i \
  Hash_Map_With_Allocator_T.i \
  High_Res_Timer.i \
  INET_Addr.i \
  IOStream_T.i \
  IO_SAP.i \
  IPC_SAP.i \
  Init_ACE.i \
  LSOCK.i \
  LSOCK_CODgram.i \
  LSOCK_Connector.i \
  LSOCK_Dgram.i \
  LSOCK_Stream.i \
  Lib_Find.i \
  Local_Tokens.i \
  Log_Record.i \
  MEM_Acceptor.i \
  MEM_Addr.i \
  MEM_Connector.i \
  MEM_IO.i \
  MEM_SAP.i \
  MEM_Stream.i \
  Malloc.i \
  Malloc_Allocator.i \
  Malloc_T.i \
  Managed_Object.i \
  Map_Manager.i \
  Map_T.i \
  Mem_Map.i \
  Memory_Pool.i \
  Message_Block.i \
  Message_Block_T.i \
  Message_Queue.i \
  Message_Queue_T.i \
  Module.i \
  Msg_WFMO_Reactor.i \
  Multiplexor.i \
  NT_Service.i \
  OS.i \
  Obchunk.i \
  Object_Manager.i \
  Obstack_T.i \
  PI_Malloc.i \
  POSIX_Asynch_IO.i \
  POSIX_Proactor.i \
  Pair_T.i \
  Parse_Node.i \
  Pipe.i \
  Proactor.i \
  Process.i \
  Process_Manager.i \
  Profile_Timer.i \
  RB_Tree.i \
  Reactor.i \
  Read_Buffer.i \
  Refcounted_Auto_Ptr.i \
  Remote_Tokens.i \
  SOCK.i \
  SOCK_Acceptor.i \
  SOCK_CODgram.i \
  SOCK_Connector.i \
  SOCK_Dgram.i \
  SOCK_Dgram_Bcast.i \
  SOCK_Dgram_Mcast.i \
  SOCK_IO.i \
  SOCK_Stream.i \
  SPIPE.i \
  SPIPE_Addr.i \
  SPIPE_Connector.i \
  SPIPE_Stream.i \
  String_Base.i \
  SString.i \
  SUN_Proactor.i \
  SV_Message.i \
  SV_Message_Queue.i \
  SV_Semaphore_Complex.i \
  SV_Semaphore_Simple.i \
  SV_Shared_Memory.i \
  Sched_Params.i \
  Select_Reactor_Base.i \
  Select_Reactor_T.i \
  Service_Config.i \
  Service_Loader.i \
  Service_Manager.i \
  Service_Object.i \
  Service_Repository.i \
  Service_Types.i \
  Shared_Memory_MM.i \
  Shared_Memory_SV.i \
  Shared_Object.i \
  Signal.i \
  Singleton.i \
  Sock_Connect.i \
  Stats.i \
  Strategies.i \
  Strategies_T.i \
  Stream.i \
  Synch.i \
  Synch_Options.i \
  Synch_T.i \
  TLI.i \
  TLI_Connector.i \
  TLI_Stream.i \
  TP_Reactor.i \
  Task.i \
  Task_T.i \
  Test_and_Set.i \
  Thread.i \
  Thread_Manager.i \
  Timeprobe.i \
  Timer_Queue_Adapters.i \
  Timer_Queue_T.i \
  Token.i \
  Token_Collection.i \
  Token_Manager.i \
  Token_Request_Reply.i \
  Typed_SV_Message.i \
  Typed_SV_Message_Queue.i \
  UNIX_Addr.i \
  UPIPE_Acceptor.i \
  UPIPE_Connector.i \
  UPIPE_Stream.i \
  WFMO_Reactor.i \
  XTI_ATM_Mcast.i \
  Array_Base.inl \
  Base_Thread_Adapter.inl \
  Basic_Stats.inl \
  CDR_Base.inl \
  Dirent_Selector.inl \
  File_Lock.inl \
  Intrusive_List.inl \
  Intrusive_List_Node.inl \
  OS_Dirent.inl \
  OS_Errno.inl \
  OS_Log_Msg_Attributes.inl \
  OS_Memory.inl \
  OS_String.inl \
  OS_TLI.inl \
  OS_Thread_Adapter.inl \
  Process_Mutex.inl \
  Process_Semaphore.inl \
  RW_Process_Mutex.inl \
  Sample_History.inl \
  Thread_Adapter.inl \
  Thread_Control.inl \
  Unbounded_Queue.inl \
  Unbounded_Set.inl \
  ace_wchar.inl

## It would be good to remove pkgincludedir.  However, we want to install
## headers in "$(prefix)/ace" not "$(prefix)/ACE" because the source files
## include files in the directory "ace."  By default pkgincludedir would
## be "$(prefix)/$(PACKAGE)" which would be "$(prefix)/ACE" in our case.
## It is for this reason that we must redefine "pkgincludedir."
## However, if we set the package to "ace" instead of "ACE" then we won't
## need to set the "pkgincludedir."
##        -Ossama
##pkgincludedir = $(includedir)/ace
pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES) \
  $(TEMPLATE_FILES)

## Make sure the following get into the distribution
##EXTRA_DIST = $(TEMPLATE_FILES)

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
