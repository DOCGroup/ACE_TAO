##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the entire ACE release
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## These are subdirectories that are always included in the
## distribution.
DIST_SUBDIRS = \
  RMCast \
  QoS \
  SSL

## Conditionally built subdirectories.

if BUILD_RMCAST
RMCAST_DIR = RMCast
else
RMCAST_DIR =
endif

if BUILD_QOS
QOS_DIR = QoS
else
QOS_DIR =
endif

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
##
## Disable building of CLASSIX library for now.
## SUBDIRS = CLASSIX
SUBDIRS = \
  . \
  $(RMCAST_DIR) \
  $(QOS_DIR) \
  $(SSL_DIR)


AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

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

if BUILD_CODECS_FILES
LIBACE_CODECS = libACE_Codecs.la
else
LIBACE_CODECS =
#AM_CPPFLAGS += -DACE_LACKS_ACE_CODECS
endif


if BUILD_OTHER_FILES
LIBACE_OTHER = libACE_Other.la
else
LIBACE_OTHER =
#AM_CPPFLAGS += -DACE_LACKS_ACE_OTHER
endif

if BUILD_UUID_FILES
LIBACE_UUID = libACE_UUID.la
else
LIBACE_UUID =
#AM_CPPFLAGS += -DACE_LACKS_ACE_UUID
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
  $(LIBACE_CODECS) \
  $(LIBACE_OTHER) \
  $(LIBACE_UUID)

libACE_OS_la_SOURCES = \
  ARGV.cpp \
  Argv_Type_Converter.cpp \
  Basic_Types.cpp \
  Time_Value.cpp \
  Cleanup.cpp \
  Object_Manager_Base.cpp \
  OS_NS_arpa_inet.cpp \
  OS_NS_ctype.cpp \
  OS_NS_dirent.cpp \
  OS_NS_dlfcn.cpp \
  OS_NS_errno.cpp \
  OS_NS_fcntl.cpp \
  OS_NS_math.cpp \
  OS_NS_netdb.cpp \
  OS_NS_poll.cpp \
  OS_NS_pwd.cpp \
  OS_NS_regex.cpp \
  OS_NS_signal.cpp \
  OS_NS_stdio.cpp \
  OS_NS_stdlib.cpp \
  OS_NS_string.cpp \
  OS_NS_strings.cpp \
  OS_NS_stropts.cpp \
  OS_NS_sys_mman.cpp \
  OS_NS_sys_msg.cpp \
  OS_NS_sys_resource.cpp \
  OS_NS_sys_select.cpp \
  OS_NS_sys_shm.cpp \
  OS_NS_sys_socket.cpp \
  OS_NS_sys_stat.cpp \
  OS_NS_sys_time.cpp \
  OS_NS_sys_uio.cpp \
  OS_NS_sys_utsname.cpp \
  OS_NS_sys_wait.cpp \
  OS_NS_Thread.cpp \
  OS_NS_time.cpp \
  OS_NS_unistd.cpp \
  OS_NS_wchar.cpp \
  OS_QoS.cpp \
  OS_TLI.cpp \
  OS_Errno.cpp \
  Base_Thread_Adapter.cpp \
  OS_Thread_Adapter.cpp \
  OS_Log_Msg_Attributes.cpp \
  Thread_Hook.cpp \
  Sched_Params.cpp \
  Handle_Set.cpp \
  Copy_Disabled.cpp

libACE_Utils_la_SOURCES = \
  ACE.cpp \
  ACE_crc32.cpp \
  ACE_crc_ccitt.cpp \
  Active_Map_Manager.cpp \
  Arg_Shifter.cpp \
  Capabilities.cpp \
  Containers.cpp \
  Configuration.cpp \
  Configuration_Import_Export.cpp \
  Date_Time.cpp \
  Dirent.cpp \
  Dirent_Selector.cpp \
  Dynamic.cpp \
  Filecache.cpp \
  Flag_Manip.cpp \
  Framework_Component.cpp \
  Functor.cpp \
  Functor_String.cpp \
  Get_Opt.cpp \
  Hash_Map_Manager.cpp \
  Connection_Recycling_Strategy.cpp \
  Hashable.cpp \
  Notification_Strategy.cpp \
  Recyclable.cpp \
  Refcountable.cpp \
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
  Container_Instantiations.cpp \
  Sample_History.cpp \
  Template_Instantiations.cpp

