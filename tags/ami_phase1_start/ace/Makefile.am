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
AUTOMAKE_OPTIONS = 1.4

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

if BUILD_TOKEN_FILES
LIBACE_TOKEN = libACE_Token.la
else
LIBACE_TOKEN = 
endif

if BUILD_OTHER_FILES
LIBACE_OTHER = libACE_Other.la
else
LIBACE_OTHER = 
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
        $(LIBACE_TOKEN) \
        $(LIBACE_OTHER)

libACE_OS_la_SOURCES = \
        Basic_Types.cpp \
        OS.cpp \
        Sched_Params.cpp

libACE_Utils_la_SOURCES = \
        ACE.cpp \
        Active_Map_Manager.cpp \
        Arg_Shifter.cpp \
        ARGV.cpp \
        Containers.cpp \
        Dirent.cpp \
        Dynamic.cpp \
        Filecache.cpp \
        Functor.cpp \
        Get_Opt.cpp \
        Hash_Map_Manager.cpp \
        High_Res_Timer.cpp \
        Method_Request.cpp \
        Object_Manager.cpp \
        Profile_Timer.cpp \
        Registry.cpp \
        SString.cpp \
        Stats.cpp \
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
EXTRA_libACE_Utils_la_SOURCES = gethrtime.cpp

libACE_Logging_la_SOURCES = \
        Dump.cpp \
        Log_Msg.cpp \
        Log_Record.cpp \
        Trace.cpp

libACE_Threads_la_SOURCES = \
        Activation_Queue.cpp \
        Process.cpp \
        Process_Manager.cpp \
        Synch.cpp \
        Synch_Options.cpp \
        Thread.cpp \
        Thread_Manager.cpp \
        Token.cpp

libACE_Demux_la_SOURCES = \
        Event_Handler.cpp \
        Handle_Set.cpp \
        POSIX_Proactor.cpp \
        Priority_Reactor.cpp \
        Proactor.cpp \
        Reactor.cpp \
        Select_Reactor.cpp \
        Select_Reactor_Base.cpp \
        WFMO_Reactor.cpp

libACE_Connection_la_SOURCES = \
        Asynch_IO.cpp \
        Asynch_IO_Impl.cpp \
        POSIX_Asynch_IO.cpp \
        Strategies.cpp

libACE_Sockets_la_SOURCES = \
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
        SOCK_Connector.cpp \
        SOCK_Dgram.cpp \
        SOCK_Dgram_Bcast.cpp \
        SOCK_Dgram_Mcast.cpp \
        SOCK_IO.cpp \
        SOCK_Stream.cpp

libACE_IPC_la_SOURCES = \
        Addr.cpp \
	ATM_Addr.cpp \
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
        INET_Addr.cpp \
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
        UPIPE_Stream.cpp

libACE_Svcconf_la_SOURCES = \
        DLL.cpp \
        Parse_Node.cpp \
        Service_Config.cpp \
        Service_Manager.cpp \
        Service_Object.cpp \
        Service_Repository.cpp \
        Service_Types.cpp \
        Shared_Object.cpp \
        Svc_Conf_l.cpp \
        Svc_Conf_y.cpp

libACE_Streams_la_SOURCES = \
        CDR_Stream.cpp \
        Message_Block.cpp \
        Message_Queue.cpp \
        Task.cpp

libACE_Memory_la_SOURCES = \
        Malloc.cpp \
        Mem_Map.cpp \
        Memory_Pool.cpp \
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

libACE_Other_la_SOURCES = \
        CORBA_Handler.cpp \
        CORBA_Ref.cpp \
        Local_Name_Space.cpp \
        Msg_WFMO_Reactor.cpp \
        Name_Proxy.cpp \
        Name_Request_Reply.cpp \
        Name_Space.cpp \
        Naming_Context.cpp \
        Registry_Name_Space.cpp \
        Remote_Name_Space.cpp \
        TP_Reactor.cpp \
        XtReactor.cpp

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
	$(libACE_Token_la_SOURCES) \
	$(libACE_Other_la_SOURCES)

# We only want `gethrtime.cpp' if building on Pentium(tm) so we need
# to let Automake know that `gethrtime.cpp' is conditionally built.
EXTRA_libACE_la_SOURCES = gethrtime.cpp


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
##                  -version-info 10:4:6   ## 10-6=4 --> libACE.so.4.6.4
libACE_la_LDFLAGS = $(X_LIBS) \
        -version-info @ACE_CURRENT@:@ACE_REVISION@:@ACE_AGE@

# We also add any X libraries to the list.
libACE_la_LIBADD = $(X_PRE_LIBS) $(ACE_XLIBS) $(X_EXTRA_LIBS)

