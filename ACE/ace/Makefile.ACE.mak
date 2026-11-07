# Microsoft Developer Studio Generated NMAKE File
!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified. Defaulting to Win32 Debug.
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release"
!ELSE
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Makefile.ACE.mak" CFG="Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE
!ERROR An invalid configuration was specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!IF "$(DEPGEN)" == ""
!IF EXISTS("$(MPC_ROOT)/depgen.pl")
DEPGEN=perl $(MPC_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(DEPGEN_ROOT)/depgen.pl")
DEPGEN=perl $(DEPGEN_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(ACE_ROOT)/bin/depgen.pl")
DEPGEN=perl $(ACE_ROOT)/bin/depgen.pl -i -t nmake
!ENDIF
!ENDIF

GENERATED_DIRTY =

!IF  "$(CFG)" == "Win32 Debug"

OUTDIR=..\lib
INTDIR=Debug\ACE\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\lib\ACEd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_BUILD_DLL -f "Makefile.ACE.dep" "ACE.cpp" "ACE_crc32.cpp" "ACE_crc_ccitt.cpp" "ATM_Acceptor.cpp" "ATM_Addr.cpp" "ATM_Connector.cpp" "ATM_Params.cpp" "ATM_QoS.cpp" "ATM_Stream.cpp" "Activation_Queue.cpp" "Active_Map_Manager.cpp" "Addr.cpp" "Argv_Type_Converter.cpp" "Assert.cpp" "Asynch_IO.cpp" "Asynch_IO_Impl.cpp" "Asynch_Pseudo_Task.cpp" "Atomic_Op.cpp" "Atomic_Op_Sparc.c" "Barrier.cpp" "Base_Thread_Adapter.cpp" "Based_Pointer_Repository.cpp" "Basic_Stats.cpp" "Basic_Types.cpp" "CDR_Base.cpp" "CDR_Size.cpp" "CDR_Stream.cpp" "Capabilities.cpp" "Cleanup.cpp" "Codecs.cpp" "Codeset_IBM1047.cpp" "Codeset_Registry.cpp" "Codeset_Registry_db.cpp" "Condition_Attributes.cpp" "Condition_Recursive_Thread_Mutex.cpp" "Condition_Thread_Mutex.cpp" "Configuration.cpp" "Configuration_Import_Export.cpp" "Connection_Recycling_Strategy.cpp" "Containers.cpp" "Copy_Disabled.cpp" "DEV.cpp" "DEV_Addr.cpp" "DEV_Connector.cpp" "DEV_IO.cpp" "DLL.cpp" "DLL_Manager.cpp" "Date_Time.cpp" "Dev_Poll_Reactor.cpp" "Dirent.cpp" "Dirent_Selector.cpp" "Dump.cpp" "Dynamic.cpp" "Dynamic_Message_Strategy.cpp" "Dynamic_Service_Base.cpp" "Dynamic_Service_Dependency.cpp" "Encoding_Converter.cpp" "Encoding_Converter_Factory.cpp" "Event_Base.cpp" "Event_Handler.cpp" "Event_Handler_Handle_Timeout_Upcall.cpp" "FIFO.cpp" "FIFO_Recv.cpp" "FIFO_Recv_Msg.cpp" "FIFO_Send.cpp" "FIFO_Send_Msg.cpp" "FILE.cpp" "FILE_Addr.cpp" "FILE_Connector.cpp" "FILE_IO.cpp" "File_Lock.cpp" "Filecache.cpp" "Flag_Manip.cpp" "Framework_Component.cpp" "Functor.cpp" "Functor_String.cpp" "Get_Opt.cpp" "Handle_Ops.cpp" "Handle_Set.cpp" "Hashable.cpp" "High_Res_Timer.cpp" "ICMP_Socket.cpp" "INET_Addr.cpp" "IOStream.cpp" "IO_Cntl_Msg.cpp" "IO_SAP.cpp" "IPC_SAP.cpp" "Init_ACE.cpp" "LSOCK.cpp" "LSOCK_Acceptor.cpp" "LSOCK_CODgram.cpp" "LSOCK_Connector.cpp" "LSOCK_Dgram.cpp" "LSOCK_Stream.cpp" "Lib_Find.cpp" "Local_Memory_Pool.cpp" "Local_Name_Space.cpp" "Local_Tokens.cpp" "Lock.cpp" "Log_Category.cpp" "Log_Msg.cpp" "Log_Msg_Backend.cpp" "Log_Msg_Callback.cpp" "Log_Msg_IPC.cpp" "Log_Msg_NT_Event_Log.cpp" "Log_Msg_UNIX_Syslog.cpp" "Log_Record.cpp" "Logging_Strategy.cpp" "MEM_Acceptor.cpp" "MEM_Addr.cpp" "MEM_Connector.cpp" "MEM_IO.cpp" "MEM_SAP.cpp" "MEM_Stream.cpp" "MMAP_Memory_Pool.cpp" "Malloc.cpp" "Malloc_Allocator.cpp" "Mem_Map.cpp" "Message_Block.cpp" "Message_Queue.cpp" "Message_Queue_NT.cpp" "Message_Queue_Vx.cpp" "Method_Request.cpp" "Monitor_Admin.cpp" "Monitor_Admin_Manager.cpp" "Monitor_Base.cpp" "Monitor_Control_Action.cpp" "Monitor_Control_Types.cpp" "Monitor_Point_Registry.cpp" "Monitor_Size.cpp" "Monotonic_Time_Policy.cpp" "Msg_WFMO_Reactor.cpp" "Multihomed_INET_Addr.cpp" "Mutex.cpp" "NT_Service.cpp" "Name_Proxy.cpp" "Name_Request_Reply.cpp" "Name_Space.cpp" "Naming_Context.cpp" "Netlink_Addr.cpp" "Notification_Queue.cpp" "Notification_Strategy.cpp" "Null_Mutex.cpp" "OS_Errno.cpp" "OS_Log_Msg_Attributes.cpp" "OS_NS_Thread.cpp" "OS_NS_arpa_inet.cpp" "OS_NS_ctype.cpp" "OS_NS_devctl.cpp" "OS_NS_dirent.cpp" "OS_NS_dlfcn.cpp" "OS_NS_errno.cpp" "OS_NS_fcntl.cpp" "OS_NS_math.cpp" "OS_NS_netdb.cpp" "OS_NS_poll.cpp" "OS_NS_pwd.cpp" "OS_NS_regex.cpp" "OS_NS_signal.cpp" "OS_NS_stdio.cpp" "OS_NS_stdlib.cpp" "OS_NS_string.cpp" "OS_NS_strings.cpp" "OS_NS_stropts.cpp" "OS_NS_sys_mman.cpp" "OS_NS_sys_msg.cpp" "OS_NS_sys_resource.cpp" "OS_NS_sys_select.cpp" "OS_NS_sys_sendfile.cpp" "OS_NS_sys_shm.cpp" "OS_NS_sys_socket.cpp" "OS_NS_sys_stat.cpp" "OS_NS_sys_time.cpp" "OS_NS_sys_uio.cpp" "OS_NS_sys_utsname.cpp" "OS_NS_sys_wait.cpp" "OS_NS_time.cpp" "OS_NS_unistd.cpp" "OS_NS_wchar.cpp" "OS_NS_wctype.cpp" "OS_QoS.cpp" "OS_TLI.cpp" "OS_Thread_Adapter.cpp" "OS_main.cpp" "Obchunk.cpp" "Object_Manager.cpp" "Object_Manager_Base.cpp" "Obstack.cpp" "PI_Malloc.cpp" "POSIX_Asynch_IO.cpp" "POSIX_CB_Proactor.cpp" "POSIX_Proactor.cpp" "Pagefile_Memory_Pool.cpp" "Parse_Node.cpp" "Ping_Socket.cpp" "Pipe.cpp" "Priority_Reactor.cpp" "Proactor.cpp" "Proactor_Impl.cpp" "Process.cpp" "Process_Manager.cpp" "Process_Mutex.cpp" "Process_Semaphore.cpp" "Profile_Timer.cpp" "RW_Mutex.cpp" "RW_Process_Mutex.cpp" "RW_Thread_Mutex.cpp" "Reactor.cpp" "Reactor_Impl.cpp" "Reactor_Notification_Strategy.cpp" "Reactor_Timer_Interface.cpp" "Read_Buffer.cpp" "Recursive_Thread_Mutex.cpp" "Recyclable.cpp" "Registry.cpp" "Registry_Name_Space.cpp" "Remote_Name_Space.cpp" "Remote_Tokens.cpp" "Rtems_init.c" "SOCK.cpp" "SOCK_Acceptor.cpp" "SOCK_CODgram.cpp" "SOCK_Connector.cpp" "SOCK_Dgram.cpp" "SOCK_Dgram_Bcast.cpp" "SOCK_Dgram_Mcast.cpp" "SOCK_IO.cpp" "SOCK_Netlink.cpp" "SOCK_SEQPACK_Acceptor.cpp" "SOCK_SEQPACK_Association.cpp" "SOCK_SEQPACK_Connector.cpp" "SOCK_Stream.cpp" "SPIPE.cpp" "SPIPE_Acceptor.cpp" "SPIPE_Addr.cpp" "SPIPE_Connector.cpp" "SPIPE_Stream.cpp" "SString.cpp" "SUN_Proactor.cpp" "SV_Message.cpp" "SV_Message_Queue.cpp" "SV_Semaphore_Complex.cpp" "SV_Semaphore_Simple.cpp" "SV_Shared_Memory.cpp" "Sample_History.cpp" "Sbrk_Memory_Pool.cpp" "Sched_Params.cpp" "Select_Reactor_Base.cpp" "Semaphore.cpp" "Service_Config.cpp" "Service_Gestalt.cpp" "Service_Manager.cpp" "Service_Object.cpp" "Service_Repository.cpp" "Service_Types.cpp" "Shared_Memory.cpp" "Shared_Memory_MM.cpp" "Shared_Memory_Pool.cpp" "Shared_Memory_SV.cpp" "Shared_Object.cpp" "Sig_Adapter.cpp" "Sig_Handler.cpp" "Signal.cpp" "Sock_Connect.cpp" "Stack_Trace.cpp" "Stats.cpp" "String_Base_Const.cpp" "Svc_Conf_Lexer.cpp" "Svc_Conf_y.cpp" "Synch_Options.cpp" "System_Time.cpp" "TLI.cpp" "TLI_Acceptor.cpp" "TLI_Connector.cpp" "TLI_Stream.cpp" "TP_Reactor.cpp" "TSS_Adapter.cpp" "TTY_IO.cpp" "Task.cpp" "Thread.cpp" "Thread_Adapter.cpp" "Thread_Control.cpp" "Thread_Exit.cpp" "Thread_Hook.cpp" "Thread_Manager.cpp" "Thread_Mutex.cpp" "Thread_Semaphore.cpp" "Throughput_Stats.cpp" "Time_Policy.cpp" "Time_Value.cpp" "Timeprobe.cpp" "Token.cpp" "Token_Collection.cpp" "Token_Invariants.cpp" "Token_Manager.cpp" "Token_Request_Reply.cpp" "Trace.cpp" "UNIX_Addr.cpp" "UPIPE_Acceptor.cpp" "UPIPE_Connector.cpp" "UPIPE_Stream.cpp" "UTF16_Encoding_Converter.cpp" "UTF32_Encoding_Converter.cpp" "UTF8_Encoding_Converter.cpp" "UUID.cpp" "WFMO_Reactor.cpp" "WIN32_Asynch_IO.cpp" "WIN32_Proactor.cpp" "XML_Svc_Conf.cpp" "XTI_ATM_Mcast.cpp" "ace_wchar.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACEd.pdb"
	-@del /f/q "..\lib\ACEd.dll"
	-@del /f/q "$(OUTDIR)\ACEd.lib"
	-@del /f/q "$(OUTDIR)\ACEd.exp"
	-@del /f/q "$(OUTDIR)\ACEd.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\ACE\$(NULL)" mkdir "Debug\ACE"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll /debug /pdb:"..\lib\ACEd.pdb" /machine:I386 /out:"..\lib\ACEd.dll" /implib:"$(OUTDIR)\ACEd.lib"
LINK32_OBJS= \
	"$(INTDIR)\ace.res" \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\ACE_crc32.obj" \
	"$(INTDIR)\ACE_crc_ccitt.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Assert.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\Atomic_Op.obj" \
	"$(INTDIR)\Atomic_Op_Sparc.obj" \
	"$(INTDIR)\Barrier.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Size.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\Cleanup.obj" \
	"$(INTDIR)\Codecs.obj" \
	"$(INTDIR)\Codeset_IBM1047.obj" \
	"$(INTDIR)\Codeset_Registry.obj" \
	"$(INTDIR)\Codeset_Registry_db.obj" \
	"$(INTDIR)\Condition_Attributes.obj" \
	"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Condition_Thread_Mutex.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\Dev_Poll_Reactor.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Message_Strategy.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Dynamic_Service_Dependency.obj" \
	"$(INTDIR)\Encoding_Converter.obj" \
	"$(INTDIR)\Encoding_Converter_Factory.obj" \
	"$(INTDIR)\Event_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\File_Lock.obj" \
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Flag_Manip.obj" \
	"$(INTDIR)\Framework_Component.obj" \
	"$(INTDIR)\Functor.obj" \
	"$(INTDIR)\Functor_String.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\ICMP_Socket.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Memory_Pool.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Log_Category.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\MMAP_Memory_Pool.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Message_Queue_NT.obj" \
	"$(INTDIR)\Message_Queue_Vx.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Monitor_Admin.obj" \
	"$(INTDIR)\Monitor_Admin_Manager.obj" \
	"$(INTDIR)\Monitor_Base.obj" \
	"$(INTDIR)\Monitor_Control_Action.obj" \
	"$(INTDIR)\Monitor_Control_Types.obj" \
	"$(INTDIR)\Monitor_Point_Registry.obj" \
	"$(INTDIR)\Monitor_Size.obj" \
	"$(INTDIR)\Monotonic_Time_Policy.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multihomed_INET_Addr.obj" \
	"$(INTDIR)\Mutex.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Netlink_Addr.obj" \
	"$(INTDIR)\Notification_Queue.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\Null_Mutex.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_NS_Thread.obj" \
	"$(INTDIR)\OS_NS_arpa_inet.obj" \
	"$(INTDIR)\OS_NS_ctype.obj" \
	"$(INTDIR)\OS_NS_devctl.obj" \
	"$(INTDIR)\OS_NS_dirent.obj" \
	"$(INTDIR)\OS_NS_dlfcn.obj" \
	"$(INTDIR)\OS_NS_errno.obj" \
	"$(INTDIR)\OS_NS_fcntl.obj" \
	"$(INTDIR)\OS_NS_math.obj" \
	"$(INTDIR)\OS_NS_netdb.obj" \
	"$(INTDIR)\OS_NS_poll.obj" \
	"$(INTDIR)\OS_NS_pwd.obj" \
	"$(INTDIR)\OS_NS_regex.obj" \
	"$(INTDIR)\OS_NS_signal.obj" \
	"$(INTDIR)\OS_NS_stdio.obj" \
	"$(INTDIR)\OS_NS_stdlib.obj" \
	"$(INTDIR)\OS_NS_string.obj" \
	"$(INTDIR)\OS_NS_strings.obj" \
	"$(INTDIR)\OS_NS_stropts.obj" \
	"$(INTDIR)\OS_NS_sys_mman.obj" \
	"$(INTDIR)\OS_NS_sys_msg.obj" \
	"$(INTDIR)\OS_NS_sys_resource.obj" \
	"$(INTDIR)\OS_NS_sys_select.obj" \
	"$(INTDIR)\OS_NS_sys_sendfile.obj" \
	"$(INTDIR)\OS_NS_sys_shm.obj" \
	"$(INTDIR)\OS_NS_sys_socket.obj" \
	"$(INTDIR)\OS_NS_sys_stat.obj" \
	"$(INTDIR)\OS_NS_sys_time.obj" \
	"$(INTDIR)\OS_NS_sys_uio.obj" \
	"$(INTDIR)\OS_NS_sys_utsname.obj" \
	"$(INTDIR)\OS_NS_sys_wait.obj" \
	"$(INTDIR)\OS_NS_time.obj" \
	"$(INTDIR)\OS_NS_unistd.obj" \
	"$(INTDIR)\OS_NS_wchar.obj" \
	"$(INTDIR)\OS_NS_wctype.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_main.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Object_Manager_Base.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_CB_Proactor.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Pagefile_Memory_Pool.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Ping_Socket.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Proactor_Impl.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\RW_Mutex.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\RW_Thread_Mutex.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Impl.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Reactor_Timer_Interface.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Rtems_init.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Netlink.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Association.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Connector.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\SUN_Proactor.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sbrk_Memory_Pool.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Semaphore.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Gestalt.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_Pool.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Sig_Adapter.obj" \
	"$(INTDIR)\Sig_Handler.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\Stack_Trace.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\Svc_Conf_Lexer.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\TSS_Adapter.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Thread_Mutex.obj" \
	"$(INTDIR)\Thread_Semaphore.obj" \
	"$(INTDIR)\Throughput_Stats.obj" \
	"$(INTDIR)\Time_Policy.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\UTF16_Encoding_Converter.obj" \
	"$(INTDIR)\UTF32_Encoding_Converter.obj" \
	"$(INTDIR)\UTF8_Encoding_Converter.obj" \
	"$(INTDIR)\UUID.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XTI_ATM_Mcast.obj" \
	"$(INTDIR)\ace_wchar.obj"

"..\lib\ACEd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\lib\ACEd.dll.manifest" mt.exe -manifest "..\lib\ACEd.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=..\lib
INTDIR=Release\ACE\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\lib\ACE.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_BUILD_DLL -f "Makefile.ACE.dep" "ACE.cpp" "ACE_crc32.cpp" "ACE_crc_ccitt.cpp" "ATM_Acceptor.cpp" "ATM_Addr.cpp" "ATM_Connector.cpp" "ATM_Params.cpp" "ATM_QoS.cpp" "ATM_Stream.cpp" "Activation_Queue.cpp" "Active_Map_Manager.cpp" "Addr.cpp" "Argv_Type_Converter.cpp" "Assert.cpp" "Asynch_IO.cpp" "Asynch_IO_Impl.cpp" "Asynch_Pseudo_Task.cpp" "Atomic_Op.cpp" "Atomic_Op_Sparc.c" "Barrier.cpp" "Base_Thread_Adapter.cpp" "Based_Pointer_Repository.cpp" "Basic_Stats.cpp" "Basic_Types.cpp" "CDR_Base.cpp" "CDR_Size.cpp" "CDR_Stream.cpp" "Capabilities.cpp" "Cleanup.cpp" "Codecs.cpp" "Codeset_IBM1047.cpp" "Codeset_Registry.cpp" "Codeset_Registry_db.cpp" "Condition_Attributes.cpp" "Condition_Recursive_Thread_Mutex.cpp" "Condition_Thread_Mutex.cpp" "Configuration.cpp" "Configuration_Import_Export.cpp" "Connection_Recycling_Strategy.cpp" "Containers.cpp" "Copy_Disabled.cpp" "DEV.cpp" "DEV_Addr.cpp" "DEV_Connector.cpp" "DEV_IO.cpp" "DLL.cpp" "DLL_Manager.cpp" "Date_Time.cpp" "Dev_Poll_Reactor.cpp" "Dirent.cpp" "Dirent_Selector.cpp" "Dump.cpp" "Dynamic.cpp" "Dynamic_Message_Strategy.cpp" "Dynamic_Service_Base.cpp" "Dynamic_Service_Dependency.cpp" "Encoding_Converter.cpp" "Encoding_Converter_Factory.cpp" "Event_Base.cpp" "Event_Handler.cpp" "Event_Handler_Handle_Timeout_Upcall.cpp" "FIFO.cpp" "FIFO_Recv.cpp" "FIFO_Recv_Msg.cpp" "FIFO_Send.cpp" "FIFO_Send_Msg.cpp" "FILE.cpp" "FILE_Addr.cpp" "FILE_Connector.cpp" "FILE_IO.cpp" "File_Lock.cpp" "Filecache.cpp" "Flag_Manip.cpp" "Framework_Component.cpp" "Functor.cpp" "Functor_String.cpp" "Get_Opt.cpp" "Handle_Ops.cpp" "Handle_Set.cpp" "Hashable.cpp" "High_Res_Timer.cpp" "ICMP_Socket.cpp" "INET_Addr.cpp" "IOStream.cpp" "IO_Cntl_Msg.cpp" "IO_SAP.cpp" "IPC_SAP.cpp" "Init_ACE.cpp" "LSOCK.cpp" "LSOCK_Acceptor.cpp" "LSOCK_CODgram.cpp" "LSOCK_Connector.cpp" "LSOCK_Dgram.cpp" "LSOCK_Stream.cpp" "Lib_Find.cpp" "Local_Memory_Pool.cpp" "Local_Name_Space.cpp" "Local_Tokens.cpp" "Lock.cpp" "Log_Category.cpp" "Log_Msg.cpp" "Log_Msg_Backend.cpp" "Log_Msg_Callback.cpp" "Log_Msg_IPC.cpp" "Log_Msg_NT_Event_Log.cpp" "Log_Msg_UNIX_Syslog.cpp" "Log_Record.cpp" "Logging_Strategy.cpp" "MEM_Acceptor.cpp" "MEM_Addr.cpp" "MEM_Connector.cpp" "MEM_IO.cpp" "MEM_SAP.cpp" "MEM_Stream.cpp" "MMAP_Memory_Pool.cpp" "Malloc.cpp" "Malloc_Allocator.cpp" "Mem_Map.cpp" "Message_Block.cpp" "Message_Queue.cpp" "Message_Queue_NT.cpp" "Message_Queue_Vx.cpp" "Method_Request.cpp" "Monitor_Admin.cpp" "Monitor_Admin_Manager.cpp" "Monitor_Base.cpp" "Monitor_Control_Action.cpp" "Monitor_Control_Types.cpp" "Monitor_Point_Registry.cpp" "Monitor_Size.cpp" "Monotonic_Time_Policy.cpp" "Msg_WFMO_Reactor.cpp" "Multihomed_INET_Addr.cpp" "Mutex.cpp" "NT_Service.cpp" "Name_Proxy.cpp" "Name_Request_Reply.cpp" "Name_Space.cpp" "Naming_Context.cpp" "Netlink_Addr.cpp" "Notification_Queue.cpp" "Notification_Strategy.cpp" "Null_Mutex.cpp" "OS_Errno.cpp" "OS_Log_Msg_Attributes.cpp" "OS_NS_Thread.cpp" "OS_NS_arpa_inet.cpp" "OS_NS_ctype.cpp" "OS_NS_devctl.cpp" "OS_NS_dirent.cpp" "OS_NS_dlfcn.cpp" "OS_NS_errno.cpp" "OS_NS_fcntl.cpp" "OS_NS_math.cpp" "OS_NS_netdb.cpp" "OS_NS_poll.cpp" "OS_NS_pwd.cpp" "OS_NS_regex.cpp" "OS_NS_signal.cpp" "OS_NS_stdio.cpp" "OS_NS_stdlib.cpp" "OS_NS_string.cpp" "OS_NS_strings.cpp" "OS_NS_stropts.cpp" "OS_NS_sys_mman.cpp" "OS_NS_sys_msg.cpp" "OS_NS_sys_resource.cpp" "OS_NS_sys_select.cpp" "OS_NS_sys_sendfile.cpp" "OS_NS_sys_shm.cpp" "OS_NS_sys_socket.cpp" "OS_NS_sys_stat.cpp" "OS_NS_sys_time.cpp" "OS_NS_sys_uio.cpp" "OS_NS_sys_utsname.cpp" "OS_NS_sys_wait.cpp" "OS_NS_time.cpp" "OS_NS_unistd.cpp" "OS_NS_wchar.cpp" "OS_NS_wctype.cpp" "OS_QoS.cpp" "OS_TLI.cpp" "OS_Thread_Adapter.cpp" "OS_main.cpp" "Obchunk.cpp" "Object_Manager.cpp" "Object_Manager_Base.cpp" "Obstack.cpp" "PI_Malloc.cpp" "POSIX_Asynch_IO.cpp" "POSIX_CB_Proactor.cpp" "POSIX_Proactor.cpp" "Pagefile_Memory_Pool.cpp" "Parse_Node.cpp" "Ping_Socket.cpp" "Pipe.cpp" "Priority_Reactor.cpp" "Proactor.cpp" "Proactor_Impl.cpp" "Process.cpp" "Process_Manager.cpp" "Process_Mutex.cpp" "Process_Semaphore.cpp" "Profile_Timer.cpp" "RW_Mutex.cpp" "RW_Process_Mutex.cpp" "RW_Thread_Mutex.cpp" "Reactor.cpp" "Reactor_Impl.cpp" "Reactor_Notification_Strategy.cpp" "Reactor_Timer_Interface.cpp" "Read_Buffer.cpp" "Recursive_Thread_Mutex.cpp" "Recyclable.cpp" "Registry.cpp" "Registry_Name_Space.cpp" "Remote_Name_Space.cpp" "Remote_Tokens.cpp" "Rtems_init.c" "SOCK.cpp" "SOCK_Acceptor.cpp" "SOCK_CODgram.cpp" "SOCK_Connector.cpp" "SOCK_Dgram.cpp" "SOCK_Dgram_Bcast.cpp" "SOCK_Dgram_Mcast.cpp" "SOCK_IO.cpp" "SOCK_Netlink.cpp" "SOCK_SEQPACK_Acceptor.cpp" "SOCK_SEQPACK_Association.cpp" "SOCK_SEQPACK_Connector.cpp" "SOCK_Stream.cpp" "SPIPE.cpp" "SPIPE_Acceptor.cpp" "SPIPE_Addr.cpp" "SPIPE_Connector.cpp" "SPIPE_Stream.cpp" "SString.cpp" "SUN_Proactor.cpp" "SV_Message.cpp" "SV_Message_Queue.cpp" "SV_Semaphore_Complex.cpp" "SV_Semaphore_Simple.cpp" "SV_Shared_Memory.cpp" "Sample_History.cpp" "Sbrk_Memory_Pool.cpp" "Sched_Params.cpp" "Select_Reactor_Base.cpp" "Semaphore.cpp" "Service_Config.cpp" "Service_Gestalt.cpp" "Service_Manager.cpp" "Service_Object.cpp" "Service_Repository.cpp" "Service_Types.cpp" "Shared_Memory.cpp" "Shared_Memory_MM.cpp" "Shared_Memory_Pool.cpp" "Shared_Memory_SV.cpp" "Shared_Object.cpp" "Sig_Adapter.cpp" "Sig_Handler.cpp" "Signal.cpp" "Sock_Connect.cpp" "Stack_Trace.cpp" "Stats.cpp" "String_Base_Const.cpp" "Svc_Conf_Lexer.cpp" "Svc_Conf_y.cpp" "Synch_Options.cpp" "System_Time.cpp" "TLI.cpp" "TLI_Acceptor.cpp" "TLI_Connector.cpp" "TLI_Stream.cpp" "TP_Reactor.cpp" "TSS_Adapter.cpp" "TTY_IO.cpp" "Task.cpp" "Thread.cpp" "Thread_Adapter.cpp" "Thread_Control.cpp" "Thread_Exit.cpp" "Thread_Hook.cpp" "Thread_Manager.cpp" "Thread_Mutex.cpp" "Thread_Semaphore.cpp" "Throughput_Stats.cpp" "Time_Policy.cpp" "Time_Value.cpp" "Timeprobe.cpp" "Token.cpp" "Token_Collection.cpp" "Token_Invariants.cpp" "Token_Manager.cpp" "Token_Request_Reply.cpp" "Trace.cpp" "UNIX_Addr.cpp" "UPIPE_Acceptor.cpp" "UPIPE_Connector.cpp" "UPIPE_Stream.cpp" "UTF16_Encoding_Converter.cpp" "UTF32_Encoding_Converter.cpp" "UTF8_Encoding_Converter.cpp" "UUID.cpp" "WFMO_Reactor.cpp" "WIN32_Asynch_IO.cpp" "WIN32_Proactor.cpp" "XML_Svc_Conf.cpp" "XTI_ATM_Mcast.cpp" "ace_wchar.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "..\lib\ACE.dll"
	-@del /f/q "$(OUTDIR)\ACE.lib"
	-@del /f/q "$(OUTDIR)\ACE.exp"
	-@del /f/q "$(OUTDIR)\ACE.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\ACE\$(NULL)" mkdir "Release\ACE"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:"..\lib\ACE.dll" /implib:"$(OUTDIR)\ACE.lib"
LINK32_OBJS= \
	"$(INTDIR)\ace.res" \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\ACE_crc32.obj" \
	"$(INTDIR)\ACE_crc_ccitt.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Assert.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\Atomic_Op.obj" \
	"$(INTDIR)\Atomic_Op_Sparc.obj" \
	"$(INTDIR)\Barrier.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Size.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\Cleanup.obj" \
	"$(INTDIR)\Codecs.obj" \
	"$(INTDIR)\Codeset_IBM1047.obj" \
	"$(INTDIR)\Codeset_Registry.obj" \
	"$(INTDIR)\Codeset_Registry_db.obj" \
	"$(INTDIR)\Condition_Attributes.obj" \
	"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Condition_Thread_Mutex.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\Dev_Poll_Reactor.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Message_Strategy.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Dynamic_Service_Dependency.obj" \
	"$(INTDIR)\Encoding_Converter.obj" \
	"$(INTDIR)\Encoding_Converter_Factory.obj" \
	"$(INTDIR)\Event_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\File_Lock.obj" \
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Flag_Manip.obj" \
	"$(INTDIR)\Framework_Component.obj" \
	"$(INTDIR)\Functor.obj" \
	"$(INTDIR)\Functor_String.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\ICMP_Socket.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Memory_Pool.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Log_Category.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\MMAP_Memory_Pool.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Message_Queue_NT.obj" \
	"$(INTDIR)\Message_Queue_Vx.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Monitor_Admin.obj" \
	"$(INTDIR)\Monitor_Admin_Manager.obj" \
	"$(INTDIR)\Monitor_Base.obj" \
	"$(INTDIR)\Monitor_Control_Action.obj" \
	"$(INTDIR)\Monitor_Control_Types.obj" \
	"$(INTDIR)\Monitor_Point_Registry.obj" \
	"$(INTDIR)\Monitor_Size.obj" \
	"$(INTDIR)\Monotonic_Time_Policy.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multihomed_INET_Addr.obj" \
	"$(INTDIR)\Mutex.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Netlink_Addr.obj" \
	"$(INTDIR)\Notification_Queue.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\Null_Mutex.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_NS_Thread.obj" \
	"$(INTDIR)\OS_NS_arpa_inet.obj" \
	"$(INTDIR)\OS_NS_ctype.obj" \
	"$(INTDIR)\OS_NS_devctl.obj" \
	"$(INTDIR)\OS_NS_dirent.obj" \
	"$(INTDIR)\OS_NS_dlfcn.obj" \
	"$(INTDIR)\OS_NS_errno.obj" \
	"$(INTDIR)\OS_NS_fcntl.obj" \
	"$(INTDIR)\OS_NS_math.obj" \
	"$(INTDIR)\OS_NS_netdb.obj" \
	"$(INTDIR)\OS_NS_poll.obj" \
	"$(INTDIR)\OS_NS_pwd.obj" \
	"$(INTDIR)\OS_NS_regex.obj" \
	"$(INTDIR)\OS_NS_signal.obj" \
	"$(INTDIR)\OS_NS_stdio.obj" \
	"$(INTDIR)\OS_NS_stdlib.obj" \
	"$(INTDIR)\OS_NS_string.obj" \
	"$(INTDIR)\OS_NS_strings.obj" \
	"$(INTDIR)\OS_NS_stropts.obj" \
	"$(INTDIR)\OS_NS_sys_mman.obj" \
	"$(INTDIR)\OS_NS_sys_msg.obj" \
	"$(INTDIR)\OS_NS_sys_resource.obj" \
	"$(INTDIR)\OS_NS_sys_select.obj" \
	"$(INTDIR)\OS_NS_sys_sendfile.obj" \
	"$(INTDIR)\OS_NS_sys_shm.obj" \
	"$(INTDIR)\OS_NS_sys_socket.obj" \
	"$(INTDIR)\OS_NS_sys_stat.obj" \
	"$(INTDIR)\OS_NS_sys_time.obj" \
	"$(INTDIR)\OS_NS_sys_uio.obj" \
	"$(INTDIR)\OS_NS_sys_utsname.obj" \
	"$(INTDIR)\OS_NS_sys_wait.obj" \
	"$(INTDIR)\OS_NS_time.obj" \
	"$(INTDIR)\OS_NS_unistd.obj" \
	"$(INTDIR)\OS_NS_wchar.obj" \
	"$(INTDIR)\OS_NS_wctype.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_main.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Object_Manager_Base.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_CB_Proactor.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Pagefile_Memory_Pool.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Ping_Socket.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Proactor_Impl.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\RW_Mutex.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\RW_Thread_Mutex.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Impl.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Reactor_Timer_Interface.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Rtems_init.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Netlink.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Association.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Connector.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\SUN_Proactor.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sbrk_Memory_Pool.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Semaphore.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Gestalt.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_Pool.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Sig_Adapter.obj" \
	"$(INTDIR)\Sig_Handler.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\Stack_Trace.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\Svc_Conf_Lexer.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\TSS_Adapter.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Thread_Mutex.obj" \
	"$(INTDIR)\Thread_Semaphore.obj" \
	"$(INTDIR)\Throughput_Stats.obj" \
	"$(INTDIR)\Time_Policy.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\UTF16_Encoding_Converter.obj" \
	"$(INTDIR)\UTF32_Encoding_Converter.obj" \
	"$(INTDIR)\UTF8_Encoding_Converter.obj" \
	"$(INTDIR)\UUID.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XTI_ATM_Mcast.obj" \
	"$(INTDIR)\ace_wchar.obj"

"..\lib\ACE.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\lib\ACE.dll.manifest" mt.exe -manifest "..\lib\ACE.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=..\lib
INTDIR=Static_Debug\ACE\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACEsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE.dep" "ACE.cpp" "ACE_crc32.cpp" "ACE_crc_ccitt.cpp" "ATM_Acceptor.cpp" "ATM_Addr.cpp" "ATM_Connector.cpp" "ATM_Params.cpp" "ATM_QoS.cpp" "ATM_Stream.cpp" "Activation_Queue.cpp" "Active_Map_Manager.cpp" "Addr.cpp" "Argv_Type_Converter.cpp" "Assert.cpp" "Asynch_IO.cpp" "Asynch_IO_Impl.cpp" "Asynch_Pseudo_Task.cpp" "Atomic_Op.cpp" "Atomic_Op_Sparc.c" "Barrier.cpp" "Base_Thread_Adapter.cpp" "Based_Pointer_Repository.cpp" "Basic_Stats.cpp" "Basic_Types.cpp" "CDR_Base.cpp" "CDR_Size.cpp" "CDR_Stream.cpp" "Capabilities.cpp" "Cleanup.cpp" "Codecs.cpp" "Codeset_IBM1047.cpp" "Codeset_Registry.cpp" "Codeset_Registry_db.cpp" "Condition_Attributes.cpp" "Condition_Recursive_Thread_Mutex.cpp" "Condition_Thread_Mutex.cpp" "Configuration.cpp" "Configuration_Import_Export.cpp" "Connection_Recycling_Strategy.cpp" "Containers.cpp" "Copy_Disabled.cpp" "DEV.cpp" "DEV_Addr.cpp" "DEV_Connector.cpp" "DEV_IO.cpp" "DLL.cpp" "DLL_Manager.cpp" "Date_Time.cpp" "Dev_Poll_Reactor.cpp" "Dirent.cpp" "Dirent_Selector.cpp" "Dump.cpp" "Dynamic.cpp" "Dynamic_Message_Strategy.cpp" "Dynamic_Service_Base.cpp" "Dynamic_Service_Dependency.cpp" "Encoding_Converter.cpp" "Encoding_Converter_Factory.cpp" "Event_Base.cpp" "Event_Handler.cpp" "Event_Handler_Handle_Timeout_Upcall.cpp" "FIFO.cpp" "FIFO_Recv.cpp" "FIFO_Recv_Msg.cpp" "FIFO_Send.cpp" "FIFO_Send_Msg.cpp" "FILE.cpp" "FILE_Addr.cpp" "FILE_Connector.cpp" "FILE_IO.cpp" "File_Lock.cpp" "Filecache.cpp" "Flag_Manip.cpp" "Framework_Component.cpp" "Functor.cpp" "Functor_String.cpp" "Get_Opt.cpp" "Handle_Ops.cpp" "Handle_Set.cpp" "Hashable.cpp" "High_Res_Timer.cpp" "ICMP_Socket.cpp" "INET_Addr.cpp" "IOStream.cpp" "IO_Cntl_Msg.cpp" "IO_SAP.cpp" "IPC_SAP.cpp" "Init_ACE.cpp" "LSOCK.cpp" "LSOCK_Acceptor.cpp" "LSOCK_CODgram.cpp" "LSOCK_Connector.cpp" "LSOCK_Dgram.cpp" "LSOCK_Stream.cpp" "Lib_Find.cpp" "Local_Memory_Pool.cpp" "Local_Name_Space.cpp" "Local_Tokens.cpp" "Lock.cpp" "Log_Category.cpp" "Log_Msg.cpp" "Log_Msg_Backend.cpp" "Log_Msg_Callback.cpp" "Log_Msg_IPC.cpp" "Log_Msg_NT_Event_Log.cpp" "Log_Msg_UNIX_Syslog.cpp" "Log_Record.cpp" "Logging_Strategy.cpp" "MEM_Acceptor.cpp" "MEM_Addr.cpp" "MEM_Connector.cpp" "MEM_IO.cpp" "MEM_SAP.cpp" "MEM_Stream.cpp" "MMAP_Memory_Pool.cpp" "Malloc.cpp" "Malloc_Allocator.cpp" "Mem_Map.cpp" "Message_Block.cpp" "Message_Queue.cpp" "Message_Queue_NT.cpp" "Message_Queue_Vx.cpp" "Method_Request.cpp" "Monitor_Admin.cpp" "Monitor_Admin_Manager.cpp" "Monitor_Base.cpp" "Monitor_Control_Action.cpp" "Monitor_Control_Types.cpp" "Monitor_Point_Registry.cpp" "Monitor_Size.cpp" "Monotonic_Time_Policy.cpp" "Msg_WFMO_Reactor.cpp" "Multihomed_INET_Addr.cpp" "Mutex.cpp" "NT_Service.cpp" "Name_Proxy.cpp" "Name_Request_Reply.cpp" "Name_Space.cpp" "Naming_Context.cpp" "Netlink_Addr.cpp" "Notification_Queue.cpp" "Notification_Strategy.cpp" "Null_Mutex.cpp" "OS_Errno.cpp" "OS_Log_Msg_Attributes.cpp" "OS_NS_Thread.cpp" "OS_NS_arpa_inet.cpp" "OS_NS_ctype.cpp" "OS_NS_devctl.cpp" "OS_NS_dirent.cpp" "OS_NS_dlfcn.cpp" "OS_NS_errno.cpp" "OS_NS_fcntl.cpp" "OS_NS_math.cpp" "OS_NS_netdb.cpp" "OS_NS_poll.cpp" "OS_NS_pwd.cpp" "OS_NS_regex.cpp" "OS_NS_signal.cpp" "OS_NS_stdio.cpp" "OS_NS_stdlib.cpp" "OS_NS_string.cpp" "OS_NS_strings.cpp" "OS_NS_stropts.cpp" "OS_NS_sys_mman.cpp" "OS_NS_sys_msg.cpp" "OS_NS_sys_resource.cpp" "OS_NS_sys_select.cpp" "OS_NS_sys_sendfile.cpp" "OS_NS_sys_shm.cpp" "OS_NS_sys_socket.cpp" "OS_NS_sys_stat.cpp" "OS_NS_sys_time.cpp" "OS_NS_sys_uio.cpp" "OS_NS_sys_utsname.cpp" "OS_NS_sys_wait.cpp" "OS_NS_time.cpp" "OS_NS_unistd.cpp" "OS_NS_wchar.cpp" "OS_NS_wctype.cpp" "OS_QoS.cpp" "OS_TLI.cpp" "OS_Thread_Adapter.cpp" "OS_main.cpp" "Obchunk.cpp" "Object_Manager.cpp" "Object_Manager_Base.cpp" "Obstack.cpp" "PI_Malloc.cpp" "POSIX_Asynch_IO.cpp" "POSIX_CB_Proactor.cpp" "POSIX_Proactor.cpp" "Pagefile_Memory_Pool.cpp" "Parse_Node.cpp" "Ping_Socket.cpp" "Pipe.cpp" "Priority_Reactor.cpp" "Proactor.cpp" "Proactor_Impl.cpp" "Process.cpp" "Process_Manager.cpp" "Process_Mutex.cpp" "Process_Semaphore.cpp" "Profile_Timer.cpp" "RW_Mutex.cpp" "RW_Process_Mutex.cpp" "RW_Thread_Mutex.cpp" "Reactor.cpp" "Reactor_Impl.cpp" "Reactor_Notification_Strategy.cpp" "Reactor_Timer_Interface.cpp" "Read_Buffer.cpp" "Recursive_Thread_Mutex.cpp" "Recyclable.cpp" "Registry.cpp" "Registry_Name_Space.cpp" "Remote_Name_Space.cpp" "Remote_Tokens.cpp" "Rtems_init.c" "SOCK.cpp" "SOCK_Acceptor.cpp" "SOCK_CODgram.cpp" "SOCK_Connector.cpp" "SOCK_Dgram.cpp" "SOCK_Dgram_Bcast.cpp" "SOCK_Dgram_Mcast.cpp" "SOCK_IO.cpp" "SOCK_Netlink.cpp" "SOCK_SEQPACK_Acceptor.cpp" "SOCK_SEQPACK_Association.cpp" "SOCK_SEQPACK_Connector.cpp" "SOCK_Stream.cpp" "SPIPE.cpp" "SPIPE_Acceptor.cpp" "SPIPE_Addr.cpp" "SPIPE_Connector.cpp" "SPIPE_Stream.cpp" "SString.cpp" "SUN_Proactor.cpp" "SV_Message.cpp" "SV_Message_Queue.cpp" "SV_Semaphore_Complex.cpp" "SV_Semaphore_Simple.cpp" "SV_Shared_Memory.cpp" "Sample_History.cpp" "Sbrk_Memory_Pool.cpp" "Sched_Params.cpp" "Select_Reactor_Base.cpp" "Semaphore.cpp" "Service_Config.cpp" "Service_Gestalt.cpp" "Service_Manager.cpp" "Service_Object.cpp" "Service_Repository.cpp" "Service_Types.cpp" "Shared_Memory.cpp" "Shared_Memory_MM.cpp" "Shared_Memory_Pool.cpp" "Shared_Memory_SV.cpp" "Shared_Object.cpp" "Sig_Adapter.cpp" "Sig_Handler.cpp" "Signal.cpp" "Sock_Connect.cpp" "Stack_Trace.cpp" "Stats.cpp" "String_Base_Const.cpp" "Svc_Conf_Lexer.cpp" "Svc_Conf_y.cpp" "Synch_Options.cpp" "System_Time.cpp" "TLI.cpp" "TLI_Acceptor.cpp" "TLI_Connector.cpp" "TLI_Stream.cpp" "TP_Reactor.cpp" "TSS_Adapter.cpp" "TTY_IO.cpp" "Task.cpp" "Thread.cpp" "Thread_Adapter.cpp" "Thread_Control.cpp" "Thread_Exit.cpp" "Thread_Hook.cpp" "Thread_Manager.cpp" "Thread_Mutex.cpp" "Thread_Semaphore.cpp" "Throughput_Stats.cpp" "Time_Policy.cpp" "Time_Value.cpp" "Timeprobe.cpp" "Token.cpp" "Token_Collection.cpp" "Token_Invariants.cpp" "Token_Manager.cpp" "Token_Request_Reply.cpp" "Trace.cpp" "UNIX_Addr.cpp" "UPIPE_Acceptor.cpp" "UPIPE_Connector.cpp" "UPIPE_Stream.cpp" "UTF16_Encoding_Converter.cpp" "UTF32_Encoding_Converter.cpp" "UTF8_Encoding_Converter.cpp" "UUID.cpp" "WFMO_Reactor.cpp" "WIN32_Asynch_IO.cpp" "WIN32_Proactor.cpp" "XML_Svc_Conf.cpp" "XTI_ATM_Mcast.cpp" "ace_wchar.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACEsd.lib"
	-@del /f/q "$(OUTDIR)\ACEsd.exp"
	-@del /f/q "$(OUTDIR)\ACEsd.ilk"
	-@del /f/q "..\lib\ACEsd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\ACE\$(NULL)" mkdir "Static_Debug\ACE"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4456 /wd4459 /wd4458 /Fd"..\lib\ACEsd.pdb" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\lib\ACEsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\ACE_crc32.obj" \
	"$(INTDIR)\ACE_crc_ccitt.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Assert.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\Atomic_Op.obj" \
	"$(INTDIR)\Atomic_Op_Sparc.obj" \
	"$(INTDIR)\Barrier.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Size.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\Cleanup.obj" \
	"$(INTDIR)\Codecs.obj" \
	"$(INTDIR)\Codeset_IBM1047.obj" \
	"$(INTDIR)\Codeset_Registry.obj" \
	"$(INTDIR)\Codeset_Registry_db.obj" \
	"$(INTDIR)\Condition_Attributes.obj" \
	"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Condition_Thread_Mutex.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\Dev_Poll_Reactor.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Message_Strategy.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Dynamic_Service_Dependency.obj" \
	"$(INTDIR)\Encoding_Converter.obj" \
	"$(INTDIR)\Encoding_Converter_Factory.obj" \
	"$(INTDIR)\Event_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\File_Lock.obj" \
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Flag_Manip.obj" \
	"$(INTDIR)\Framework_Component.obj" \
	"$(INTDIR)\Functor.obj" \
	"$(INTDIR)\Functor_String.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\ICMP_Socket.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Memory_Pool.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Log_Category.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\MMAP_Memory_Pool.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Message_Queue_NT.obj" \
	"$(INTDIR)\Message_Queue_Vx.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Monitor_Admin.obj" \
	"$(INTDIR)\Monitor_Admin_Manager.obj" \
	"$(INTDIR)\Monitor_Base.obj" \
	"$(INTDIR)\Monitor_Control_Action.obj" \
	"$(INTDIR)\Monitor_Control_Types.obj" \
	"$(INTDIR)\Monitor_Point_Registry.obj" \
	"$(INTDIR)\Monitor_Size.obj" \
	"$(INTDIR)\Monotonic_Time_Policy.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multihomed_INET_Addr.obj" \
	"$(INTDIR)\Mutex.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Netlink_Addr.obj" \
	"$(INTDIR)\Notification_Queue.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\Null_Mutex.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_NS_Thread.obj" \
	"$(INTDIR)\OS_NS_arpa_inet.obj" \
	"$(INTDIR)\OS_NS_ctype.obj" \
	"$(INTDIR)\OS_NS_devctl.obj" \
	"$(INTDIR)\OS_NS_dirent.obj" \
	"$(INTDIR)\OS_NS_dlfcn.obj" \
	"$(INTDIR)\OS_NS_errno.obj" \
	"$(INTDIR)\OS_NS_fcntl.obj" \
	"$(INTDIR)\OS_NS_math.obj" \
	"$(INTDIR)\OS_NS_netdb.obj" \
	"$(INTDIR)\OS_NS_poll.obj" \
	"$(INTDIR)\OS_NS_pwd.obj" \
	"$(INTDIR)\OS_NS_regex.obj" \
	"$(INTDIR)\OS_NS_signal.obj" \
	"$(INTDIR)\OS_NS_stdio.obj" \
	"$(INTDIR)\OS_NS_stdlib.obj" \
	"$(INTDIR)\OS_NS_string.obj" \
	"$(INTDIR)\OS_NS_strings.obj" \
	"$(INTDIR)\OS_NS_stropts.obj" \
	"$(INTDIR)\OS_NS_sys_mman.obj" \
	"$(INTDIR)\OS_NS_sys_msg.obj" \
	"$(INTDIR)\OS_NS_sys_resource.obj" \
	"$(INTDIR)\OS_NS_sys_select.obj" \
	"$(INTDIR)\OS_NS_sys_sendfile.obj" \
	"$(INTDIR)\OS_NS_sys_shm.obj" \
	"$(INTDIR)\OS_NS_sys_socket.obj" \
	"$(INTDIR)\OS_NS_sys_stat.obj" \
	"$(INTDIR)\OS_NS_sys_time.obj" \
	"$(INTDIR)\OS_NS_sys_uio.obj" \
	"$(INTDIR)\OS_NS_sys_utsname.obj" \
	"$(INTDIR)\OS_NS_sys_wait.obj" \
	"$(INTDIR)\OS_NS_time.obj" \
	"$(INTDIR)\OS_NS_unistd.obj" \
	"$(INTDIR)\OS_NS_wchar.obj" \
	"$(INTDIR)\OS_NS_wctype.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_main.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Object_Manager_Base.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_CB_Proactor.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Pagefile_Memory_Pool.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Ping_Socket.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Proactor_Impl.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\RW_Mutex.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\RW_Thread_Mutex.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Impl.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Reactor_Timer_Interface.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Rtems_init.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Netlink.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Association.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Connector.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\SUN_Proactor.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sbrk_Memory_Pool.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Semaphore.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Gestalt.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_Pool.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Sig_Adapter.obj" \
	"$(INTDIR)\Sig_Handler.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\Stack_Trace.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\Svc_Conf_Lexer.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\TSS_Adapter.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Thread_Mutex.obj" \
	"$(INTDIR)\Thread_Semaphore.obj" \
	"$(INTDIR)\Throughput_Stats.obj" \
	"$(INTDIR)\Time_Policy.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\UTF16_Encoding_Converter.obj" \
	"$(INTDIR)\UTF32_Encoding_Converter.obj" \
	"$(INTDIR)\UTF8_Encoding_Converter.obj" \
	"$(INTDIR)\UUID.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XTI_ATM_Mcast.obj" \
	"$(INTDIR)\ace_wchar.obj"

"$(OUTDIR)\ACEsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACEsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACEsd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=..\lib
INTDIR=Static_Release\ACE\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACEs.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE.dep" "ACE.cpp" "ACE_crc32.cpp" "ACE_crc_ccitt.cpp" "ATM_Acceptor.cpp" "ATM_Addr.cpp" "ATM_Connector.cpp" "ATM_Params.cpp" "ATM_QoS.cpp" "ATM_Stream.cpp" "Activation_Queue.cpp" "Active_Map_Manager.cpp" "Addr.cpp" "Argv_Type_Converter.cpp" "Assert.cpp" "Asynch_IO.cpp" "Asynch_IO_Impl.cpp" "Asynch_Pseudo_Task.cpp" "Atomic_Op.cpp" "Atomic_Op_Sparc.c" "Barrier.cpp" "Base_Thread_Adapter.cpp" "Based_Pointer_Repository.cpp" "Basic_Stats.cpp" "Basic_Types.cpp" "CDR_Base.cpp" "CDR_Size.cpp" "CDR_Stream.cpp" "Capabilities.cpp" "Cleanup.cpp" "Codecs.cpp" "Codeset_IBM1047.cpp" "Codeset_Registry.cpp" "Codeset_Registry_db.cpp" "Condition_Attributes.cpp" "Condition_Recursive_Thread_Mutex.cpp" "Condition_Thread_Mutex.cpp" "Configuration.cpp" "Configuration_Import_Export.cpp" "Connection_Recycling_Strategy.cpp" "Containers.cpp" "Copy_Disabled.cpp" "DEV.cpp" "DEV_Addr.cpp" "DEV_Connector.cpp" "DEV_IO.cpp" "DLL.cpp" "DLL_Manager.cpp" "Date_Time.cpp" "Dev_Poll_Reactor.cpp" "Dirent.cpp" "Dirent_Selector.cpp" "Dump.cpp" "Dynamic.cpp" "Dynamic_Message_Strategy.cpp" "Dynamic_Service_Base.cpp" "Dynamic_Service_Dependency.cpp" "Encoding_Converter.cpp" "Encoding_Converter_Factory.cpp" "Event_Base.cpp" "Event_Handler.cpp" "Event_Handler_Handle_Timeout_Upcall.cpp" "FIFO.cpp" "FIFO_Recv.cpp" "FIFO_Recv_Msg.cpp" "FIFO_Send.cpp" "FIFO_Send_Msg.cpp" "FILE.cpp" "FILE_Addr.cpp" "FILE_Connector.cpp" "FILE_IO.cpp" "File_Lock.cpp" "Filecache.cpp" "Flag_Manip.cpp" "Framework_Component.cpp" "Functor.cpp" "Functor_String.cpp" "Get_Opt.cpp" "Handle_Ops.cpp" "Handle_Set.cpp" "Hashable.cpp" "High_Res_Timer.cpp" "ICMP_Socket.cpp" "INET_Addr.cpp" "IOStream.cpp" "IO_Cntl_Msg.cpp" "IO_SAP.cpp" "IPC_SAP.cpp" "Init_ACE.cpp" "LSOCK.cpp" "LSOCK_Acceptor.cpp" "LSOCK_CODgram.cpp" "LSOCK_Connector.cpp" "LSOCK_Dgram.cpp" "LSOCK_Stream.cpp" "Lib_Find.cpp" "Local_Memory_Pool.cpp" "Local_Name_Space.cpp" "Local_Tokens.cpp" "Lock.cpp" "Log_Category.cpp" "Log_Msg.cpp" "Log_Msg_Backend.cpp" "Log_Msg_Callback.cpp" "Log_Msg_IPC.cpp" "Log_Msg_NT_Event_Log.cpp" "Log_Msg_UNIX_Syslog.cpp" "Log_Record.cpp" "Logging_Strategy.cpp" "MEM_Acceptor.cpp" "MEM_Addr.cpp" "MEM_Connector.cpp" "MEM_IO.cpp" "MEM_SAP.cpp" "MEM_Stream.cpp" "MMAP_Memory_Pool.cpp" "Malloc.cpp" "Malloc_Allocator.cpp" "Mem_Map.cpp" "Message_Block.cpp" "Message_Queue.cpp" "Message_Queue_NT.cpp" "Message_Queue_Vx.cpp" "Method_Request.cpp" "Monitor_Admin.cpp" "Monitor_Admin_Manager.cpp" "Monitor_Base.cpp" "Monitor_Control_Action.cpp" "Monitor_Control_Types.cpp" "Monitor_Point_Registry.cpp" "Monitor_Size.cpp" "Monotonic_Time_Policy.cpp" "Msg_WFMO_Reactor.cpp" "Multihomed_INET_Addr.cpp" "Mutex.cpp" "NT_Service.cpp" "Name_Proxy.cpp" "Name_Request_Reply.cpp" "Name_Space.cpp" "Naming_Context.cpp" "Netlink_Addr.cpp" "Notification_Queue.cpp" "Notification_Strategy.cpp" "Null_Mutex.cpp" "OS_Errno.cpp" "OS_Log_Msg_Attributes.cpp" "OS_NS_Thread.cpp" "OS_NS_arpa_inet.cpp" "OS_NS_ctype.cpp" "OS_NS_devctl.cpp" "OS_NS_dirent.cpp" "OS_NS_dlfcn.cpp" "OS_NS_errno.cpp" "OS_NS_fcntl.cpp" "OS_NS_math.cpp" "OS_NS_netdb.cpp" "OS_NS_poll.cpp" "OS_NS_pwd.cpp" "OS_NS_regex.cpp" "OS_NS_signal.cpp" "OS_NS_stdio.cpp" "OS_NS_stdlib.cpp" "OS_NS_string.cpp" "OS_NS_strings.cpp" "OS_NS_stropts.cpp" "OS_NS_sys_mman.cpp" "OS_NS_sys_msg.cpp" "OS_NS_sys_resource.cpp" "OS_NS_sys_select.cpp" "OS_NS_sys_sendfile.cpp" "OS_NS_sys_shm.cpp" "OS_NS_sys_socket.cpp" "OS_NS_sys_stat.cpp" "OS_NS_sys_time.cpp" "OS_NS_sys_uio.cpp" "OS_NS_sys_utsname.cpp" "OS_NS_sys_wait.cpp" "OS_NS_time.cpp" "OS_NS_unistd.cpp" "OS_NS_wchar.cpp" "OS_NS_wctype.cpp" "OS_QoS.cpp" "OS_TLI.cpp" "OS_Thread_Adapter.cpp" "OS_main.cpp" "Obchunk.cpp" "Object_Manager.cpp" "Object_Manager_Base.cpp" "Obstack.cpp" "PI_Malloc.cpp" "POSIX_Asynch_IO.cpp" "POSIX_CB_Proactor.cpp" "POSIX_Proactor.cpp" "Pagefile_Memory_Pool.cpp" "Parse_Node.cpp" "Ping_Socket.cpp" "Pipe.cpp" "Priority_Reactor.cpp" "Proactor.cpp" "Proactor_Impl.cpp" "Process.cpp" "Process_Manager.cpp" "Process_Mutex.cpp" "Process_Semaphore.cpp" "Profile_Timer.cpp" "RW_Mutex.cpp" "RW_Process_Mutex.cpp" "RW_Thread_Mutex.cpp" "Reactor.cpp" "Reactor_Impl.cpp" "Reactor_Notification_Strategy.cpp" "Reactor_Timer_Interface.cpp" "Read_Buffer.cpp" "Recursive_Thread_Mutex.cpp" "Recyclable.cpp" "Registry.cpp" "Registry_Name_Space.cpp" "Remote_Name_Space.cpp" "Remote_Tokens.cpp" "Rtems_init.c" "SOCK.cpp" "SOCK_Acceptor.cpp" "SOCK_CODgram.cpp" "SOCK_Connector.cpp" "SOCK_Dgram.cpp" "SOCK_Dgram_Bcast.cpp" "SOCK_Dgram_Mcast.cpp" "SOCK_IO.cpp" "SOCK_Netlink.cpp" "SOCK_SEQPACK_Acceptor.cpp" "SOCK_SEQPACK_Association.cpp" "SOCK_SEQPACK_Connector.cpp" "SOCK_Stream.cpp" "SPIPE.cpp" "SPIPE_Acceptor.cpp" "SPIPE_Addr.cpp" "SPIPE_Connector.cpp" "SPIPE_Stream.cpp" "SString.cpp" "SUN_Proactor.cpp" "SV_Message.cpp" "SV_Message_Queue.cpp" "SV_Semaphore_Complex.cpp" "SV_Semaphore_Simple.cpp" "SV_Shared_Memory.cpp" "Sample_History.cpp" "Sbrk_Memory_Pool.cpp" "Sched_Params.cpp" "Select_Reactor_Base.cpp" "Semaphore.cpp" "Service_Config.cpp" "Service_Gestalt.cpp" "Service_Manager.cpp" "Service_Object.cpp" "Service_Repository.cpp" "Service_Types.cpp" "Shared_Memory.cpp" "Shared_Memory_MM.cpp" "Shared_Memory_Pool.cpp" "Shared_Memory_SV.cpp" "Shared_Object.cpp" "Sig_Adapter.cpp" "Sig_Handler.cpp" "Signal.cpp" "Sock_Connect.cpp" "Stack_Trace.cpp" "Stats.cpp" "String_Base_Const.cpp" "Svc_Conf_Lexer.cpp" "Svc_Conf_y.cpp" "Synch_Options.cpp" "System_Time.cpp" "TLI.cpp" "TLI_Acceptor.cpp" "TLI_Connector.cpp" "TLI_Stream.cpp" "TP_Reactor.cpp" "TSS_Adapter.cpp" "TTY_IO.cpp" "Task.cpp" "Thread.cpp" "Thread_Adapter.cpp" "Thread_Control.cpp" "Thread_Exit.cpp" "Thread_Hook.cpp" "Thread_Manager.cpp" "Thread_Mutex.cpp" "Thread_Semaphore.cpp" "Throughput_Stats.cpp" "Time_Policy.cpp" "Time_Value.cpp" "Timeprobe.cpp" "Token.cpp" "Token_Collection.cpp" "Token_Invariants.cpp" "Token_Manager.cpp" "Token_Request_Reply.cpp" "Trace.cpp" "UNIX_Addr.cpp" "UPIPE_Acceptor.cpp" "UPIPE_Connector.cpp" "UPIPE_Stream.cpp" "UTF16_Encoding_Converter.cpp" "UTF32_Encoding_Converter.cpp" "UTF8_Encoding_Converter.cpp" "UUID.cpp" "WFMO_Reactor.cpp" "WIN32_Asynch_IO.cpp" "WIN32_Proactor.cpp" "XML_Svc_Conf.cpp" "XTI_ATM_Mcast.cpp" "ace_wchar.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACEs.lib"
	-@del /f/q "$(OUTDIR)\ACEs.exp"
	-@del /f/q "$(OUTDIR)\ACEs.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\ACE\$(NULL)" mkdir "Static_Release\ACE"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\lib\ACEs.lib"
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\ACE_crc32.obj" \
	"$(INTDIR)\ACE_crc_ccitt.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Assert.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\Atomic_Op.obj" \
	"$(INTDIR)\Atomic_Op_Sparc.obj" \
	"$(INTDIR)\Barrier.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Size.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\Cleanup.obj" \
	"$(INTDIR)\Codecs.obj" \
	"$(INTDIR)\Codeset_IBM1047.obj" \
	"$(INTDIR)\Codeset_Registry.obj" \
	"$(INTDIR)\Codeset_Registry_db.obj" \
	"$(INTDIR)\Condition_Attributes.obj" \
	"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Condition_Thread_Mutex.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\Dev_Poll_Reactor.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Message_Strategy.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Dynamic_Service_Dependency.obj" \
	"$(INTDIR)\Encoding_Converter.obj" \
	"$(INTDIR)\Encoding_Converter_Factory.obj" \
	"$(INTDIR)\Event_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\File_Lock.obj" \
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Flag_Manip.obj" \
	"$(INTDIR)\Framework_Component.obj" \
	"$(INTDIR)\Functor.obj" \
	"$(INTDIR)\Functor_String.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\ICMP_Socket.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Memory_Pool.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Log_Category.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\MMAP_Memory_Pool.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Message_Queue_NT.obj" \
	"$(INTDIR)\Message_Queue_Vx.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Monitor_Admin.obj" \
	"$(INTDIR)\Monitor_Admin_Manager.obj" \
	"$(INTDIR)\Monitor_Base.obj" \
	"$(INTDIR)\Monitor_Control_Action.obj" \
	"$(INTDIR)\Monitor_Control_Types.obj" \
	"$(INTDIR)\Monitor_Point_Registry.obj" \
	"$(INTDIR)\Monitor_Size.obj" \
	"$(INTDIR)\Monotonic_Time_Policy.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multihomed_INET_Addr.obj" \
	"$(INTDIR)\Mutex.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Netlink_Addr.obj" \
	"$(INTDIR)\Notification_Queue.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\Null_Mutex.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_NS_Thread.obj" \
	"$(INTDIR)\OS_NS_arpa_inet.obj" \
	"$(INTDIR)\OS_NS_ctype.obj" \
	"$(INTDIR)\OS_NS_devctl.obj" \
	"$(INTDIR)\OS_NS_dirent.obj" \
	"$(INTDIR)\OS_NS_dlfcn.obj" \
	"$(INTDIR)\OS_NS_errno.obj" \
	"$(INTDIR)\OS_NS_fcntl.obj" \
	"$(INTDIR)\OS_NS_math.obj" \
	"$(INTDIR)\OS_NS_netdb.obj" \
	"$(INTDIR)\OS_NS_poll.obj" \
	"$(INTDIR)\OS_NS_pwd.obj" \
	"$(INTDIR)\OS_NS_regex.obj" \
	"$(INTDIR)\OS_NS_signal.obj" \
	"$(INTDIR)\OS_NS_stdio.obj" \
	"$(INTDIR)\OS_NS_stdlib.obj" \
	"$(INTDIR)\OS_NS_string.obj" \
	"$(INTDIR)\OS_NS_strings.obj" \
	"$(INTDIR)\OS_NS_stropts.obj" \
	"$(INTDIR)\OS_NS_sys_mman.obj" \
	"$(INTDIR)\OS_NS_sys_msg.obj" \
	"$(INTDIR)\OS_NS_sys_resource.obj" \
	"$(INTDIR)\OS_NS_sys_select.obj" \
	"$(INTDIR)\OS_NS_sys_sendfile.obj" \
	"$(INTDIR)\OS_NS_sys_shm.obj" \
	"$(INTDIR)\OS_NS_sys_socket.obj" \
	"$(INTDIR)\OS_NS_sys_stat.obj" \
	"$(INTDIR)\OS_NS_sys_time.obj" \
	"$(INTDIR)\OS_NS_sys_uio.obj" \
	"$(INTDIR)\OS_NS_sys_utsname.obj" \
	"$(INTDIR)\OS_NS_sys_wait.obj" \
	"$(INTDIR)\OS_NS_time.obj" \
	"$(INTDIR)\OS_NS_unistd.obj" \
	"$(INTDIR)\OS_NS_wchar.obj" \
	"$(INTDIR)\OS_NS_wctype.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_main.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Object_Manager_Base.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_CB_Proactor.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Pagefile_Memory_Pool.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Ping_Socket.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Proactor_Impl.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\RW_Mutex.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\RW_Thread_Mutex.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Impl.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Reactor_Timer_Interface.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recursive_Thread_Mutex.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Rtems_init.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Netlink.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Association.obj" \
	"$(INTDIR)\SOCK_SEQPACK_Connector.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\SUN_Proactor.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sbrk_Memory_Pool.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Semaphore.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Gestalt.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_Pool.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Sig_Adapter.obj" \
	"$(INTDIR)\Sig_Handler.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\Stack_Trace.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\Svc_Conf_Lexer.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\TSS_Adapter.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Thread_Mutex.obj" \
	"$(INTDIR)\Thread_Semaphore.obj" \
	"$(INTDIR)\Throughput_Stats.obj" \
	"$(INTDIR)\Time_Policy.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\UTF16_Encoding_Converter.obj" \
	"$(INTDIR)\UTF32_Encoding_Converter.obj" \
	"$(INTDIR)\UTF8_Encoding_Converter.obj" \
	"$(INTDIR)\UUID.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XTI_ATM_Mcast.obj" \
	"$(INTDIR)\ace_wchar.obj"

"$(OUTDIR)\ACEs.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACEs.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACEs.lib.manifest" -outputresource:$@;2

!ENDIF

CLEAN :
	-@del /f/s/q "$(INTDIR)"

"$(OUTDIR)" ::
	if not exist "$(OUTDIR)\$(NULL)" mkdir "$(OUTDIR)"

.c{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.ACE.dep")
!INCLUDE "Makefile.ACE.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="ACE.cpp"

"$(INTDIR)\ACE.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ACE.obj" $(SOURCE)

SOURCE="ACE_crc32.cpp"

"$(INTDIR)\ACE_crc32.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ACE_crc32.obj" $(SOURCE)

SOURCE="ACE_crc_ccitt.cpp"

"$(INTDIR)\ACE_crc_ccitt.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ACE_crc_ccitt.obj" $(SOURCE)

SOURCE="ATM_Acceptor.cpp"

"$(INTDIR)\ATM_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_Acceptor.obj" $(SOURCE)

SOURCE="ATM_Addr.cpp"

"$(INTDIR)\ATM_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_Addr.obj" $(SOURCE)

SOURCE="ATM_Connector.cpp"

"$(INTDIR)\ATM_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_Connector.obj" $(SOURCE)

SOURCE="ATM_Params.cpp"

"$(INTDIR)\ATM_Params.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_Params.obj" $(SOURCE)

SOURCE="ATM_QoS.cpp"

"$(INTDIR)\ATM_QoS.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_QoS.obj" $(SOURCE)

SOURCE="ATM_Stream.cpp"

"$(INTDIR)\ATM_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ATM_Stream.obj" $(SOURCE)

SOURCE="Activation_Queue.cpp"

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Activation_Queue.obj" $(SOURCE)

SOURCE="Active_Map_Manager.cpp"

"$(INTDIR)\Active_Map_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Active_Map_Manager.obj" $(SOURCE)

SOURCE="Addr.cpp"

"$(INTDIR)\Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Addr.obj" $(SOURCE)

SOURCE="Argv_Type_Converter.cpp"

"$(INTDIR)\Argv_Type_Converter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Argv_Type_Converter.obj" $(SOURCE)

SOURCE="Assert.cpp"

"$(INTDIR)\Assert.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Assert.obj" $(SOURCE)

SOURCE="Asynch_IO.cpp"

"$(INTDIR)\Asynch_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Asynch_IO.obj" $(SOURCE)

SOURCE="Asynch_IO_Impl.cpp"

"$(INTDIR)\Asynch_IO_Impl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Asynch_IO_Impl.obj" $(SOURCE)

SOURCE="Asynch_Pseudo_Task.cpp"

"$(INTDIR)\Asynch_Pseudo_Task.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Asynch_Pseudo_Task.obj" $(SOURCE)

SOURCE="Atomic_Op.cpp"

"$(INTDIR)\Atomic_Op.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Atomic_Op.obj" $(SOURCE)

SOURCE="Atomic_Op_Sparc.c"

"$(INTDIR)\Atomic_Op_Sparc.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Atomic_Op_Sparc.obj" $(SOURCE)

SOURCE="Barrier.cpp"

"$(INTDIR)\Barrier.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Barrier.obj" $(SOURCE)

SOURCE="Base_Thread_Adapter.cpp"

"$(INTDIR)\Base_Thread_Adapter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Base_Thread_Adapter.obj" $(SOURCE)

SOURCE="Based_Pointer_Repository.cpp"

"$(INTDIR)\Based_Pointer_Repository.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Based_Pointer_Repository.obj" $(SOURCE)

SOURCE="Basic_Stats.cpp"

"$(INTDIR)\Basic_Stats.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Basic_Stats.obj" $(SOURCE)

SOURCE="Basic_Types.cpp"

"$(INTDIR)\Basic_Types.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Basic_Types.obj" $(SOURCE)

SOURCE="CDR_Base.cpp"

"$(INTDIR)\CDR_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\CDR_Base.obj" $(SOURCE)

SOURCE="CDR_Size.cpp"

"$(INTDIR)\CDR_Size.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\CDR_Size.obj" $(SOURCE)

SOURCE="CDR_Stream.cpp"

"$(INTDIR)\CDR_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\CDR_Stream.obj" $(SOURCE)

SOURCE="Capabilities.cpp"

"$(INTDIR)\Capabilities.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Capabilities.obj" $(SOURCE)

SOURCE="Cleanup.cpp"

"$(INTDIR)\Cleanup.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Cleanup.obj" $(SOURCE)

SOURCE="Codecs.cpp"

"$(INTDIR)\Codecs.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Codecs.obj" $(SOURCE)

SOURCE="Codeset_IBM1047.cpp"

"$(INTDIR)\Codeset_IBM1047.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Codeset_IBM1047.obj" $(SOURCE)

SOURCE="Codeset_Registry.cpp"

"$(INTDIR)\Codeset_Registry.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Codeset_Registry.obj" $(SOURCE)

SOURCE="Codeset_Registry_db.cpp"

"$(INTDIR)\Codeset_Registry_db.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Codeset_Registry_db.obj" $(SOURCE)

SOURCE="Condition_Attributes.cpp"

"$(INTDIR)\Condition_Attributes.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Condition_Attributes.obj" $(SOURCE)

SOURCE="Condition_Recursive_Thread_Mutex.cpp"

"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Condition_Recursive_Thread_Mutex.obj" $(SOURCE)

SOURCE="Condition_Thread_Mutex.cpp"

"$(INTDIR)\Condition_Thread_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Condition_Thread_Mutex.obj" $(SOURCE)

SOURCE="Configuration.cpp"

"$(INTDIR)\Configuration.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Configuration.obj" $(SOURCE)

SOURCE="Configuration_Import_Export.cpp"

"$(INTDIR)\Configuration_Import_Export.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Configuration_Import_Export.obj" $(SOURCE)

SOURCE="Connection_Recycling_Strategy.cpp"

"$(INTDIR)\Connection_Recycling_Strategy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Connection_Recycling_Strategy.obj" $(SOURCE)

SOURCE="Containers.cpp"

"$(INTDIR)\Containers.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Containers.obj" $(SOURCE)

SOURCE="Copy_Disabled.cpp"

"$(INTDIR)\Copy_Disabled.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Copy_Disabled.obj" $(SOURCE)

SOURCE="DEV.cpp"

"$(INTDIR)\DEV.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DEV.obj" $(SOURCE)

SOURCE="DEV_Addr.cpp"

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DEV_Addr.obj" $(SOURCE)

SOURCE="DEV_Connector.cpp"

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DEV_Connector.obj" $(SOURCE)

SOURCE="DEV_IO.cpp"

"$(INTDIR)\DEV_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DEV_IO.obj" $(SOURCE)

SOURCE="DLL.cpp"

"$(INTDIR)\DLL.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DLL.obj" $(SOURCE)

SOURCE="DLL_Manager.cpp"

"$(INTDIR)\DLL_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DLL_Manager.obj" $(SOURCE)

SOURCE="Date_Time.cpp"

"$(INTDIR)\Date_Time.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Date_Time.obj" $(SOURCE)

SOURCE="Dev_Poll_Reactor.cpp"

"$(INTDIR)\Dev_Poll_Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dev_Poll_Reactor.obj" $(SOURCE)

SOURCE="Dirent.cpp"

"$(INTDIR)\Dirent.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dirent.obj" $(SOURCE)

SOURCE="Dirent_Selector.cpp"

"$(INTDIR)\Dirent_Selector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dirent_Selector.obj" $(SOURCE)

SOURCE="Dump.cpp"

"$(INTDIR)\Dump.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dump.obj" $(SOURCE)

SOURCE="Dynamic.cpp"

"$(INTDIR)\Dynamic.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dynamic.obj" $(SOURCE)

SOURCE="Dynamic_Message_Strategy.cpp"

"$(INTDIR)\Dynamic_Message_Strategy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dynamic_Message_Strategy.obj" $(SOURCE)

SOURCE="Dynamic_Service_Base.cpp"

"$(INTDIR)\Dynamic_Service_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dynamic_Service_Base.obj" $(SOURCE)

SOURCE="Dynamic_Service_Dependency.cpp"

"$(INTDIR)\Dynamic_Service_Dependency.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Dynamic_Service_Dependency.obj" $(SOURCE)

SOURCE="Encoding_Converter.cpp"

"$(INTDIR)\Encoding_Converter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Encoding_Converter.obj" $(SOURCE)

SOURCE="Encoding_Converter_Factory.cpp"

"$(INTDIR)\Encoding_Converter_Factory.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Encoding_Converter_Factory.obj" $(SOURCE)

SOURCE="Event_Base.cpp"

"$(INTDIR)\Event_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Event_Base.obj" $(SOURCE)

SOURCE="Event_Handler.cpp"

"$(INTDIR)\Event_Handler.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Event_Handler.obj" $(SOURCE)

SOURCE="Event_Handler_Handle_Timeout_Upcall.cpp"

"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Event_Handler_Handle_Timeout_Upcall.obj" $(SOURCE)

SOURCE="FIFO.cpp"

"$(INTDIR)\FIFO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FIFO.obj" $(SOURCE)

SOURCE="FIFO_Recv.cpp"

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FIFO_Recv.obj" $(SOURCE)

SOURCE="FIFO_Recv_Msg.cpp"

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FIFO_Recv_Msg.obj" $(SOURCE)

SOURCE="FIFO_Send.cpp"

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FIFO_Send.obj" $(SOURCE)

SOURCE="FIFO_Send_Msg.cpp"

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FIFO_Send_Msg.obj" $(SOURCE)

SOURCE="FILE.cpp"

"$(INTDIR)\FILE.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FILE.obj" $(SOURCE)

SOURCE="FILE_Addr.cpp"

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FILE_Addr.obj" $(SOURCE)

SOURCE="FILE_Connector.cpp"

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FILE_Connector.obj" $(SOURCE)

SOURCE="FILE_IO.cpp"

"$(INTDIR)\FILE_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FILE_IO.obj" $(SOURCE)

SOURCE="File_Lock.cpp"

"$(INTDIR)\File_Lock.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\File_Lock.obj" $(SOURCE)

SOURCE="Filecache.cpp"

"$(INTDIR)\Filecache.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Filecache.obj" $(SOURCE)

SOURCE="Flag_Manip.cpp"

"$(INTDIR)\Flag_Manip.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Flag_Manip.obj" $(SOURCE)

SOURCE="Framework_Component.cpp"

"$(INTDIR)\Framework_Component.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Framework_Component.obj" $(SOURCE)

SOURCE="Functor.cpp"

"$(INTDIR)\Functor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Functor.obj" $(SOURCE)

SOURCE="Functor_String.cpp"

"$(INTDIR)\Functor_String.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Functor_String.obj" $(SOURCE)

SOURCE="Get_Opt.cpp"

"$(INTDIR)\Get_Opt.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Get_Opt.obj" $(SOURCE)

SOURCE="Handle_Ops.cpp"

"$(INTDIR)\Handle_Ops.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Handle_Ops.obj" $(SOURCE)

SOURCE="Handle_Set.cpp"

"$(INTDIR)\Handle_Set.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Handle_Set.obj" $(SOURCE)

SOURCE="Hashable.cpp"

"$(INTDIR)\Hashable.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Hashable.obj" $(SOURCE)

SOURCE="High_Res_Timer.cpp"

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\High_Res_Timer.obj" $(SOURCE)

SOURCE="ICMP_Socket.cpp"

"$(INTDIR)\ICMP_Socket.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ICMP_Socket.obj" $(SOURCE)

SOURCE="INET_Addr.cpp"

"$(INTDIR)\INET_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\INET_Addr.obj" $(SOURCE)

SOURCE="IOStream.cpp"

"$(INTDIR)\IOStream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\IOStream.obj" $(SOURCE)

SOURCE="IO_Cntl_Msg.cpp"

"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\IO_Cntl_Msg.obj" $(SOURCE)

SOURCE="IO_SAP.cpp"

"$(INTDIR)\IO_SAP.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\IO_SAP.obj" $(SOURCE)

SOURCE="IPC_SAP.cpp"

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\IPC_SAP.obj" $(SOURCE)

SOURCE="Init_ACE.cpp"

"$(INTDIR)\Init_ACE.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Init_ACE.obj" $(SOURCE)

SOURCE="LSOCK.cpp"

"$(INTDIR)\LSOCK.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK.obj" $(SOURCE)

SOURCE="LSOCK_Acceptor.cpp"

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK_Acceptor.obj" $(SOURCE)

SOURCE="LSOCK_CODgram.cpp"

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK_CODgram.obj" $(SOURCE)

SOURCE="LSOCK_Connector.cpp"

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK_Connector.obj" $(SOURCE)

SOURCE="LSOCK_Dgram.cpp"

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK_Dgram.obj" $(SOURCE)

SOURCE="LSOCK_Stream.cpp"

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\LSOCK_Stream.obj" $(SOURCE)

SOURCE="Lib_Find.cpp"

"$(INTDIR)\Lib_Find.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Lib_Find.obj" $(SOURCE)

SOURCE="Local_Memory_Pool.cpp"

"$(INTDIR)\Local_Memory_Pool.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Local_Memory_Pool.obj" $(SOURCE)

SOURCE="Local_Name_Space.cpp"

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Local_Name_Space.obj" $(SOURCE)

SOURCE="Local_Tokens.cpp"

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Local_Tokens.obj" $(SOURCE)

SOURCE="Lock.cpp"

"$(INTDIR)\Lock.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Lock.obj" $(SOURCE)

SOURCE="Log_Category.cpp"

"$(INTDIR)\Log_Category.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Category.obj" $(SOURCE)

SOURCE="Log_Msg.cpp"

"$(INTDIR)\Log_Msg.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg.obj" $(SOURCE)

SOURCE="Log_Msg_Backend.cpp"

"$(INTDIR)\Log_Msg_Backend.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg_Backend.obj" $(SOURCE)

SOURCE="Log_Msg_Callback.cpp"

"$(INTDIR)\Log_Msg_Callback.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg_Callback.obj" $(SOURCE)

SOURCE="Log_Msg_IPC.cpp"

"$(INTDIR)\Log_Msg_IPC.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg_IPC.obj" $(SOURCE)

SOURCE="Log_Msg_NT_Event_Log.cpp"

"$(INTDIR)\Log_Msg_NT_Event_Log.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg_NT_Event_Log.obj" $(SOURCE)

SOURCE="Log_Msg_UNIX_Syslog.cpp"

"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" $(SOURCE)

SOURCE="Log_Record.cpp"

"$(INTDIR)\Log_Record.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Log_Record.obj" $(SOURCE)

SOURCE="Logging_Strategy.cpp"

"$(INTDIR)\Logging_Strategy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Logging_Strategy.obj" $(SOURCE)

SOURCE="MEM_Acceptor.cpp"

"$(INTDIR)\MEM_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_Acceptor.obj" $(SOURCE)

SOURCE="MEM_Addr.cpp"

"$(INTDIR)\MEM_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_Addr.obj" $(SOURCE)

SOURCE="MEM_Connector.cpp"

"$(INTDIR)\MEM_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_Connector.obj" $(SOURCE)

SOURCE="MEM_IO.cpp"

"$(INTDIR)\MEM_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_IO.obj" $(SOURCE)

SOURCE="MEM_SAP.cpp"

"$(INTDIR)\MEM_SAP.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_SAP.obj" $(SOURCE)

SOURCE="MEM_Stream.cpp"

"$(INTDIR)\MEM_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MEM_Stream.obj" $(SOURCE)

SOURCE="MMAP_Memory_Pool.cpp"

"$(INTDIR)\MMAP_Memory_Pool.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\MMAP_Memory_Pool.obj" $(SOURCE)

SOURCE="Malloc.cpp"

"$(INTDIR)\Malloc.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Malloc.obj" $(SOURCE)

SOURCE="Malloc_Allocator.cpp"

"$(INTDIR)\Malloc_Allocator.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Malloc_Allocator.obj" $(SOURCE)

SOURCE="Mem_Map.cpp"

"$(INTDIR)\Mem_Map.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Mem_Map.obj" $(SOURCE)

SOURCE="Message_Block.cpp"

"$(INTDIR)\Message_Block.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Message_Block.obj" $(SOURCE)

SOURCE="Message_Queue.cpp"

"$(INTDIR)\Message_Queue.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Message_Queue.obj" $(SOURCE)

SOURCE="Message_Queue_NT.cpp"

"$(INTDIR)\Message_Queue_NT.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Message_Queue_NT.obj" $(SOURCE)

SOURCE="Message_Queue_Vx.cpp"

"$(INTDIR)\Message_Queue_Vx.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Message_Queue_Vx.obj" $(SOURCE)

SOURCE="Method_Request.cpp"

"$(INTDIR)\Method_Request.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Method_Request.obj" $(SOURCE)

SOURCE="Monitor_Admin.cpp"

"$(INTDIR)\Monitor_Admin.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Admin.obj" $(SOURCE)

SOURCE="Monitor_Admin_Manager.cpp"

"$(INTDIR)\Monitor_Admin_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Admin_Manager.obj" $(SOURCE)

SOURCE="Monitor_Base.cpp"

"$(INTDIR)\Monitor_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Base.obj" $(SOURCE)

SOURCE="Monitor_Control_Action.cpp"

"$(INTDIR)\Monitor_Control_Action.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Control_Action.obj" $(SOURCE)

SOURCE="Monitor_Control_Types.cpp"

"$(INTDIR)\Monitor_Control_Types.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Control_Types.obj" $(SOURCE)

SOURCE="Monitor_Point_Registry.cpp"

"$(INTDIR)\Monitor_Point_Registry.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Point_Registry.obj" $(SOURCE)

SOURCE="Monitor_Size.cpp"

"$(INTDIR)\Monitor_Size.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Size.obj" $(SOURCE)

SOURCE="Monotonic_Time_Policy.cpp"

"$(INTDIR)\Monotonic_Time_Policy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monotonic_Time_Policy.obj" $(SOURCE)

SOURCE="Msg_WFMO_Reactor.cpp"

"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Msg_WFMO_Reactor.obj" $(SOURCE)

SOURCE="Multihomed_INET_Addr.cpp"

"$(INTDIR)\Multihomed_INET_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Multihomed_INET_Addr.obj" $(SOURCE)

SOURCE="Mutex.cpp"

"$(INTDIR)\Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Mutex.obj" $(SOURCE)

SOURCE="NT_Service.cpp"

"$(INTDIR)\NT_Service.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\NT_Service.obj" $(SOURCE)

SOURCE="Name_Proxy.cpp"

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Name_Proxy.obj" $(SOURCE)

SOURCE="Name_Request_Reply.cpp"

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Name_Request_Reply.obj" $(SOURCE)

SOURCE="Name_Space.cpp"

"$(INTDIR)\Name_Space.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Name_Space.obj" $(SOURCE)

SOURCE="Naming_Context.cpp"

"$(INTDIR)\Naming_Context.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Naming_Context.obj" $(SOURCE)

SOURCE="Netlink_Addr.cpp"

"$(INTDIR)\Netlink_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Netlink_Addr.obj" $(SOURCE)

SOURCE="Notification_Queue.cpp"

"$(INTDIR)\Notification_Queue.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Notification_Queue.obj" $(SOURCE)

SOURCE="Notification_Strategy.cpp"

"$(INTDIR)\Notification_Strategy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Notification_Strategy.obj" $(SOURCE)

SOURCE="Null_Mutex.cpp"

"$(INTDIR)\Null_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Null_Mutex.obj" $(SOURCE)

SOURCE="OS_Errno.cpp"

"$(INTDIR)\OS_Errno.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_Errno.obj" $(SOURCE)

SOURCE="OS_Log_Msg_Attributes.cpp"

"$(INTDIR)\OS_Log_Msg_Attributes.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_Log_Msg_Attributes.obj" $(SOURCE)

SOURCE="OS_NS_Thread.cpp"

"$(INTDIR)\OS_NS_Thread.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_Thread.obj" $(SOURCE)

SOURCE="OS_NS_arpa_inet.cpp"

"$(INTDIR)\OS_NS_arpa_inet.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_arpa_inet.obj" $(SOURCE)

SOURCE="OS_NS_ctype.cpp"

"$(INTDIR)\OS_NS_ctype.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_ctype.obj" $(SOURCE)

SOURCE="OS_NS_devctl.cpp"

"$(INTDIR)\OS_NS_devctl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_devctl.obj" $(SOURCE)

SOURCE="OS_NS_dirent.cpp"

"$(INTDIR)\OS_NS_dirent.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_dirent.obj" $(SOURCE)

SOURCE="OS_NS_dlfcn.cpp"

"$(INTDIR)\OS_NS_dlfcn.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_dlfcn.obj" $(SOURCE)

SOURCE="OS_NS_errno.cpp"

"$(INTDIR)\OS_NS_errno.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_errno.obj" $(SOURCE)

SOURCE="OS_NS_fcntl.cpp"

"$(INTDIR)\OS_NS_fcntl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_fcntl.obj" $(SOURCE)

SOURCE="OS_NS_math.cpp"

"$(INTDIR)\OS_NS_math.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_math.obj" $(SOURCE)

SOURCE="OS_NS_netdb.cpp"

"$(INTDIR)\OS_NS_netdb.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_netdb.obj" $(SOURCE)

SOURCE="OS_NS_poll.cpp"

"$(INTDIR)\OS_NS_poll.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_poll.obj" $(SOURCE)

SOURCE="OS_NS_pwd.cpp"

"$(INTDIR)\OS_NS_pwd.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_pwd.obj" $(SOURCE)

SOURCE="OS_NS_regex.cpp"

"$(INTDIR)\OS_NS_regex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_regex.obj" $(SOURCE)

SOURCE="OS_NS_signal.cpp"

"$(INTDIR)\OS_NS_signal.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_signal.obj" $(SOURCE)

SOURCE="OS_NS_stdio.cpp"

"$(INTDIR)\OS_NS_stdio.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_stdio.obj" $(SOURCE)

SOURCE="OS_NS_stdlib.cpp"

"$(INTDIR)\OS_NS_stdlib.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_stdlib.obj" $(SOURCE)

SOURCE="OS_NS_string.cpp"

"$(INTDIR)\OS_NS_string.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_string.obj" $(SOURCE)

SOURCE="OS_NS_strings.cpp"

"$(INTDIR)\OS_NS_strings.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_strings.obj" $(SOURCE)

SOURCE="OS_NS_stropts.cpp"

"$(INTDIR)\OS_NS_stropts.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_stropts.obj" $(SOURCE)

SOURCE="OS_NS_sys_mman.cpp"

"$(INTDIR)\OS_NS_sys_mman.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_mman.obj" $(SOURCE)

SOURCE="OS_NS_sys_msg.cpp"

"$(INTDIR)\OS_NS_sys_msg.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_msg.obj" $(SOURCE)

SOURCE="OS_NS_sys_resource.cpp"

"$(INTDIR)\OS_NS_sys_resource.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_resource.obj" $(SOURCE)

SOURCE="OS_NS_sys_select.cpp"

"$(INTDIR)\OS_NS_sys_select.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_select.obj" $(SOURCE)

SOURCE="OS_NS_sys_sendfile.cpp"

"$(INTDIR)\OS_NS_sys_sendfile.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_sendfile.obj" $(SOURCE)

SOURCE="OS_NS_sys_shm.cpp"

"$(INTDIR)\OS_NS_sys_shm.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_shm.obj" $(SOURCE)

SOURCE="OS_NS_sys_socket.cpp"

"$(INTDIR)\OS_NS_sys_socket.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_socket.obj" $(SOURCE)

SOURCE="OS_NS_sys_stat.cpp"

"$(INTDIR)\OS_NS_sys_stat.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_stat.obj" $(SOURCE)

SOURCE="OS_NS_sys_time.cpp"

"$(INTDIR)\OS_NS_sys_time.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_time.obj" $(SOURCE)

SOURCE="OS_NS_sys_uio.cpp"

"$(INTDIR)\OS_NS_sys_uio.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_uio.obj" $(SOURCE)

SOURCE="OS_NS_sys_utsname.cpp"

"$(INTDIR)\OS_NS_sys_utsname.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_utsname.obj" $(SOURCE)

SOURCE="OS_NS_sys_wait.cpp"

"$(INTDIR)\OS_NS_sys_wait.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_sys_wait.obj" $(SOURCE)

SOURCE="OS_NS_time.cpp"

"$(INTDIR)\OS_NS_time.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_time.obj" $(SOURCE)

SOURCE="OS_NS_unistd.cpp"

"$(INTDIR)\OS_NS_unistd.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_unistd.obj" $(SOURCE)

SOURCE="OS_NS_wchar.cpp"

"$(INTDIR)\OS_NS_wchar.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_wchar.obj" $(SOURCE)

SOURCE="OS_NS_wctype.cpp"

"$(INTDIR)\OS_NS_wctype.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_NS_wctype.obj" $(SOURCE)

SOURCE="OS_QoS.cpp"

"$(INTDIR)\OS_QoS.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_QoS.obj" $(SOURCE)

SOURCE="OS_TLI.cpp"

"$(INTDIR)\OS_TLI.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_TLI.obj" $(SOURCE)

SOURCE="OS_Thread_Adapter.cpp"

"$(INTDIR)\OS_Thread_Adapter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_Thread_Adapter.obj" $(SOURCE)

SOURCE="OS_main.cpp"

"$(INTDIR)\OS_main.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\OS_main.obj" $(SOURCE)

SOURCE="Obchunk.cpp"

"$(INTDIR)\Obchunk.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Obchunk.obj" $(SOURCE)

SOURCE="Object_Manager.cpp"

"$(INTDIR)\Object_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Object_Manager.obj" $(SOURCE)

SOURCE="Object_Manager_Base.cpp"

"$(INTDIR)\Object_Manager_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Object_Manager_Base.obj" $(SOURCE)

SOURCE="Obstack.cpp"

"$(INTDIR)\Obstack.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Obstack.obj" $(SOURCE)

SOURCE="PI_Malloc.cpp"

"$(INTDIR)\PI_Malloc.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\PI_Malloc.obj" $(SOURCE)

SOURCE="POSIX_Asynch_IO.cpp"

"$(INTDIR)\POSIX_Asynch_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\POSIX_Asynch_IO.obj" $(SOURCE)

SOURCE="POSIX_CB_Proactor.cpp"

"$(INTDIR)\POSIX_CB_Proactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\POSIX_CB_Proactor.obj" $(SOURCE)

SOURCE="POSIX_Proactor.cpp"

"$(INTDIR)\POSIX_Proactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\POSIX_Proactor.obj" $(SOURCE)

SOURCE="Pagefile_Memory_Pool.cpp"

"$(INTDIR)\Pagefile_Memory_Pool.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Pagefile_Memory_Pool.obj" $(SOURCE)

SOURCE="Parse_Node.cpp"

"$(INTDIR)\Parse_Node.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Parse_Node.obj" $(SOURCE)

SOURCE="Ping_Socket.cpp"

"$(INTDIR)\Ping_Socket.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Ping_Socket.obj" $(SOURCE)

SOURCE="Pipe.cpp"

"$(INTDIR)\Pipe.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Pipe.obj" $(SOURCE)

SOURCE="Priority_Reactor.cpp"

"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Priority_Reactor.obj" $(SOURCE)

SOURCE="Proactor.cpp"

"$(INTDIR)\Proactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Proactor.obj" $(SOURCE)

SOURCE="Proactor_Impl.cpp"

"$(INTDIR)\Proactor_Impl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Proactor_Impl.obj" $(SOURCE)

SOURCE="Process.cpp"

"$(INTDIR)\Process.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Process.obj" $(SOURCE)

SOURCE="Process_Manager.cpp"

"$(INTDIR)\Process_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Process_Manager.obj" $(SOURCE)

SOURCE="Process_Mutex.cpp"

"$(INTDIR)\Process_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Process_Mutex.obj" $(SOURCE)

SOURCE="Process_Semaphore.cpp"

"$(INTDIR)\Process_Semaphore.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Process_Semaphore.obj" $(SOURCE)

SOURCE="Profile_Timer.cpp"

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Profile_Timer.obj" $(SOURCE)

SOURCE="RW_Mutex.cpp"

"$(INTDIR)\RW_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\RW_Mutex.obj" $(SOURCE)

SOURCE="RW_Process_Mutex.cpp"

"$(INTDIR)\RW_Process_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\RW_Process_Mutex.obj" $(SOURCE)

SOURCE="RW_Thread_Mutex.cpp"

"$(INTDIR)\RW_Thread_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\RW_Thread_Mutex.obj" $(SOURCE)

SOURCE="Reactor.cpp"

"$(INTDIR)\Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Reactor.obj" $(SOURCE)

SOURCE="Reactor_Impl.cpp"

"$(INTDIR)\Reactor_Impl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Reactor_Impl.obj" $(SOURCE)

SOURCE="Reactor_Notification_Strategy.cpp"

"$(INTDIR)\Reactor_Notification_Strategy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Reactor_Notification_Strategy.obj" $(SOURCE)

SOURCE="Reactor_Timer_Interface.cpp"

"$(INTDIR)\Reactor_Timer_Interface.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Reactor_Timer_Interface.obj" $(SOURCE)

SOURCE="Read_Buffer.cpp"

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Read_Buffer.obj" $(SOURCE)

SOURCE="Recursive_Thread_Mutex.cpp"

"$(INTDIR)\Recursive_Thread_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Recursive_Thread_Mutex.obj" $(SOURCE)

SOURCE="Recyclable.cpp"

"$(INTDIR)\Recyclable.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Recyclable.obj" $(SOURCE)

SOURCE="Registry.cpp"

"$(INTDIR)\Registry.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Registry.obj" $(SOURCE)

SOURCE="Registry_Name_Space.cpp"

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Registry_Name_Space.obj" $(SOURCE)

SOURCE="Remote_Name_Space.cpp"

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Remote_Name_Space.obj" $(SOURCE)

SOURCE="Remote_Tokens.cpp"

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Remote_Tokens.obj" $(SOURCE)

SOURCE="Rtems_init.c"

"$(INTDIR)\Rtems_init.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Rtems_init.obj" $(SOURCE)

SOURCE="SOCK.cpp"

"$(INTDIR)\SOCK.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK.obj" $(SOURCE)

SOURCE="SOCK_Acceptor.cpp"

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Acceptor.obj" $(SOURCE)

SOURCE="SOCK_CODgram.cpp"

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_CODgram.obj" $(SOURCE)

SOURCE="SOCK_Connector.cpp"

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Connector.obj" $(SOURCE)

SOURCE="SOCK_Dgram.cpp"

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Dgram.obj" $(SOURCE)

SOURCE="SOCK_Dgram_Bcast.cpp"

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Dgram_Bcast.obj" $(SOURCE)

SOURCE="SOCK_Dgram_Mcast.cpp"

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Dgram_Mcast.obj" $(SOURCE)

SOURCE="SOCK_IO.cpp"

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_IO.obj" $(SOURCE)

SOURCE="SOCK_Netlink.cpp"

"$(INTDIR)\SOCK_Netlink.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Netlink.obj" $(SOURCE)

SOURCE="SOCK_SEQPACK_Acceptor.cpp"

"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_SEQPACK_Acceptor.obj" $(SOURCE)

SOURCE="SOCK_SEQPACK_Association.cpp"

"$(INTDIR)\SOCK_SEQPACK_Association.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_SEQPACK_Association.obj" $(SOURCE)

SOURCE="SOCK_SEQPACK_Connector.cpp"

"$(INTDIR)\SOCK_SEQPACK_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_SEQPACK_Connector.obj" $(SOURCE)

SOURCE="SOCK_Stream.cpp"

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SOCK_Stream.obj" $(SOURCE)

SOURCE="SPIPE.cpp"

"$(INTDIR)\SPIPE.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SPIPE.obj" $(SOURCE)

SOURCE="SPIPE_Acceptor.cpp"

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SPIPE_Acceptor.obj" $(SOURCE)

SOURCE="SPIPE_Addr.cpp"

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SPIPE_Addr.obj" $(SOURCE)

SOURCE="SPIPE_Connector.cpp"

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SPIPE_Connector.obj" $(SOURCE)

SOURCE="SPIPE_Stream.cpp"

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SPIPE_Stream.obj" $(SOURCE)

SOURCE="SString.cpp"

"$(INTDIR)\SString.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SString.obj" $(SOURCE)

SOURCE="SUN_Proactor.cpp"

"$(INTDIR)\SUN_Proactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SUN_Proactor.obj" $(SOURCE)

SOURCE="SV_Message.cpp"

"$(INTDIR)\SV_Message.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SV_Message.obj" $(SOURCE)

SOURCE="SV_Message_Queue.cpp"

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SV_Message_Queue.obj" $(SOURCE)

SOURCE="SV_Semaphore_Complex.cpp"

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SV_Semaphore_Complex.obj" $(SOURCE)

SOURCE="SV_Semaphore_Simple.cpp"

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SV_Semaphore_Simple.obj" $(SOURCE)

SOURCE="SV_Shared_Memory.cpp"

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SV_Shared_Memory.obj" $(SOURCE)

SOURCE="Sample_History.cpp"

"$(INTDIR)\Sample_History.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sample_History.obj" $(SOURCE)

SOURCE="Sbrk_Memory_Pool.cpp"

"$(INTDIR)\Sbrk_Memory_Pool.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sbrk_Memory_Pool.obj" $(SOURCE)

SOURCE="Sched_Params.cpp"

"$(INTDIR)\Sched_Params.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sched_Params.obj" $(SOURCE)

SOURCE="Select_Reactor_Base.cpp"

"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Select_Reactor_Base.obj" $(SOURCE)

SOURCE="Semaphore.cpp"

"$(INTDIR)\Semaphore.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Semaphore.obj" $(SOURCE)

SOURCE="Service_Config.cpp"

"$(INTDIR)\Service_Config.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Config.obj" $(SOURCE)

SOURCE="Service_Gestalt.cpp"

"$(INTDIR)\Service_Gestalt.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Gestalt.obj" $(SOURCE)

SOURCE="Service_Manager.cpp"

"$(INTDIR)\Service_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Manager.obj" $(SOURCE)

SOURCE="Service_Object.cpp"

"$(INTDIR)\Service_Object.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Object.obj" $(SOURCE)

SOURCE="Service_Repository.cpp"

"$(INTDIR)\Service_Repository.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Repository.obj" $(SOURCE)

SOURCE="Service_Types.cpp"

"$(INTDIR)\Service_Types.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Service_Types.obj" $(SOURCE)

SOURCE="Shared_Memory.cpp"

"$(INTDIR)\Shared_Memory.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Shared_Memory.obj" $(SOURCE)

SOURCE="Shared_Memory_MM.cpp"

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Shared_Memory_MM.obj" $(SOURCE)

SOURCE="Shared_Memory_Pool.cpp"

"$(INTDIR)\Shared_Memory_Pool.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Shared_Memory_Pool.obj" $(SOURCE)

SOURCE="Shared_Memory_SV.cpp"

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Shared_Memory_SV.obj" $(SOURCE)

SOURCE="Shared_Object.cpp"

"$(INTDIR)\Shared_Object.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Shared_Object.obj" $(SOURCE)

SOURCE="Sig_Adapter.cpp"

"$(INTDIR)\Sig_Adapter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sig_Adapter.obj" $(SOURCE)

SOURCE="Sig_Handler.cpp"

"$(INTDIR)\Sig_Handler.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sig_Handler.obj" $(SOURCE)

SOURCE="Signal.cpp"

"$(INTDIR)\Signal.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Signal.obj" $(SOURCE)

SOURCE="Sock_Connect.cpp"

"$(INTDIR)\Sock_Connect.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Sock_Connect.obj" $(SOURCE)

SOURCE="Stack_Trace.cpp"

"$(INTDIR)\Stack_Trace.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Stack_Trace.obj" $(SOURCE)

SOURCE="Stats.cpp"

"$(INTDIR)\Stats.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Stats.obj" $(SOURCE)

SOURCE="String_Base_Const.cpp"

"$(INTDIR)\String_Base_Const.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\String_Base_Const.obj" $(SOURCE)

SOURCE="Svc_Conf_Lexer.cpp"

"$(INTDIR)\Svc_Conf_Lexer.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Svc_Conf_Lexer.obj" $(SOURCE)

SOURCE="Svc_Conf_y.cpp"

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Svc_Conf_y.obj" $(SOURCE)

SOURCE="Synch_Options.cpp"

"$(INTDIR)\Synch_Options.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Synch_Options.obj" $(SOURCE)

SOURCE="System_Time.cpp"

"$(INTDIR)\System_Time.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\System_Time.obj" $(SOURCE)

SOURCE="TLI.cpp"

"$(INTDIR)\TLI.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TLI.obj" $(SOURCE)

SOURCE="TLI_Acceptor.cpp"

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TLI_Acceptor.obj" $(SOURCE)

SOURCE="TLI_Connector.cpp"

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TLI_Connector.obj" $(SOURCE)

SOURCE="TLI_Stream.cpp"

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TLI_Stream.obj" $(SOURCE)

SOURCE="TP_Reactor.cpp"

"$(INTDIR)\TP_Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TP_Reactor.obj" $(SOURCE)

SOURCE="TSS_Adapter.cpp"

"$(INTDIR)\TSS_Adapter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TSS_Adapter.obj" $(SOURCE)

SOURCE="TTY_IO.cpp"

"$(INTDIR)\TTY_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\TTY_IO.obj" $(SOURCE)

SOURCE="Task.cpp"

"$(INTDIR)\Task.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Task.obj" $(SOURCE)

SOURCE="Thread.cpp"

"$(INTDIR)\Thread.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread.obj" $(SOURCE)

SOURCE="Thread_Adapter.cpp"

"$(INTDIR)\Thread_Adapter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Adapter.obj" $(SOURCE)

SOURCE="Thread_Control.cpp"

"$(INTDIR)\Thread_Control.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Control.obj" $(SOURCE)

SOURCE="Thread_Exit.cpp"

"$(INTDIR)\Thread_Exit.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Exit.obj" $(SOURCE)

SOURCE="Thread_Hook.cpp"

"$(INTDIR)\Thread_Hook.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Hook.obj" $(SOURCE)

SOURCE="Thread_Manager.cpp"

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Manager.obj" $(SOURCE)

SOURCE="Thread_Mutex.cpp"

"$(INTDIR)\Thread_Mutex.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Mutex.obj" $(SOURCE)

SOURCE="Thread_Semaphore.cpp"

"$(INTDIR)\Thread_Semaphore.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Thread_Semaphore.obj" $(SOURCE)

SOURCE="Throughput_Stats.cpp"

"$(INTDIR)\Throughput_Stats.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Throughput_Stats.obj" $(SOURCE)

SOURCE="Time_Policy.cpp"

"$(INTDIR)\Time_Policy.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Time_Policy.obj" $(SOURCE)

SOURCE="Time_Value.cpp"

"$(INTDIR)\Time_Value.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Time_Value.obj" $(SOURCE)

SOURCE="Timeprobe.cpp"

"$(INTDIR)\Timeprobe.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Timeprobe.obj" $(SOURCE)

SOURCE="Token.cpp"

"$(INTDIR)\Token.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Token.obj" $(SOURCE)

SOURCE="Token_Collection.cpp"

"$(INTDIR)\Token_Collection.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Token_Collection.obj" $(SOURCE)

SOURCE="Token_Invariants.cpp"

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Token_Invariants.obj" $(SOURCE)

SOURCE="Token_Manager.cpp"

"$(INTDIR)\Token_Manager.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Token_Manager.obj" $(SOURCE)

SOURCE="Token_Request_Reply.cpp"

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Token_Request_Reply.obj" $(SOURCE)

SOURCE="Trace.cpp"

"$(INTDIR)\Trace.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Trace.obj" $(SOURCE)

SOURCE="UNIX_Addr.cpp"

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UNIX_Addr.obj" $(SOURCE)

SOURCE="UPIPE_Acceptor.cpp"

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UPIPE_Acceptor.obj" $(SOURCE)

SOURCE="UPIPE_Connector.cpp"

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UPIPE_Connector.obj" $(SOURCE)

SOURCE="UPIPE_Stream.cpp"

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UPIPE_Stream.obj" $(SOURCE)

SOURCE="UTF16_Encoding_Converter.cpp"

"$(INTDIR)\UTF16_Encoding_Converter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UTF16_Encoding_Converter.obj" $(SOURCE)

SOURCE="UTF32_Encoding_Converter.cpp"

"$(INTDIR)\UTF32_Encoding_Converter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UTF32_Encoding_Converter.obj" $(SOURCE)

SOURCE="UTF8_Encoding_Converter.cpp"

"$(INTDIR)\UTF8_Encoding_Converter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UTF8_Encoding_Converter.obj" $(SOURCE)

SOURCE="UUID.cpp"

"$(INTDIR)\UUID.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UUID.obj" $(SOURCE)

SOURCE="WFMO_Reactor.cpp"

"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\WFMO_Reactor.obj" $(SOURCE)

SOURCE="WIN32_Asynch_IO.cpp"

"$(INTDIR)\WIN32_Asynch_IO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\WIN32_Asynch_IO.obj" $(SOURCE)

SOURCE="WIN32_Proactor.cpp"

"$(INTDIR)\WIN32_Proactor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\WIN32_Proactor.obj" $(SOURCE)

SOURCE="XML_Svc_Conf.cpp"

"$(INTDIR)\XML_Svc_Conf.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XML_Svc_Conf.obj" $(SOURCE)

SOURCE="XTI_ATM_Mcast.cpp"

"$(INTDIR)\XTI_ATM_Mcast.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XTI_ATM_Mcast.obj" $(SOURCE)

SOURCE="ace_wchar.cpp"

"$(INTDIR)\ace_wchar.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ace_wchar.obj" $(SOURCE)

!IF  "$(CFG)" == "Win32 Debug"
!ELSEIF  "$(CFG)" == "Win32 Release"
!ELSEIF  "$(CFG)" == "Win32 Static Debug"
!ELSEIF  "$(CFG)" == "Win32 Static Release"
!ENDIF

SOURCE="ace.rc"

"$(INTDIR)\ace.res" : $(SOURCE)
	$(RSC) /l 0x409 /fo"$(INTDIR)\ace.res" /d NDEBUG /d _CRT_SECURE_NO_WARNINGS /d _CRT_SECURE_NO_DEPRECATE /d _CRT_NONSTDC_NO_DEPRECATE /d _WINSOCK_DEPRECATED_NO_WARNINGS /i ".." $(SOURCE)



!ENDIF

GENERATED : "$(INTDIR)" "$(OUTDIR)" $(GENERATED_DIRTY)
	-@rem

DEPENDCHECK :
!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.ACE.dep")
	@echo Using "Makefile.ACE.dep"
!ELSE
	@echo Warning: cannot find "Makefile.ACE.dep"
!ENDIF
!ENDIF