libACE_Logging_la_SOURCES = \
  Dump.cpp \
  Log_Msg.cpp \
  Log_Msg_Callback.cpp \
  Log_Msg_Backend.cpp \
  Log_Msg_IPC.cpp \
  Log_Msg_NT_Event_Log.cpp \
  Log_Msg_UNIX_Syslog.cpp \
  Log_Record.cpp \
  Logging_Strategy.cpp \
  Trace.cpp

libACE_Threads_la_SOURCES = \
  Activation_Queue.cpp \
  Atomic_Op.cpp \
  Process.cpp \
  Process_Manager.cpp \
  TSS_Adapter.cpp \
  Auto_Event.cpp \
  Manual_Event.cpp \
  Barrier.cpp \
  Condition_Thread_Mutex.cpp \
  Condition_Recursive_Thread_Mutex.cpp \
  Event.cpp \
  Lock.cpp \
  Mutex.cpp \
  RW_Mutex.cpp \
  RW_Thread_Mutex.cpp \
  Recursive_Thread_Mutex.cpp \
  Semaphore.cpp \
  Thread_Mutex.cpp \
  Thread_Semaphore.cpp \
  Synch_Options.cpp \
  Process_Semaphore.cpp \
  Process_Mutex.cpp \
  RW_Process_Mutex.cpp \
  File_Lock.cpp \
  Thread.cpp \
  Thread_Manager.cpp \
  Thread_Adapter.cpp \
  Thread_Exit.cpp \
  Thread_Control.cpp \
  Token.cpp

libACE_Demux_la_SOURCES = \
  Event_Handler.cpp \
  FlReactor.cpp \
  Msg_WFMO_Reactor.cpp \
  POSIX_Proactor.cpp \
  POSIX_CB_Proactor.cpp \
  WIN32_Proactor.cpp \
  Priority_Reactor.cpp \
  Proactor.cpp \
  Reactor.cpp \
  Select_Reactor.cpp \
  Select_Reactor_Base.cpp \
  SUN_Proactor.cpp \
  Dev_Poll_Reactor.cpp \
  TP_Reactor.cpp \
  TkReactor.cpp \
  WFMO_Reactor.cpp \
  XtReactor.cpp \
  QtReactor.cpp

libACE_Connection_la_SOURCES = \
  Asynch_IO.cpp \
  Asynch_IO_Impl.cpp \
  Asynch_Pseudo_Task.cpp \
  POSIX_Asynch_IO.cpp \
  WIN32_Asynch_IO.cpp

libACE_Sockets_la_SOURCES = \
  Addr.cpp \
  INET_Addr.cpp \
  Multihomed_INET_Addr.cpp \
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
  SOCK_SEQPACK_Acceptor.cpp \
  SOCK_SEQPACK_Association.cpp \
  SOCK_SEQPACK_Connector.cpp \
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
  DLL_Manager.cpp \
  Dynamic_Service_Base.cpp \
  Parse_Node.cpp \
  Service_Config.cpp \
  Service_Manager.cpp \
  Service_Object.cpp \
  Service_Repository.cpp \
  Service_Types.cpp \
  Service_Templates.cpp \
  Shared_Object.cpp \
  XML_Svc_Conf.cpp \
  Svc_Conf_l.cpp \
  Svc_Conf_y.cpp \
  Svc_Conf_Lexer_Guard.cpp

libACE_Streams_la_SOURCES = \
  CDR_Base.cpp \
  CDR_Stream.cpp \
  Codeset_IBM1047.cpp \
  Codeset_Registry.cpp \
  Codeset_Registry_db.cpp \
  Message_Queue.cpp \
  Reactor_Notification_Strategy.cpp \
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

libACE_Codecs_la_SOURCES = \
  Codecs.cpp

libACE_Other_la_SOURCES = \
  Local_Name_Space.cpp \
  Name_Proxy.cpp \
  Name_Request_Reply.cpp \
  Name_Space.cpp \
  Naming_Context.cpp \
  Registry_Name_Space.cpp \
  Remote_Name_Space.cpp

libACE_UUID_la_SOURCES = \
  UUID.cpp

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
  $(libACE_Codecs_la_SOURCES) \
  $(libACE_Other_la_SOURCES) \
  $(libACE_UUID_la_SOURCES)


## ACE's versioning scheme is not compatible with libtool's scheme so
## use "-version-number", instead of "-version-info", to override it.
libACE_la_LDFLAGS = $(X_LIBS) \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Codecs_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Connection_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Demux_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_IPC_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Logging_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Memory_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_OS_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Other_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Sockets_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Streams_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Svcconf_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Threads_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Timer_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Token_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_Utils_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

libACE_UUID_la_LDFLAGS = \
  -version-number @ACE_MAJOR@:@ACE_MINOR@:@ACE_BETA@

# We also add any X libraries to the list.
libACE_la_LIBADD = $(X_PRE_LIBS) $(ACE_XLIBS) $(X_EXTRA_LIBS)

## These are template source files.
TEMPLATE_FILES = \
  Acceptor.cpp \
  Active_Map_Manager_T.cpp \
  Array_Base.cpp \
  Asynch_Acceptor.cpp \
  Asynch_Connector.cpp \
  Atomic_Op_T.cpp \
  Auto_IncDec_T.cpp \
  Auto_Ptr.cpp \
  Based_Pointer_T.cpp \
  Cached_Connect_Strategy_T.cpp \
  Cache_Map_Manager_T.cpp \
  Caching_Strategies_T.cpp \
  Caching_Utility_T.cpp \
  Cleanup_Strategies_T.cpp \
  Condition_T.cpp \
  Connector.cpp \
  Containers_T.cpp \
  Dump_T.cpp \
  Dynamic_Service.cpp \
  Env_Value_T.cpp \
  Event_Handler_T.cpp \
  Framework_Component_T.cpp \
  Free_List.cpp \
  Functor_T.cpp \
  Future.cpp \
  Future_Set.cpp \
  Guard_T.cpp \
  Hash_Cache_Map_Manager_T.cpp \
  Hash_Map_Manager_T.cpp \
  Hash_Map_With_Allocator_T.cpp \
  Intrusive_List.cpp \
  Intrusive_List_Node.cpp \
  IOStream_T.cpp \
  Local_Name_Space_T.cpp \
  Lock_Adapter_T.cpp \
  LOCK_SOCK_Acceptor.cpp \
  Malloc_T.cpp \
  Managed_Object.cpp \
  Map_Manager.cpp \
  Map_T.cpp \
  Message_Block_T.cpp \
  Message_Queue_T.cpp \
  Metrics_Cache_T.cpp \
  Module.cpp \
  Node.cpp \
  Obstack_T.cpp \
  Pair_T.cpp \
  RB_Tree.cpp \
  Reverse_Lock_T.cpp \
  Select_Reactor_T.cpp \
  Singleton.cpp \
  Strategies_T.cpp \
  Stream.cpp \
  Stream_Modules.cpp \
  String_Base.cpp \
  Svc_Handler.cpp \
  Swap.cpp \
  Synch_T.cpp \
  Task_Ex_T.cpp \
  Task_T.cpp \
  Test_and_Set.cpp \
  Timeprobe_T.cpp \
  Timer_Hash_T.cpp \
  Timer_Heap_T.cpp \
  Timer_List_T.cpp \
  Timer_Queue_Adapters.cpp \
  Timer_Queue_T.cpp \
  Timer_Wheel_T.cpp \
  TSS_T.cpp \
  Typed_SV_Message.cpp \
  Typed_SV_Message_Queue.cpp \
  Unbounded_Queue.cpp \
  Unbounded_Set_Ex.cpp \
  Unbounded_Set.cpp \
  Vector_T.cpp

HEADER_FILES = \
  Acceptor.h \
  ACE_export.h \
  ACE.h \
  ace_wchar.h \
  Activation_Queue.h \
  Active_Map_Manager.h \
  Active_Map_Manager_T.h \
  Addr.h \
  Arg_Shifter.h \
  ARGV.h \
  Argv_Type_Converter.h \
  Array_Base.h \
  Array.h \
  Asynch_Acceptor.h \
  Asynch_Connector.h \
  Asynch_IO.h \
  Asynch_IO_Impl.h \
  Asynch_Pseudo_Task.h \
  ATM_Acceptor.h \
  ATM_Addr.h \
  ATM_Connector.h \
  ATM_Params.h \
  ATM_QoS.h \
  ATM_Stream.h \
  Atomic_Op.h \
  Atomic_Op_T.h \
  Auto_Event.h \
  Auto_IncDec_T.h \
  Auto_Ptr.h \
  Barrier.h \
  Based_Pointer_Repository.h \
  Based_Pointer_T.h \
  Base_Thread_Adapter.h \
  Basic_Stats.h \
  Basic_Types.h \
  Bound_Ptr.h \
  Cached_Connect_Strategy_T.h \
  Cache_Map_Manager_T.h \
  Caching_Strategies_T.h \
  Caching_Utility_T.h \
  Capabilities.h \
  CDR_Base.h \
  CDR_Stream.h \
  CE_Screen_Output.h \
  Cleanup.h \
  Cleanup_Strategies_T.h \
  Codecs.h \
  Codeset_IBM1047.h \
  Codeset_Registry.h \
  Condition_Recursive_Thread_Mutex.h \
  Condition_T.h \
  Condition_Thread_Mutex.h \
  config-all.h \
  config-doxygen.h \
  config.h \
  config-lite.h \
  config-minimal.h \
  config-posix.h \
  Configuration.h \
  Configuration_Import_Export.h \
  Connection_Recycling_Strategy.h \
  Connector.h \
  Containers.h \
  Containers_T.h \
  Copy_Disabled.h \
  CORBA_macros.h \
  Date_Time.h \
  Default_Constants.h \
  DEV_Addr.h \
  DEV_Connector.h \
  DEV.h \
  DEV_IO.h \
  Dev_Poll_Reactor.h \
  Dirent.h \
  Dirent_Selector.h \
  DLL.h \
  DLL_Manager.h \
  Dump.h \
  Dump_T.h \
  Dynamic.h \
  Dynamic_Service_Base.h \
  Dynamic_Service.h \
  Env_Value_T.h \
  Event.h \
  Event_Handler.h \
  Event_Handler_T.h \
  Exception_Macros.h \
  FIFO.h \
  FIFO_Recv.h \
  FIFO_Recv_Msg.h \
  FIFO_Send.h \
  FIFO_Send_Msg.h \
  FILE_Addr.h \
  Filecache.h \
  FILE_Connector.h \
  FILE.h \
  FILE_IO.h \
  File_Lock.h \
  Flag_Manip.h \
  FlReactor.h \
  Framework_Component.h \
  Framework_Component_T.h \
  Free_List.h \
  Functor.h \
  Functor_String.h \
  Functor_T.h \
  Future.h \
  Future_Set.h \
  Get_Opt.h \
  Global_Macros.h \
  Guard_T.h \
  Handle_Gobbler.h \
  Handle_Ops.h \
  Handle_Set.h \
  Hashable.h \
  Hash_Cache_Map_Manager_T.h \
  Hash_Map_Manager.h \
  Hash_Map_Manager_T.h \
  Hash_Map_With_Allocator_T.h \
  High_Res_Timer.h \
  If_Then_Else.h \
  INET_Addr.h \
  Init_ACE.h \
  Intrusive_List.h \
  Intrusive_List_Node.h \
  IO_Cntl_Msg.h \
  IO_SAP.h \
  iosfwd.h \
  IOStream.h \
  IOStream_T.h \
  IPC_SAP.h \
  Lib_Find.h \
  Local_Name_Space.h \
  Local_Name_Space_T.h \
  Local_Tokens.h \
  Lock_Adapter_T.h \
  Lock.h \
  LOCK_SOCK_Acceptor.h \
  Logging_Strategy.h \
  Log_Msg_Backend.h \
  Log_Msg_Callback.h \
  Log_Msg.h \
  Log_Msg_IPC.h \
  Log_Msg_NT_Event_Log.h \
  Log_Msg_UNIX_Syslog.h \
  Log_Priority.h \
  Log_Record.h \
  LSOCK_Acceptor.h \
  LSOCK_CODgram.h \
  LSOCK_Connector.h \
  LSOCK_Dgram.h \
  LSOCK.h \
  LSOCK_Stream.h \
  Malloc_Allocator.h \
  Malloc_Base.h \
  Malloc.h \
  Malloc_T.h \
  Managed_Object.h \
  Manual_Event.h \
  Map.h \
  Map_Manager.h \
  Map_T.h \
  MEM_Acceptor.h \
  MEM_Addr.h \
  MEM_Connector.h \
  MEM_IO.h \
  Mem_Map.h \
  Memory_Pool.h \
  MEM_SAP.h \
  MEM_Stream.h \
  Message_Block.h \
  Message_Block_T.h \
  Message_Queue.h \
  Message_Queue_T.h \
  Method_Object.h \
  Method_Request.h \
  Metrics_Cache.h \
  Metrics_Cache_T.h \
  Min_Max.h \
  Module.h \
  Msg_WFMO_Reactor.h \
  Multihomed_INET_Addr.h \
  Multiplexor.h \
  Mutex.h \
  Name_Proxy.h \
  Name_Request_Reply.h \
  Name_Space.h \
  Naming_Context.h \
  Node.h \
  Notification_Strategy.h \
  NT_Service.h \
  Null_Barrier.h \
  Null_Condition.h \
  Null_Mutex.h \
  Null_Semaphore.h \
  Obchunk.h \
  Object_Manager_Base.h \
  Object_Manager.h \
  Obstack.h \
  Obstack_T.h \
  OS_Dirent.h \
  OS_Errno.h \
  OS.h \
  OS_Log_Msg_Attributes.h \
  OS_main.h \
  OS_Memory.h \
  OS_NS_arpa_inet.h \
  OS_NS_ctype.h \
  OS_NS_dirent.h \
  OS_NS_dlfcn.h \
  OS_NS_errno.h \
  OS_NS_fcntl.h \
  OS_NS_macros.h \
  OS_NS_math.h \
  OS_NS_netdb.h \
  OS_NS_poll.h \
  OS_NS_pwd.h \
  OS_NS_regex.h \
  OS_NS_signal.h \
  OS_NS_stdio.h \
  OS_NS_stdlib.h \
  OS_NS_string.h \
  OS_NS_strings.h \
  OS_NS_stropts.h \
  OS_NS_sys_mman.h \
  OS_NS_sys_msg.h \
  OS_NS_sys_resource.h \
  OS_NS_sys_select.h \
  OS_NS_sys_shm.h \
  OS_NS_sys_socket.h \
  OS_NS_sys_stat.h \
  OS_NS_sys_time.h \
  OS_NS_sys_uio.h \
  OS_NS_sys_utsname.h \
  OS_NS_sys_wait.h \
  OS_NS_Thread.h \
  OS_NS_time.h \
  OS_NS_unistd.h \
  OS_NS_wchar.h \
  OS_QoS.h \
  OS_String.h \
  OS_Thread_Adapter.h \
  OS_TLI.h \
  Pair.h \
  Pair_T.h \
  Parse_Node.h \
  PI_Malloc.h \
  Pipe.h \
  POSIX_Asynch_IO.h \
  POSIX_CB_Proactor.h \
  POSIX_Proactor.h \
  post.h \
  pre.h \
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
  Reactor.h \
  Reactor_Impl.h \
  Reactor_Notification_Strategy.h \
  Reactor_Timer_Interface.h \
  Read_Buffer.h \
  Recursive_Thread_Mutex.h \
  Recyclable.h \
  Refcountable.h \
  Refcounted_Auto_Ptr.h \
  Registry.h \
  Registry_Name_Space.h \
  Remote_Name_Space.h \
  Remote_Tokens.h \
  Reverse_Lock_T.h \
  RW_Mutex.h \
  RW_Process_Mutex.h \
  RW_Thread_Mutex.h \
  Sample_History.h \
  Sched_Params.h \
  Select_Reactor_Base.h \
  Select_Reactor.h \
  Select_Reactor_T.h \
  Semaphore.h \
  Service_Config.h \
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
  SOCK_Acceptor.h \
  SOCK_CODgram.h \
  Sock_Connect.h \
  SOCK_Connector.h \
  SOCK_Dgram_Bcast.h \
  SOCK_Dgram.h \
  SOCK_Dgram_Mcast.h \
  SOCK.h \
  SOCK_IO.h \
  SOCK_SEQPACK_Acceptor.h \
  SOCK_SEQPACK_Association.h \
  SOCK_SEQPACK_Connector.h \
  SOCK_Stream.h \
  SPIPE_Acceptor.h \
  SPIPE_Addr.h \
  SPIPE_Connector.h \
  SPIPE.h \
  SPIPE_Stream.h \
  SStringfwd.h \
  SString.h \
  Static_Object_Lock.h \
  Stats.h \
  Strategies.h \
  Strategies_T.h \
  Stream.h \
  Stream_Modules.h \
  streams.h \
  String_Base_Const.h \
  String_Base.h \
  SUN_Proactor.h \
  Svc_Conf.h \
  Svc_Conf_Lexer_Guard.h \
  Svc_Conf_Tokens.h \
  svc_export.h \
  Svc_Handler.h \
  SV_Message.h \
  SV_Message_Queue.h \
  SV_Semaphore_Complex.h \
  SV_Semaphore_Simple.h \
  SV_Shared_Memory.h \
  Swap.h \
  Synch.h \
  Synch_Options.h \
  Synch_T.h \
  Synch_Traits.h \
  System_Time.h \
  Task_Ex_T.h \
  Task.h \
  Task_T.h \
  Test_and_Set.h \
  Thread_Adapter.h \
  Thread_Control.h \
  Thread_Exit.h \
  Thread.h \
  Thread_Hook.h \
  Thread_Manager.h \
  Thread_Mutex.h \
  Thread_Semaphore.h \
  Timeprobe.h \
  Timeprobe_T.h \
  Time_Request_Reply.h \
  Timer_Hash.h \
  Timer_Hash_T.h \
  Timer_Heap.h \
  Timer_Heap_T.h \
  Timer_List.h \
  Timer_List_T.h \
  Timer_Queue_Adapters.h \
  Timer_Queuefwd.h \
  Timer_Queue.h \
  Timer_Queue_T.h \
  Timer_Wheel.h \
  Timer_Wheel_T.h \
  Time_Value.h \
  TkReactor.h \
  TLI_Acceptor.h \
  TLI_Connector.h \
  TLI.h \
  TLI_Stream.h \
  Token_Collection.h \
  Token.h \
  Token_Invariants.h \
  Token_Manager.h \
  Token_Request_Reply.h \
  TP_Reactor.h \
  Trace.h \
  TSS_Adapter.h \
  TSS_T.h \
  TTY_IO.h \
  Typed_SV_Message.h \
  Typed_SV_Message_Queue.h \
  Unbounded_Queue.h \
  Unbounded_Set_Ex.h \
  Unbounded_Set.h \
  UNIX_Addr.h \
  UPIPE_Acceptor.h \
  UPIPE_Addr.h \
  UPIPE_Connector.h \
  UPIPE_Stream.h \
  UUID.h \
  Vector_T.h \
  Version.h \
  WFMO_Reactor.h \
  WIN32_Asynch_IO.h \
  WIN32_Proactor.h \
  XML_Svc_Conf.h \
  XTI_ATM_Mcast.h \
  XtReactor.h