## These are template source files.
ACE_TEMPLATE_FILES = \
        Acceptor.cpp \
        Active_Map_Manager_T.cpp \
        Asynch_Acceptor.cpp \
        Auto_Ptr.cpp \
        Connector.cpp \
        Containers_T.cpp \
        Dump_T.cpp \
        Dynamic_Service.cpp \
        Env_Value_T.cpp \
        Event_Handler_T.cpp \
        Free_List.cpp \
        Functor_T.cpp \
        Future.cpp \
        Hash_Map_Manager_T.cpp \
        IOStream_T.cpp \
        LOCK_SOCK_Acceptor.cpp \
        Local_Name_Space_T.cpp \
        Malloc_T.cpp \
        Managed_Object.cpp \
        Map_Manager.cpp \
        Map_T.cpp \
        Message_Block_T.cpp \
        Message_Queue_T.cpp \
        Module.cpp \
	Pair_T.cpp \
        RB_Tree.cpp \
        Select_Reactor_T.cpp \
        Singleton.cpp \
        Strategies_T.cpp \
        Stream.cpp \
        Stream_Modules.cpp \
        Svc_Handler.cpp \
        Synch_T.cpp \
        Task_T.cpp \
        Template_Instantiations.cpp \
        Timeprobe_T.cpp \
        Timer_Hash_T.cpp \
        Timer_Heap_T.cpp \
        Timer_List_T.cpp \
        Timer_Queue_Adapters.cpp \
        Timer_Queue_T.cpp \
        Timer_Wheel_T.cpp \
        Typed_SV_Message.cpp \
        Typed_SV_Message_Queue.cpp