## Make sure config.h and config-all.h are in the above header list!

INLINE_FILES = \
  ACE.inl \
  Activation_Queue.inl \
  Active_Map_Manager.inl \
  Active_Map_Manager_T.inl \
  Addr.inl \
  ARGV.inl \
  Asynch_IO_Impl.inl \
  ATM_Acceptor.inl \
  ATM_Addr.inl \
  ATM_Connector.inl \
  ATM_Params.inl \
  ATM_QoS.inl \
  ATM_Stream.inl \
  Atomic_Op.inl \
  Atomic_Op_T.inl \
  Auto_IncDec_T.inl \
  Auto_Ptr.inl \
  Based_Pointer_T.inl \
  Basic_Types.inl \
  Bound_Ptr.inl \
  Cache_Map_Manager_T.inl \
  Caching_Strategies_T.inl \
  Capabilities.inl \
  CDR_Stream.inl \
  Containers.inl \
  Containers_T.inl \
  Date_Time.inl \
  DEV_Addr.inl \
  DEV_Connector.inl \
  DEV.inl \
  DEV_IO.inl \
  Dirent.inl \
  Dynamic.inl \
  Dynamic_Service.inl \
  Env_Value_T.inl \
  Event_Handler.inl \
  Event_Handler_T.inl \
  FIFO.inl \
  FIFO_Recv.inl \
  FIFO_Recv_Msg.inl \
  FIFO_Send.inl \
  FIFO_Send_Msg.inl \
  FILE_Addr.inl \
  FILE_Connector.inl \
  FILE.inl \
  FILE_IO.inl \
  Flag_Manip.inl \
  Free_List.inl \
  Functor.inl \
  Functor_String.inl \
  Functor_T.inl \
  Get_Opt.inl \
  Handle_Gobbler.inl \
  Handle_Set.inl \
  Hash_Cache_Map_Manager_T.inl \
  Hash_Map_Manager_T.inl \
  Hash_Map_With_Allocator_T.inl \
  High_Res_Timer.inl \
  INET_Addr.inl \
  IO_SAP.inl \
  IOStream_T.inl \
  IPC_SAP.inl \
  Local_Tokens.inl \
  Log_Record.inl \
  LSOCK_CODgram.inl \
  LSOCK_Connector.inl \
  LSOCK_Dgram.inl \
  LSOCK.inl \
  LSOCK_Stream.inl \
  Malloc_Allocator.inl \
  Malloc.inl \
  Malloc_T.inl \
  Managed_Object.inl \
  Map_Manager.inl \
  Map_T.inl \
  MEM_Acceptor.inl \
  MEM_Addr.inl \
  MEM_Connector.inl \
  MEM_IO.inl \
  Mem_Map.inl \
  Memory_Pool.inl \
  MEM_SAP.inl \
  MEM_Stream.inl \
  Message_Block.inl \
  Message_Block_T.inl \
  Message_Queue.inl \
  Message_Queue_T.inl \
  Metrics_Cache_T.inl \
  Module.inl \
  Msg_WFMO_Reactor.inl \
  Multihomed_INET_Addr.inl \
  Multiplexor.inl \
  NT_Service.inl \
  Obchunk.inl \
  Object_Manager.inl \
  Obstack_T.inl \
  OS.inl \
  Pair_T.inl \
  Parse_Node.inl \
  PI_Malloc.inl \
  Pipe.inl \
  POSIX_CB_Proactor.inl \
  POSIX_Proactor.inl \
  Proactor.inl \
  Process.inl \
  Process_Manager.inl \
  Profile_Timer.inl \
  RB_Tree.inl \
  Reactor.inl \
  Read_Buffer.inl \
  Refcounted_Auto_Ptr.inl \
  Remote_Tokens.inl \
  Sched_Params.inl \
  Select_Reactor_Base.inl \
  Select_Reactor_T.inl \
  Service_Config.inl \
  Service_Manager.inl \
  Service_Object.inl \
  Service_Repository.inl \
  Service_Types.inl \
  Shared_Memory_MM.inl \
  Shared_Memory_SV.inl \
  Shared_Object.inl \
  Signal.inl \
  Singleton.inl \
  SOCK_Acceptor.inl \
  SOCK_CODgram.inl \
  SOCK_Connector.inl \
  SOCK_Dgram_Bcast.inl \
  SOCK_Dgram.inl \
  SOCK_Dgram_Mcast.inl \
  SOCK.inl \
  SOCK_IO.inl \
  SOCK_SEQPACK_Acceptor.inl \
  SOCK_SEQPACK_Association.inl \
  SOCK_SEQPACK_Connector.inl \
  SOCK_Stream.inl \
  SPIPE_Addr.inl \
  SPIPE_Connector.inl \
  SPIPE.inl \
  SPIPE_Stream.inl \
  SString.inl \
  Stats.inl \
  Strategies_T.inl \
  Stream.inl \
  String_Base.inl \
  SUN_Proactor.inl \
  SV_Message.inl \
  SV_Message_Queue.inl \
  SV_Semaphore_Complex.inl \
  SV_Semaphore_Simple.inl \
  SV_Shared_Memory.inl \
  Synch_Options.inl \
  Task_Ex_T.inl \
  Task.inl \
  Task_T.inl \
  Thread.inl \
  Thread_Manager.inl \
  Timeprobe.inl \
  Timer_Queue_Adapters.inl \
  Timer_Queue_T.inl \
  TLI_Connector.inl \
  TLI.inl \
  TLI_Stream.inl \
  Token_Collection.inl \
  Token.inl \
  Token_Manager.inl \
  Token_Request_Reply.inl \
  TP_Reactor.inl \
  Typed_SV_Message.inl \
  Typed_SV_Message_Queue.inl \
  UNIX_Addr.inl \
  UPIPE_Acceptor.inl \
  UPIPE_Connector.inl \
  UPIPE_Stream.inl \
  Vector_T.inl \
  WFMO_Reactor.inl \
  XTI_ATM_Mcast.inl \
  ace_wchar.inl \
  Argv_Type_Converter.inl \
  Array_Base.inl \
  Auto_Event.inl \
  Barrier.inl \
  Base_Thread_Adapter.inl \
  Basic_Stats.inl \
  CDR_Base.inl \
  Cleanup.inl \
  Codeset_Registry.inl \
  Condition_Thread_Mutex.inl \
  Condition_T.inl \
  Dev_Poll_Reactor.inl \
  Dirent_Selector.inl \
  Event.inl \
  File_Lock.inl \
  Framework_Component.inl \
  Guard_T.inl \
  Hashable.inl \
  Intrusive_List.inl \
  Intrusive_List_Node.inl \
  Lock_Adapter_T.inl \
  Lock.inl \
  Manual_Event.inl \
  Mutex.inl \
  Notification_Strategy.inl \
  OS_Errno.inl \
  OS_Log_Msg_Attributes.inl \
  OS_Memory.inl \
  OS_NS_arpa_inet.inl \
  OS_NS_ctype.inl \
  OS_NS_dirent.inl \
  OS_NS_dlfcn.inl \
  OS_NS_errno.inl \
  OS_NS_fcntl.inl \
  OS_NS_math.inl \
  OS_NS_netdb.inl \
  OS_NS_poll.inl \
  OS_NS_pwd.inl \
  OS_NS_regex.inl \
  OS_NS_signal.inl \
  OS_NS_stdio.inl \
  OS_NS_stdlib.inl \
  OS_NS_string.inl \
  OS_NS_strings.inl \
  OS_NS_stropts.inl \
  OS_NS_sys_mman.inl \
  OS_NS_sys_msg.inl \
  OS_NS_sys_resource.inl \
  OS_NS_sys_select.inl \
  OS_NS_sys_shm.inl \
  OS_NS_sys_socket.inl \
  OS_NS_sys_stat.inl \
  OS_NS_sys_time.inl \
  OS_NS_sys_uio.inl \
  OS_NS_sys_utsname.inl \
  OS_NS_sys_wait.inl \
  OS_NS_Thread.inl \
  OS_NS_time.inl \
  OS_NS_unistd.inl \
  OS_NS_wchar.inl \
  OS_String.inl \
  OS_TLI.inl \
  Process_Mutex.inl \
  Process_Semaphore.inl \
  Reactor_Notification_Strategy.inl \
  Recursive_Thread_Mutex.inl \
  Recyclable.inl \
  Refcountable.inl \
  Reverse_Lock_T.inl \
  RW_Mutex.inl \
  RW_Process_Mutex.inl \
  RW_Thread_Mutex.inl \
  Sample_History.inl \
  Semaphore.inl \
  Swap.inl \
  Thread_Adapter.inl \
  Thread_Control.inl \
  Thread_Mutex.inl \
  Thread_Semaphore.inl \
  Time_Value.inl \
  TSS_Adapter.inl \
  TSS_T.inl \
  Unbounded_Queue.inl \
  Unbounded_Set_Ex.inl \
  Unbounded_Set.inl \
  UUID.inl

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

## Directories to include in the distribution but which I am too lazy to
## create Makefile.am files for.  A Makefile.am for these directories isn't
## needed anyway.
ACE_EXTRA_DIST = os_include

## Automake recognized targets

dist-hook:
	(cd $(srcdir); tar cf - $(ACE_EXTRA_DIST)) | (cd $(distdir); \
          tar xfBp -)
	list=`find $(distdir) -type d -a \( -name CVS -o -name .svn \) -a -print`; for p in $$list; do \
          rm -rf $$p; \
        done

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core core.*
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf Templates.DB gcctemp.c gcctemp so_locations *.ics
	-rm -rf templateregistry SunWS_cache ir.out

## Install headers in "os_include" directory.  Adding a Makefile.am in
## os_include subdirectory just for installation purposes seemed like
## overkill.
install-data-local:
	list=`(cd $(srcdir); find os_include -type d -a \! \( \( -name CVS -o -name .svn \) -prune \) )`; for p in $$list; do \
	  $(mkdir_p) $(DESTDIR)$(pkgincludedir)/$$p; \
	  files=`(cd $(srcdir)/$$p; $(ECHO) *.h)`; \
          test "$$files" != "*.h"  && \
            (cd $(srcdir)/$$p; $(INSTALL_DATA) $$files $(DESTDIR)$(pkgincludedir)/$$p); \
	done

## Clean up data installed by local targets.
uninstall-local:
	-rm -rf $(DESTDIR)$(pkgincludedir)/os_include