## It would be good to remove pkgincludedir.  However, we want to install
## headers in "$(prefix)/ace" not "$(prefix)/ACE" because the source files
## include files in the directory "ace."  By default pkgincludedir would
## be "$(prefix)/$(PACKAGE)" which would be "$(prefix)/ACE" in our case.
## It is for this reason that we must redefine "pkgincludedir."
## However, if we set the package to "ace" instead of "ACE" then we won't
## need to set the "pkgincludedir."
##                    -Ossama
##pkgincludedir = $(prefix)/include/ace
pkginclude_HEADERS = \
        ACE.h \
        ACE.i \
        ARGV.h \
        ARGV.i \
	ATM_Addr.h \
	ATM_Addr.i \
        Acceptor.h \
        Acceptor.i \
        Activation_Queue.h \
        Activation_Queue.i \
        Active_Map_Manager.h \
        Active_Map_Manager.i \
        Active_Map_Manager_T.h \
        Active_Map_Manager_T.i \
        Addr.h \
        Addr.i \
        Arg_Shifter.h \
        Array.h \
        Asynch_Acceptor.h \
        Asynch_Acceptor.i \
        Asynch_IO.h \
        Asynch_IO.i \
        Asynch_IO_Impl.h \
        Asynch_IO_Impl.i \
        Atomic_Op.i \
        Auto_Ptr.h \
        Auto_Ptr.i \
        Basic_Types.h \
        Basic_Types.i \
        CDR_Stream.h \
        CDR_Stream.i \
        CORBA_Handler.h \
        CORBA_Handler.i \
        CORBA_Ref.h \
        CORBA_Ref.i \
        CORBA_macros.h \
        Connector.h \
        Connector.i \
        Containers.h \
        Containers.i \
        Containers_T.h \
        Containers_T.i \
        DEV.h \
        DEV.i \
        DEV_Addr.h \
        DEV_Addr.i \
        DEV_Connector.h \
        DEV_Connector.i \
        DEV_IO.h \
        DEV_IO.i \
        DLL.h \
        Date_Time.h \
        Date_Time.i \
        Dirent.h \
        Dirent.i \
        Dump.h \
        Dump_T.h \
        Dynamic.h \
        Dynamic.i \
        Dynamic_Service.h \
        Dynamic_Service.i \
        Env_Value_T.h \
        Env_Value_T.i \
        Event_Handler.h \
        Event_Handler.i \
        Event_Handler_T.h \
        Event_Handler_T.i \
        FIFO.h \
        FIFO.i \
        FIFO_Recv.h \
        FIFO_Recv.i \
        FIFO_Recv_Msg.h \
        FIFO_Recv_Msg.i \
        FIFO_Send.h \
        FIFO_Send.i \
        FIFO_Send_Msg.h \
        FIFO_Send_Msg.i \
        FILE.h \
        FILE.i \
        FILE_Addr.h \
        FILE_Addr.i \
        FILE_Connector.h \
        FILE_Connector.i \
        FILE_IO.h \
        FILE_IO.i \
        Filecache.h \
        Free_List.h \
        Free_List.i \
        Functor.h \
        Functor.i \
        Functor_T.h \
        Functor_T.i \
        Future.h \
        Get_Opt.h \
        Get_Opt.i \
        Handle_Set.h \
        Handle_Set.i \
        Hash_Map_Manager.h \
        Hash_Map_Manager.i \
        Hash_Map_Manager_T.h \
        Hash_Map_Manager_T.i \
        Hash_Purgable_Map_Manager_T.h \
        Hash_Purgable_Map_Manager_T.i \
        High_Res_Timer.h \
        High_Res_Timer.i \
        INET_Addr.h \
        INET_Addr.i \
        IOStream.h \
        IOStream_T.h \
        IOStream_T.i \
        IO_Cntl_Msg.h \
        IO_Cntl_Msg.i \
        IO_SAP.h \
        IO_SAP.i \
        IPC_SAP.h \
        IPC_SAP.i \
        LOCK_SOCK_Acceptor.h \
        LSOCK.h \
        LSOCK.i \
        LSOCK_Acceptor.h \
        LSOCK_Acceptor.i \
        LSOCK_CODgram.h \
        LSOCK_CODgram.i \
        LSOCK_Connector.h \
        LSOCK_Connector.i \
        LSOCK_Dgram.h \
        LSOCK_Dgram.i \
        LSOCK_Stream.h \
        LSOCK_Stream.i \
        Local_Name_Space.h \
        Local_Name_Space_T.h \
        Local_Tokens.h \
        Local_Tokens.i \
        Log_Msg.h \
        Log_Msg.i \
        Log_Priority.h \
        Log_Record.h \
        Log_Record.i \
        Malloc.h \
        Malloc.i \
        Malloc_Base.h \
        Malloc_T.h \
        Malloc_T.i \
        Managed_Object.h \
        Managed_Object.i \
        Map.h \
        Map.i \
        Map_Manager.h \
        Map_Manager.i \
        Map_T.h \
        Map_T.i \
        Mem_Map.h \
        Mem_Map.i \
        Memory_Pool.h \
        Memory_Pool.i \
        Message_Block.h \
        Message_Block.i \
        Message_Block_T.h \
        Message_Block_T.i \
        Message_Queue.h \
        Message_Queue.i \
        Message_Queue_T.h \
        Message_Queue_T.i \
        Method_Object.h \
        Method_Request.h \
        Module.h \
        Module.i \
        Msg_WFMO_Reactor.h \
        Msg_WFMO_Reactor.i \
        Multiplexor.h \
        Multiplexor.i \
        NT_Service.h \
        NT_Service.i \
        Name_Proxy.h \
        Name_Request_Reply.h \
        Name_Space.h \
        Naming_Context.h \
        OS.h \
        OS.i \
        Object_Manager.h \
        Object_Manager.i \
        Obstack.h \
        Obstack.i \
        POSIX_Asynch_IO.h \
        POSIX_Asynch_IO.i \
        POSIX_Proactor.h \
        POSIX_Proactor.i \
        Pair.h \
        Pair.i \
        Pair_T.h \
        Pair_T.i \
        Parse_Node.h \
        Parse_Node.i \
        Pipe.h \
        Pipe.i \
        Priority_Reactor.h \
        Priority_Reactor.i \
        Proactor.h \
        Proactor.i \
        Proactor_Impl.h \
        Process.h \
        Process.i \
        Process_Manager.h \
        Process_Manager.i \
        Profile_Timer.h \
        Profile_Timer.i \
        RB_Tree.h \
        RB_Tree.i \
        Reactor.h \
        Reactor.i \
        Reactor_Impl.h \
        Read_Buffer.h \
        Read_Buffer.i \
        Registry.h \
        Registry_Name_Space.h \
        Remote_Name_Space.h \
        Remote_Tokens.h \
        Remote_Tokens.i \
        SOCK.h \
        SOCK.i \
        SOCK_Acceptor.h \
        SOCK_Acceptor.i \
        SOCK_CODgram.h \
        SOCK_CODgram.i \
        SOCK_Connector.h \
        SOCK_Connector.i \
        SOCK_Dgram.h \
        SOCK_Dgram.i \
        SOCK_Dgram_Bcast.h \
        SOCK_Dgram_Bcast.i \
        SOCK_Dgram_Mcast.h \
        SOCK_Dgram_Mcast.i \
        SOCK_IO.h \
        SOCK_IO.i \
        SOCK_Stream.h \
        SOCK_Stream.i \
        SPIPE.h \
        SPIPE.i \
        SPIPE_Acceptor.h \
        SPIPE_Acceptor.i \
        SPIPE_Addr.h \
        SPIPE_Addr.i \
        SPIPE_Connector.h \
        SPIPE_Connector.i \
        SPIPE_Stream.h \
        SPIPE_Stream.i \
        SString.h \
        SString.i \
        SV_Message.h \
        SV_Message.i \
        SV_Message_Queue.h \
        SV_Message_Queue.i \
        SV_Semaphore_Complex.h \
        SV_Semaphore_Complex.i \
        SV_Semaphore_Simple.h \
        SV_Semaphore_Simple.i \
        SV_Shared_Memory.h \
        SV_Shared_Memory.i \
        Sched_Params.h \
        Sched_Params.i \
        Select_Reactor.h \
        Select_Reactor.i \
        Select_Reactor_Base.h \
        Select_Reactor_Base.i \
        Select_Reactor_T.h \
        Select_Reactor_T.i \
        Service_Config.h \
        Service_Config.i \
        Service_Manager.h \
        Service_Manager.i \
        Service_Object.h \
        Service_Object.i \
        Service_Repository.h \
        Service_Repository.i \
        Service_Types.h \
        Service_Types.i \
        Shared_Memory.h \
        Shared_Memory_MM.h \
        Shared_Memory_MM.i \
        Shared_Memory_SV.h \
        Shared_Memory_SV.i \
        Shared_Object.h \
        Shared_Object.i \
        Signal.h \
        Signal.i \
        Singleton.h \
        Singleton.i \
        Stats.h \
        Stats.i \
        Strategies.h \
        Strategies.i \
        Strategies_T.h \
        Strategies_T.i \
        Stream.h \
        Stream.i \
        Stream_Modules.h \
        Stream_Modules.i \
        Svc_Conf.h \
        Svc_Conf_Tokens.h \
        Svc_Handler.h \
        Svc_Handler.i \
        Synch.h \
        Synch.i \
        Synch_Options.h \
        Synch_Options.i \
        Synch_T.h \
        Synch_T.i \
        System_Time.h \
        TLI.h \
        TLI.i \
        TLI_Acceptor.h \
        TLI_Acceptor.i \
        TLI_Connector.h \
        TLI_Connector.i \
        TLI_Stream.h \
        TLI_Stream.i \
        TP_Reactor.h \
        TP_Reactor.i \
        TTY_IO.h \
        Task.h \
        Task.i \
        Task_T.h \
        Task_T.i \
        Thread.h \
        Thread.i \
        Thread_Manager.h \
        Thread_Manager.i \
        Time_Request_Reply.h \
        Time_Value.h \
        Timeprobe.h \
        Timeprobe.i \
        Timeprobe_T.h \
        Timer_Hash.h \
        Timer_Hash_T.h \
        Timer_Heap.h \
        Timer_Heap_T.h \
        Timer_List.h \
        Timer_List_T.h \
        Timer_Queue.h \
        Timer_Queue_Adapters.h \
        Timer_Queue_Adapters.i \
        Timer_Queue_T.h \
        Timer_Queue_T.i \
        Timer_Wheel.h \
        Timer_Wheel_T.h \
        Token.h \
        Token.i \
        Token_Collection.h \
        Token_Collection.i \
        Token_Invariants.h \
        Token_Invariants.i \
        Token_Manager.h \
        Token_Manager.i \
        Token_Request_Reply.h \
        Token_Request_Reply.i \
        Trace.h \
        Trace.i \
        Typed_SV_Message.h \
        Typed_SV_Message.i \
        Typed_SV_Message_Queue.h \
        Typed_SV_Message_Queue.i \
        UNIX_Addr.h \
        UNIX_Addr.i \
        UPIPE_Acceptor.h \
        UPIPE_Acceptor.i \
        UPIPE_Addr.h \
        UPIPE_Connector.h \
        UPIPE_Connector.i \
        UPIPE_Stream.h \
        UPIPE_Stream.i \
        Version.h \
        WFMO_Reactor.h \
        WFMO_Reactor.i \
        WIN32_Asynch_IO.h \
        WIN32_Asynch_IO.i \
        WIN32_Proactor.h \
        WIN32_Proactor.i \
        XtReactor.h \
        config.h \
        inc_user_config.h \
        iosfwd.h \
        streams.h \
        ws2tcpip.h \
        $(ACE_TEMPLATE_FILES)
## Make sure config.h and $(ACE_TEMPLATE_FILES) are in the above header list!

## Make sure the following get into the distribution
EXTRA_DIST = $(ACE_TEMPLATE_FILES)

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations
