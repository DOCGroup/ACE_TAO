# Microsoft Developer Studio Generated NMAKE File, Based on ace_lib.dsp
!IF "$(CFG)" == ""
CFG=ACE LIB - Win64 Static Debug
!MESSAGE No configuration specified. Defaulting to ACE LIB - Win64 Static Debug.
!ENDIF 

!IF "$(CFG)" != "ACE LIB - Win64 Static Release" && "$(CFG)" != "ACE LIB - Win64 Static Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_lib.mak" CFG="ACE LIB - Win64 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE LIB - Win64 Static Release" (based on "Win64 (x86) Static Library")
!MESSAGE "ACE LIB - Win64 Static Debug" (based on "Win64 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ACE LIB - Win64 Static Release"

OUTDIR=.
INTDIR=.\LIB\Release
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\aces.lib"


CLEAN :
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\ace.res"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Active_Map_Manager.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Argv_Type_Converter.obj"
	-@erase "$(INTDIR)\Asynch_Connector.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Asynch_IO_Impl.obj"
	-@erase "$(INTDIR)\Asynch_Pseudo_Task.obj"
	-@erase "$(INTDIR)\ATM_Acceptor.obj"
	-@erase "$(INTDIR)\ATM_Addr.obj"
	-@erase "$(INTDIR)\ATM_Connector.obj"
	-@erase "$(INTDIR)\ATM_Params.obj"
	-@erase "$(INTDIR)\ATM_QoS.obj"
	-@erase "$(INTDIR)\ATM_Stream.obj"
	-@erase "$(INTDIR)\Atomic_Op.obj"
	-@erase "$(INTDIR)\Base_Thread_Adapter.obj"
	-@erase "$(INTDIR)\Based_Pointer_Repository.obj"
	-@erase "$(INTDIR)\Basic_Stats.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Capabilities.obj"
	-@erase "$(INTDIR)\CDR_Base.obj"
	-@erase "$(INTDIR)\CDR_Stream.obj"
	-@erase "$(INTDIR)\Codecs.obj"
	-@erase "$(INTDIR)\Codeset_Registry.obj"
	-@erase "$(INTDIR)\Codeset_Registry_db.obj"
	-@erase "$(INTDIR)\Configuration.obj"
	-@erase "$(INTDIR)\Configuration_Import_Export.obj"
	-@erase "$(INTDIR)\Connection_Recycling_Strategy.obj"
	-@erase "$(INTDIR)\Containers.obj"
	-@erase "$(INTDIR)\Copy_Disabled.obj"
	-@erase "$(INTDIR)\Date_Time.obj"
	-@erase "$(INTDIR)\DEV.obj"
	-@erase "$(INTDIR)\DEV_Addr.obj"
	-@erase "$(INTDIR)\DEV_Connector.obj"
	-@erase "$(INTDIR)\DEV_IO.obj"
	-@erase "$(INTDIR)\Dirent.obj"
	-@erase "$(INTDIR)\Dirent_Selector.obj"
	-@erase "$(INTDIR)\DLL.obj"
	-@erase "$(INTDIR)\DLL_Manager.obj"
	-@erase "$(INTDIR)\Dump.obj"
	-@erase "$(INTDIR)\Dynamic.obj"
	-@erase "$(INTDIR)\Dynamic_Service.obj"
	-@erase "$(INTDIR)\Dynamic_Service_Base.obj"
	-@erase "$(INTDIR)\Event_Handler.obj"
	-@erase "$(INTDIR)\Event_Handler_T.obj"
	-@erase "$(INTDIR)\FIFO.obj"
	-@erase "$(INTDIR)\FIFO_Recv.obj"
	-@erase "$(INTDIR)\FIFO_Recv_Msg.obj"
	-@erase "$(INTDIR)\FIFO_Send.obj"
	-@erase "$(INTDIR)\FIFO_Send_Msg.obj"
	-@erase "$(INTDIR)\FILE.obj"
	-@erase "$(INTDIR)\FILE_Addr.obj"
	-@erase "$(INTDIR)\FILE_Connector.obj"
	-@erase "$(INTDIR)\FILE_IO.obj"
	-@erase "$(INTDIR)\File_Lock.obj"
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Flag_Manip.obj"
	-@erase "$(INTDIR)\Framework_Component.obj"
	-@erase "$(INTDIR)\Framework_Component_T.obj"
	-@erase "$(INTDIR)\Functor.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Ops.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\Hash_Map_Manager.obj"
	-@erase "$(INTDIR)\Hashable.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\Init_ACE.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
	-@erase "$(INTDIR)\IPC_SAP.obj"
	-@erase "$(INTDIR)\Lib_Find.obj"
	-@erase "$(INTDIR)\Local_Name_Space.obj"
	-@erase "$(INTDIR)\Local_Tokens.obj"
	-@erase "$(INTDIR)\Log_Msg.obj"
	-@erase "$(INTDIR)\Log_Msg_Backend.obj"
	-@erase "$(INTDIR)\Log_Msg_Callback.obj"
	-@erase "$(INTDIR)\Log_Msg_IPC.obj"
	-@erase "$(INTDIR)\Log_Msg_NT_Event_Log.obj"
	-@erase "$(INTDIR)\Log_Msg_UNIX_Syslog.obj"
	-@erase "$(INTDIR)\Log_Record.obj"
	-@erase "$(INTDIR)\Logging_Strategy.obj"
	-@erase "$(INTDIR)\LSOCK.obj"
	-@erase "$(INTDIR)\LSOCK_Acceptor.obj"
	-@erase "$(INTDIR)\LSOCK_CODgram.obj"
	-@erase "$(INTDIR)\LSOCK_Connector.obj"
	-@erase "$(INTDIR)\LSOCK_Dgram.obj"
	-@erase "$(INTDIR)\LSOCK_Stream.obj"
	-@erase "$(INTDIR)\Malloc.obj"
	-@erase "$(INTDIR)\Malloc_Allocator.obj"
	-@erase "$(INTDIR)\Malloc_Instantiations.obj"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\MEM_Acceptor.obj"
	-@erase "$(INTDIR)\MEM_Addr.obj"
	-@erase "$(INTDIR)\MEM_Connector.obj"
	-@erase "$(INTDIR)\MEM_IO.obj"
	-@erase "$(INTDIR)\Mem_Map.obj"
	-@erase "$(INTDIR)\MEM_SAP.obj"
	-@erase "$(INTDIR)\MEM_Stream.obj"
	-@erase "$(INTDIR)\Memory_Pool.obj"
	-@erase "$(INTDIR)\Message_Block.obj"
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\Notification_Strategy.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Obchunk.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\OS_Dirent.obj"
	-@erase "$(INTDIR)\OS_Errno.obj"
	-@erase "$(INTDIR)\OS_Log_Msg_Attributes.obj"
	-@erase "$(INTDIR)\OS_Memory.obj"
	-@erase "$(INTDIR)\OS_QoS.obj"
	-@erase "$(INTDIR)\OS_String.obj"
	-@erase "$(INTDIR)\OS_Thread_Adapter.obj"
	-@erase "$(INTDIR)\OS_TLI.obj"
	-@erase "$(INTDIR)\Pair.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\PI_Malloc.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\POSIX_Asynch_IO.obj"
	-@erase "$(INTDIR)\POSIX_Proactor.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Process_Mutex.obj"
	-@erase "$(INTDIR)\Process_Semaphore.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Reactor_Notification_Strategy.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Recyclable.obj"
	-@erase "$(INTDIR)\Refcountable.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\RW_Process_Mutex.obj"
	-@erase "$(INTDIR)\Sample_History.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Templates.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
	-@erase "$(INTDIR)\Shared_Memory_MM.obj"
	-@erase "$(INTDIR)\Shared_Memory_SV.obj"
	-@erase "$(INTDIR)\Shared_Object.obj"
	-@erase "$(INTDIR)\Signal.obj"
	-@erase "$(INTDIR)\SOCK.obj"
	-@erase "$(INTDIR)\SOCK_Acceptor.obj"
	-@erase "$(INTDIR)\SOCK_CODgram.obj"
	-@erase "$(INTDIR)\Sock_Connect.obj"
	-@erase "$(INTDIR)\SOCK_Connector.obj"
	-@erase "$(INTDIR)\SOCK_Dgram.obj"
	-@erase "$(INTDIR)\SOCK_Dgram_Bcast.obj"
	-@erase "$(INTDIR)\SOCK_Dgram_Mcast.obj"
	-@erase "$(INTDIR)\SOCK_IO.obj"
	-@erase "$(INTDIR)\SOCK_Stream.obj"
	-@erase "$(INTDIR)\SPIPE.obj"
	-@erase "$(INTDIR)\SPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\SPIPE_Addr.obj"
	-@erase "$(INTDIR)\SPIPE_Connector.obj"
	-@erase "$(INTDIR)\SPIPE_Stream.obj"
	-@erase "$(INTDIR)\SString.obj"
	-@erase "$(INTDIR)\Stats.obj"
	-@erase "$(INTDIR)\String_Base_Const.obj"
	-@erase "$(INTDIR)\SV_Message.obj"
	-@erase "$(INTDIR)\SV_Message_Queue.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Complex.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Simple.obj"
	-@erase "$(INTDIR)\SV_Shared_Memory.obj"
	-@erase "$(INTDIR)\Svc_Conf_l.obj"
	-@erase "$(INTDIR)\Svc_Conf_Lexer_Guard.obj"
	-@erase "$(INTDIR)\Svc_Conf_y.obj"
	-@erase "$(INTDIR)\Svc_Handler.obj"
	-@erase "$(INTDIR)\Synch.obj"
	-@erase "$(INTDIR)\Synch_Options.obj"
	-@erase "$(INTDIR)\System_Time.obj"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\Template_Instantiations.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread_Adapter.obj"
	-@erase "$(INTDIR)\Thread_Control.obj"
	-@erase "$(INTDIR)\Thread_Exit.obj"
	-@erase "$(INTDIR)\Thread_Hook.obj"
	-@erase "$(INTDIR)\Thread_Manager.obj"
	-@erase "$(INTDIR)\Time_Request_Reply.obj"
	-@erase "$(INTDIR)\Time_Value.obj"
	-@erase "$(INTDIR)\Timeprobe.obj"
	-@erase "$(INTDIR)\Timer_Hash.obj"
	-@erase "$(INTDIR)\Timer_Heap.obj"
	-@erase "$(INTDIR)\Timer_List.obj"
	-@erase "$(INTDIR)\Timer_Queue.obj"
	-@erase "$(INTDIR)\Timer_Queue_Adapters.obj"
	-@erase "$(INTDIR)\Timer_Wheel.obj"
	-@erase "$(INTDIR)\TLI.obj"
	-@erase "$(INTDIR)\TLI_Acceptor.obj"
	-@erase "$(INTDIR)\TLI_Connector.obj"
	-@erase "$(INTDIR)\TLI_Stream.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Token_Collection.obj"
	-@erase "$(INTDIR)\Token_Invariants.obj"
	-@erase "$(INTDIR)\Token_Manager.obj"
	-@erase "$(INTDIR)\Token_Request_Reply.obj"
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\Typed_SV_Message.obj"
	-@erase "$(INTDIR)\Typed_SV_Message_Queue.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(INTDIR)\WIN32_Asynch_IO.obj"
	-@erase "$(INTDIR)\WIN32_Proactor.obj"
	-@erase "$(INTDIR)\XML_Svc_Conf.obj"
	-@erase "$(INTDIR)\XtReactor.obj"
	-@erase "$(OUTDIR)\aces.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

LINK32=link.exe -lib
MTL=midl.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Wp64 /GX /Zi /O1 /I "../" /D "_WINDOWS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ace.res" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ace.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\aces.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Asynch_Connector.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Codecs.obj" \
	"$(INTDIR)\Codeset_Registry.obj"
	"$(INTDIR)\Codeset_Registry_db.obj"
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Service.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_T.obj" \
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
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hash_Map_Manager.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Malloc_Instantiations.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\Memory_Pool.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\OS_Dirent.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_Memory.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_String.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\Pair.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Refcountable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Templates.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Svc_Conf_l.obj" \
	"$(INTDIR)\Svc_Conf_Lexer_Guard.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Svc_Handler.obj" \
	"$(INTDIR)\Synch.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Template_Instantiations.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Time_Request_Reply.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Timer_Hash.obj" \
	"$(INTDIR)\Timer_Heap.obj" \
	"$(INTDIR)\Timer_List.obj" \
	"$(INTDIR)\Timer_Queue.obj" \
	"$(INTDIR)\Timer_Queue_Adapters.obj" \
	"$(INTDIR)\Timer_Wheel.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Typed_SV_Message.obj" \
	"$(INTDIR)\Typed_SV_Message_Queue.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XtReactor.obj" \
	"$(INTDIR)\Atomic_Op.obj" \
	"$(INTDIR)\Framework_Component_T.obj" \
	"$(INTDIR)\ace.res"

"$(OUTDIR)\aces.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE LIB - Win64 Static Debug"

OUTDIR=.
INTDIR=.\LIB\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\acesd.lib"


CLEAN :
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\ace.res"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Active_Map_Manager.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Argv_Type_Converter.obj"
	-@erase "$(INTDIR)\Asynch_Connector.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Asynch_IO_Impl.obj"
	-@erase "$(INTDIR)\Asynch_Pseudo_Task.obj"
	-@erase "$(INTDIR)\ATM_Acceptor.obj"
	-@erase "$(INTDIR)\ATM_Addr.obj"
	-@erase "$(INTDIR)\ATM_Connector.obj"
	-@erase "$(INTDIR)\ATM_Params.obj"
	-@erase "$(INTDIR)\ATM_QoS.obj"
	-@erase "$(INTDIR)\ATM_Stream.obj"
	-@erase "$(INTDIR)\Base_Thread_Adapter.obj"
	-@erase "$(INTDIR)\Based_Pointer_Repository.obj"
	-@erase "$(INTDIR)\Basic_Stats.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Capabilities.obj"
	-@erase "$(INTDIR)\CDR_Base.obj"
	-@erase "$(INTDIR)\CDR_Stream.obj"
	-@erase "$(INTDIR)\Codecs.obj"
	-@erase "$(INTDIR)\Codeset_Registry.obj"
	-@erase "$(INTDIR)\Codeset_Registry_db.obj"
	-@erase "$(INTDIR)\Configuration.obj"
	-@erase "$(INTDIR)\Configuration_Import_Export.obj"
	-@erase "$(INTDIR)\Connection_Recycling_Strategy.obj"
	-@erase "$(INTDIR)\Containers.obj"
	-@erase "$(INTDIR)\Copy_Disabled.obj"
	-@erase "$(INTDIR)\Date_Time.obj"
	-@erase "$(INTDIR)\DEV.obj"
	-@erase "$(INTDIR)\DEV_Addr.obj"
	-@erase "$(INTDIR)\DEV_Connector.obj"
	-@erase "$(INTDIR)\DEV_IO.obj"
	-@erase "$(INTDIR)\Dirent.obj"
	-@erase "$(INTDIR)\Dirent_Selector.obj"
	-@erase "$(INTDIR)\DLL.obj"
	-@erase "$(INTDIR)\DLL_Manager.obj"
	-@erase "$(INTDIR)\Dump.obj"
	-@erase "$(INTDIR)\Dynamic.obj"
	-@erase "$(INTDIR)\Dynamic_Service.obj"
	-@erase "$(INTDIR)\Dynamic_Service_Base.obj"
	-@erase "$(INTDIR)\Event_Handler.obj"
	-@erase "$(INTDIR)\Event_Handler_T.obj"
	-@erase "$(INTDIR)\FIFO.obj"
	-@erase "$(INTDIR)\FIFO_Recv.obj"
	-@erase "$(INTDIR)\FIFO_Recv_Msg.obj"
	-@erase "$(INTDIR)\FIFO_Send.obj"
	-@erase "$(INTDIR)\FIFO_Send_Msg.obj"
	-@erase "$(INTDIR)\FILE.obj"
	-@erase "$(INTDIR)\FILE_Addr.obj"
	-@erase "$(INTDIR)\FILE_Connector.obj"
	-@erase "$(INTDIR)\FILE_IO.obj"
	-@erase "$(INTDIR)\File_Lock.obj"
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Flag_Manip.obj"
	-@erase "$(INTDIR)\Framework_Component.obj"
	-@erase "$(INTDIR)\Functor.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Ops.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\Hash_Map_Manager.obj"
	-@erase "$(INTDIR)\Hashable.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\Init_ACE.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
	-@erase "$(INTDIR)\IPC_SAP.obj"
	-@erase "$(INTDIR)\Lib_Find.obj"
	-@erase "$(INTDIR)\Local_Name_Space.obj"
	-@erase "$(INTDIR)\Local_Tokens.obj"
	-@erase "$(INTDIR)\Log_Msg.obj"
	-@erase "$(INTDIR)\Log_Msg_Backend.obj"
	-@erase "$(INTDIR)\Log_Msg_Callback.obj"
	-@erase "$(INTDIR)\Log_Msg_IPC.obj"
	-@erase "$(INTDIR)\Log_Msg_NT_Event_Log.obj"
	-@erase "$(INTDIR)\Log_Msg_UNIX_Syslog.obj"
	-@erase "$(INTDIR)\Log_Record.obj"
	-@erase "$(INTDIR)\Logging_Strategy.obj"
	-@erase "$(INTDIR)\LSOCK.obj"
	-@erase "$(INTDIR)\LSOCK_Acceptor.obj"
	-@erase "$(INTDIR)\LSOCK_CODgram.obj"
	-@erase "$(INTDIR)\LSOCK_Connector.obj"
	-@erase "$(INTDIR)\LSOCK_Dgram.obj"
	-@erase "$(INTDIR)\LSOCK_Stream.obj"
	-@erase "$(INTDIR)\Malloc.obj"
	-@erase "$(INTDIR)\Malloc_Allocator.obj"
	-@erase "$(INTDIR)\Malloc_Instantiations.obj"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\MEM_Acceptor.obj"
	-@erase "$(INTDIR)\MEM_Addr.obj"
	-@erase "$(INTDIR)\MEM_Connector.obj"
	-@erase "$(INTDIR)\MEM_IO.obj"
	-@erase "$(INTDIR)\Mem_Map.obj"
	-@erase "$(INTDIR)\MEM_SAP.obj"
	-@erase "$(INTDIR)\MEM_Stream.obj"
	-@erase "$(INTDIR)\Memory_Pool.obj"
	-@erase "$(INTDIR)\Message_Block.obj"
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\Notification_Strategy.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Obchunk.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\OS_Dirent.obj"
	-@erase "$(INTDIR)\OS_Errno.obj"
	-@erase "$(INTDIR)\OS_Log_Msg_Attributes.obj"
	-@erase "$(INTDIR)\OS_Memory.obj"
	-@erase "$(INTDIR)\OS_QoS.obj"
	-@erase "$(INTDIR)\OS_String.obj"
	-@erase "$(INTDIR)\OS_Thread_Adapter.obj"
	-@erase "$(INTDIR)\OS_TLI.obj"
	-@erase "$(INTDIR)\Pair.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\PI_Malloc.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\POSIX_Asynch_IO.obj"
	-@erase "$(INTDIR)\POSIX_Proactor.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Process_Mutex.obj"
	-@erase "$(INTDIR)\Process_Semaphore.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Reactor_Notification_Strategy.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Recyclable.obj"
	-@erase "$(INTDIR)\Refcountable.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\RW_Process_Mutex.obj"
	-@erase "$(INTDIR)\Sample_History.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Templates.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
	-@erase "$(INTDIR)\Shared_Memory_MM.obj"
	-@erase "$(INTDIR)\Shared_Memory_SV.obj"
	-@erase "$(INTDIR)\Shared_Object.obj"
	-@erase "$(INTDIR)\Signal.obj"
	-@erase "$(INTDIR)\SOCK.obj"
	-@erase "$(INTDIR)\SOCK_Acceptor.obj"
	-@erase "$(INTDIR)\SOCK_CODgram.obj"
	-@erase "$(INTDIR)\Sock_Connect.obj"
	-@erase "$(INTDIR)\SOCK_Connector.obj"
	-@erase "$(INTDIR)\SOCK_Dgram.obj"
	-@erase "$(INTDIR)\SOCK_Dgram_Bcast.obj"
	-@erase "$(INTDIR)\SOCK_Dgram_Mcast.obj"
	-@erase "$(INTDIR)\SOCK_IO.obj"
	-@erase "$(INTDIR)\SOCK_Stream.obj"
	-@erase "$(INTDIR)\SPIPE.obj"
	-@erase "$(INTDIR)\SPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\SPIPE_Addr.obj"
	-@erase "$(INTDIR)\SPIPE_Connector.obj"
	-@erase "$(INTDIR)\SPIPE_Stream.obj"
	-@erase "$(INTDIR)\SString.obj"
	-@erase "$(INTDIR)\Stats.obj"
	-@erase "$(INTDIR)\String_Base_Const.obj"
	-@erase "$(INTDIR)\SV_Message.obj"
	-@erase "$(INTDIR)\SV_Message_Queue.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Complex.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Simple.obj"
	-@erase "$(INTDIR)\SV_Shared_Memory.obj"
	-@erase "$(INTDIR)\Svc_Conf_l.obj"
	-@erase "$(INTDIR)\Svc_Conf_Lexer_Guard.obj"
	-@erase "$(INTDIR)\Svc_Conf_y.obj"
	-@erase "$(INTDIR)\Svc_Handler.obj"
	-@erase "$(INTDIR)\Synch.obj"
	-@erase "$(INTDIR)\Synch_Options.obj"
	-@erase "$(INTDIR)\System_Time.obj"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\Template_Instantiations.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread_Adapter.obj"
	-@erase "$(INTDIR)\Thread_Control.obj"
	-@erase "$(INTDIR)\Thread_Exit.obj"
	-@erase "$(INTDIR)\Thread_Hook.obj"
	-@erase "$(INTDIR)\Thread_Manager.obj"
	-@erase "$(INTDIR)\Time_Request_Reply.obj"
	-@erase "$(INTDIR)\Time_Value.obj"
	-@erase "$(INTDIR)\Timeprobe.obj"
	-@erase "$(INTDIR)\Timer_Hash.obj"
	-@erase "$(INTDIR)\Timer_Heap.obj"
	-@erase "$(INTDIR)\Timer_List.obj"
	-@erase "$(INTDIR)\Timer_Queue.obj"
	-@erase "$(INTDIR)\Timer_Queue_Adapters.obj"
	-@erase "$(INTDIR)\Timer_Wheel.obj"
	-@erase "$(INTDIR)\TLI.obj"
	-@erase "$(INTDIR)\TLI_Acceptor.obj"
	-@erase "$(INTDIR)\TLI_Connector.obj"
	-@erase "$(INTDIR)\TLI_Stream.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Token_Collection.obj"
	-@erase "$(INTDIR)\Token_Invariants.obj"
	-@erase "$(INTDIR)\Token_Manager.obj"
	-@erase "$(INTDIR)\Token_Request_Reply.obj"
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\Typed_SV_Message.obj"
	-@erase "$(INTDIR)\Typed_SV_Message_Queue.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(INTDIR)\WIN32_Asynch_IO.obj"
	-@erase "$(INTDIR)\WIN32_Proactor.obj"
	-@erase "$(INTDIR)\XML_Svc_Conf.obj"
	-@erase "$(INTDIR)\XtReactor.obj"
	-@erase "$(OUTDIR)\acesd.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

LINK32=link.exe -lib
MTL=midl.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Wp64 /Gm /GX /Zi /Od /Gy /I "../" /D "_WINDOWS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ace.res" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ace.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\acesd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Active_Map_Manager.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Argv_Type_Converter.obj" \
	"$(INTDIR)\Asynch_Connector.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Asynch_IO_Impl.obj" \
	"$(INTDIR)\Asynch_Pseudo_Task.obj" \
	"$(INTDIR)\ATM_Acceptor.obj" \
	"$(INTDIR)\ATM_Addr.obj" \
	"$(INTDIR)\ATM_Connector.obj" \
	"$(INTDIR)\ATM_Params.obj" \
	"$(INTDIR)\ATM_QoS.obj" \
	"$(INTDIR)\ATM_Stream.obj" \
	"$(INTDIR)\Base_Thread_Adapter.obj" \
	"$(INTDIR)\Based_Pointer_Repository.obj" \
	"$(INTDIR)\Basic_Stats.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Capabilities.obj" \
	"$(INTDIR)\CDR_Base.obj" \
	"$(INTDIR)\CDR_Stream.obj" \
	"$(INTDIR)\Codecs.obj" \
        "$(INTDIR)\Codeset_Registry.obj"
	"$(INTDIR)\Codeset_Registry_db.obj"
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\Configuration_Import_Export.obj" \
	"$(INTDIR)\Connection_Recycling_Strategy.obj" \
	"$(INTDIR)\Containers.obj" \
	"$(INTDIR)\Copy_Disabled.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\Dirent.obj" \
	"$(INTDIR)\Dirent_Selector.obj" \
	"$(INTDIR)\DLL.obj" \
	"$(INTDIR)\DLL_Manager.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Dynamic_Service.obj" \
	"$(INTDIR)\Dynamic_Service_Base.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\Event_Handler_T.obj" \
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
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Ops.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\Hash_Map_Manager.obj" \
	"$(INTDIR)\Hashable.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\Init_ACE.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Lib_Find.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Msg_Backend.obj" \
	"$(INTDIR)\Log_Msg_Callback.obj" \
	"$(INTDIR)\Log_Msg_IPC.obj" \
	"$(INTDIR)\Log_Msg_NT_Event_Log.obj" \
	"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Malloc_Allocator.obj" \
	"$(INTDIR)\Malloc_Instantiations.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\MEM_Acceptor.obj" \
	"$(INTDIR)\MEM_Addr.obj" \
	"$(INTDIR)\MEM_Connector.obj" \
	"$(INTDIR)\MEM_IO.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\MEM_SAP.obj" \
	"$(INTDIR)\MEM_Stream.obj" \
	"$(INTDIR)\Memory_Pool.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Notification_Strategy.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Obchunk.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\OS_Dirent.obj" \
	"$(INTDIR)\OS_Errno.obj" \
	"$(INTDIR)\OS_Log_Msg_Attributes.obj" \
	"$(INTDIR)\OS_Memory.obj" \
	"$(INTDIR)\OS_QoS.obj" \
	"$(INTDIR)\OS_String.obj" \
	"$(INTDIR)\OS_Thread_Adapter.obj" \
	"$(INTDIR)\OS_TLI.obj" \
	"$(INTDIR)\Pair.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\PI_Malloc.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\POSIX_Asynch_IO.obj" \
	"$(INTDIR)\POSIX_Proactor.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Process_Mutex.obj" \
	"$(INTDIR)\Process_Semaphore.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Reactor_Notification_Strategy.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Recyclable.obj" \
	"$(INTDIR)\Refcountable.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\RW_Process_Mutex.obj" \
	"$(INTDIR)\Sample_History.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Templates.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
	"$(INTDIR)\Sock_Connect.obj" \
	"$(INTDIR)\SOCK_Connector.obj" \
	"$(INTDIR)\SOCK_Dgram.obj" \
	"$(INTDIR)\SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)\SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)\SOCK_IO.obj" \
	"$(INTDIR)\SOCK_Stream.obj" \
	"$(INTDIR)\SPIPE.obj" \
	"$(INTDIR)\SPIPE_Acceptor.obj" \
	"$(INTDIR)\SPIPE_Addr.obj" \
	"$(INTDIR)\SPIPE_Connector.obj" \
	"$(INTDIR)\SPIPE_Stream.obj" \
	"$(INTDIR)\SString.obj" \
	"$(INTDIR)\Stats.obj" \
	"$(INTDIR)\String_Base_Const.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Svc_Conf_l.obj" \
	"$(INTDIR)\Svc_Conf_Lexer_Guard.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Svc_Handler.obj" \
	"$(INTDIR)\Synch.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Template_Instantiations.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Adapter.obj" \
	"$(INTDIR)\Thread_Control.obj" \
	"$(INTDIR)\Thread_Exit.obj" \
	"$(INTDIR)\Thread_Hook.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Time_Request_Reply.obj" \
	"$(INTDIR)\Time_Value.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Timer_Hash.obj" \
	"$(INTDIR)\Timer_Heap.obj" \
	"$(INTDIR)\Timer_List.obj" \
	"$(INTDIR)\Timer_Queue.obj" \
	"$(INTDIR)\Timer_Queue_Adapters.obj" \
	"$(INTDIR)\Timer_Wheel.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\Typed_SV_Message.obj" \
	"$(INTDIR)\Typed_SV_Message_Queue.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj" \
	"$(INTDIR)\WIN32_Asynch_IO.obj" \
	"$(INTDIR)\WIN32_Proactor.obj" \
	"$(INTDIR)\XML_Svc_Conf.obj" \
	"$(INTDIR)\XtReactor.obj" \
	"$(INTDIR)\ace.res"

"$(OUTDIR)\acesd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ace_lib.dep")
!INCLUDE "ace_lib.dep"
!ELSE 
!MESSAGE Warning: cannot find "ace_lib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ACE LIB - Win64 Static Release" || "$(CFG)" == "ACE LIB - Win64 Static Debug"
SOURCE=.\ACE.cpp

"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Activation_Queue.cpp

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Active_Map_Manager.cpp

"$(INTDIR)\Active_Map_Manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Addr.cpp

"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Arg_Shifter.cpp

"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ARGV.cpp

"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Argv_Type_Converter.cpp

"$(INTDIR)\Argv_Type_Converter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Asynch_Connector.cpp

"$(INTDIR)\Asynch_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Asynch_IO.cpp

"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Asynch_IO_Impl.cpp

"$(INTDIR)\Asynch_IO_Impl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Asynch_Pseudo_Task.cpp

"$(INTDIR)\Asynch_Pseudo_Task.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\ATM_Acceptor.cpp

"$(INTDIR)\ATM_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ATM_Addr.cpp

"$(INTDIR)\ATM_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ATM_Connector.cpp

"$(INTDIR)\ATM_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ATM_Params.cpp

"$(INTDIR)\ATM_Params.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ATM_QoS.cpp

"$(INTDIR)\ATM_QoS.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ATM_Stream.cpp

"$(INTDIR)\ATM_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Base_Thread_Adapter.cpp

"$(INTDIR)\Base_Thread_Adapter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Based_Pointer_Repository.cpp

"$(INTDIR)\Based_Pointer_Repository.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Basic_Stats.cpp

"$(INTDIR)\Basic_Stats.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Basic_Types.cpp

"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)" ".\Template_Instantiations.cpp" ".\Framework_Component_T.cpp"


SOURCE=.\Capabilities.cpp

"$(INTDIR)\Capabilities.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CDR_Base.cpp

"$(INTDIR)\CDR_Base.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CDR_Stream.cpp

"$(INTDIR)\CDR_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Codecs.cpp

"$(INTDIR)\Codecs.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Codeset_Registry.cpp

"$(INTDIR)\Codeset_Registry.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Codeset_Registry_db.cpp

"$(INTDIR)\Codeset_Registry_db.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Configuration.cpp

"$(INTDIR)\Configuration.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Configuration_Import_Export.cpp

"$(INTDIR)\Configuration_Import_Export.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Connection_Recycling_Strategy.cpp

"$(INTDIR)\Connection_Recycling_Strategy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Containers.cpp

"$(INTDIR)\Containers.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Copy_Disabled.cpp

"$(INTDIR)\Copy_Disabled.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Date_Time.cpp

"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DEV.cpp

"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DEV_Addr.cpp

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DEV_Connector.cpp

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DEV_IO.cpp

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Dirent.cpp

"$(INTDIR)\Dirent.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Dirent_Selector.cpp

"$(INTDIR)\Dirent_Selector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DLL.cpp

"$(INTDIR)\DLL.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DLL_Manager.cpp

"$(INTDIR)\DLL_Manager.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Dump.cpp

"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Dynamic.cpp

"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Dynamic_Service.cpp

"$(INTDIR)\Dynamic_Service.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Dynamic_Service_Base.cpp

"$(INTDIR)\Dynamic_Service_Base.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Event_Handler.cpp

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Event_Handler_T.cpp

"$(INTDIR)\Event_Handler_T.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FIFO.cpp

"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FIFO_Recv.cpp

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FIFO_Recv_Msg.cpp

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FIFO_Send.cpp

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FIFO_Send_Msg.cpp

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FILE.cpp

"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FILE_Addr.cpp

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FILE_Connector.cpp

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FILE_IO.cpp

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\File_Lock.cpp

"$(INTDIR)\File_Lock.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Filecache.cpp

"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Flag_Manip.cpp

"$(INTDIR)\Flag_Manip.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Framework_Component.cpp

"$(INTDIR)\Framework_Component.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Functor.cpp

"$(INTDIR)\Functor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Get_Opt.cpp

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Handle_Ops.cpp

"$(INTDIR)\Handle_Ops.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Handle_Set.cpp

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Hash_Map_Manager.cpp

"$(INTDIR)\Hash_Map_Manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Hashable.cpp

"$(INTDIR)\Hashable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\High_Res_Timer.cpp

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\INET_Addr.cpp

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Init_ACE.cpp

"$(INTDIR)\Init_ACE.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IO_Cntl_Msg.cpp

"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IO_SAP.cpp

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IOStream.cpp

"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IPC_SAP.cpp

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Lib_Find.cpp

"$(INTDIR)\Lib_Find.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Local_Name_Space.cpp

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Local_Tokens.cpp

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Msg.cpp

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Log_Msg_Backend.cpp

"$(INTDIR)\Log_Msg_Backend.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Msg_Callback.cpp

"$(INTDIR)\Log_Msg_Callback.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Msg_IPC.cpp

"$(INTDIR)\Log_Msg_IPC.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Msg_NT_Event_Log.cpp

"$(INTDIR)\Log_Msg_NT_Event_Log.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Msg_UNIX_Syslog.cpp

"$(INTDIR)\Log_Msg_UNIX_Syslog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Log_Record.cpp

"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Logging_Strategy.cpp

"$(INTDIR)\Logging_Strategy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK.cpp

"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK_Acceptor.cpp

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK_CODgram.cpp

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK_Connector.cpp

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK_Dgram.cpp

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSOCK_Stream.cpp

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Malloc.cpp

"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Malloc_Allocator.cpp

"$(INTDIR)\Malloc_Allocator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Malloc_Instantiations.cpp

"$(INTDIR)\Malloc_Instantiations.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Map.cpp

"$(INTDIR)\Map.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MEM_Acceptor.cpp

"$(INTDIR)\MEM_Acceptor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\MEM_Addr.cpp

"$(INTDIR)\MEM_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MEM_Connector.cpp

"$(INTDIR)\MEM_Connector.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\MEM_IO.cpp

"$(INTDIR)\MEM_IO.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Mem_Map.cpp

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MEM_SAP.cpp

"$(INTDIR)\MEM_SAP.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\MEM_Stream.cpp

"$(INTDIR)\MEM_Stream.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Memory_Pool.cpp

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Message_Block.cpp

"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Message_Queue.cpp

"$(INTDIR)\Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Method_Request.cpp

"$(INTDIR)\Method_Request.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Msg_WFMO_Reactor.cpp

"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Multiplexor.cpp

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Name_Proxy.cpp

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Name_Request_Reply.cpp

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Name_Space.cpp

"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Naming_Context.cpp

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Notification_Strategy.cpp

"$(INTDIR)\Notification_Strategy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NT_Service.cpp

"$(INTDIR)\NT_Service.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Obchunk.cpp

"$(INTDIR)\Obchunk.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Object_Manager.cpp

"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Obstack.cpp

"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS.cpp

"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_Dirent.cpp

"$(INTDIR)\OS_Dirent.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_Errno.cpp

"$(INTDIR)\OS_Errno.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_Log_Msg_Attributes.cpp

"$(INTDIR)\OS_Log_Msg_Attributes.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_Memory.cpp

"$(INTDIR)\OS_Memory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_QoS.cpp

"$(INTDIR)\OS_QoS.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_String.cpp

"$(INTDIR)\OS_String.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_Thread_Adapter.cpp

"$(INTDIR)\OS_Thread_Adapter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OS_TLI.cpp

"$(INTDIR)\OS_TLI.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Pair.cpp

"$(INTDIR)\Pair.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Parse_Node.cpp

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\PI_Malloc.cpp

"$(INTDIR)\PI_Malloc.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Pipe.cpp

"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\POSIX_Asynch_IO.cpp

"$(INTDIR)\POSIX_Asynch_IO.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\POSIX_Proactor.cpp

"$(INTDIR)\POSIX_Proactor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Priority_Reactor.cpp

"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Proactor.cpp

"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Process.cpp

"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Process_Manager.cpp

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Process_Mutex.cpp

"$(INTDIR)\Process_Mutex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Process_Semaphore.cpp

"$(INTDIR)\Process_Semaphore.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Profile_Timer.cpp

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Reactor.cpp

"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Reactor_Notification_Strategy.cpp

"$(INTDIR)\Reactor_Notification_Strategy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Read_Buffer.cpp

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Recyclable.cpp

"$(INTDIR)\Recyclable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Refcountable.cpp

"$(INTDIR)\Refcountable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Registry.cpp

"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Registry_Name_Space.cpp

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Remote_Name_Space.cpp

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Remote_Tokens.cpp

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\RW_Process_Mutex.cpp

"$(INTDIR)\RW_Process_Mutex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Sample_History.cpp

"$(INTDIR)\Sample_History.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Sched_Params.cpp

"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Select_Reactor.cpp

"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Select_Reactor_Base.cpp

"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Service_Config.cpp

"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Service_Manager.cpp

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Service_Object.cpp

"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Service_Repository.cpp

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Service_Templates.cpp

"$(INTDIR)\Service_Templates.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Service_Types.cpp

"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Shared_Memory.cpp

"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Shared_Memory_MM.cpp

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Shared_Memory_SV.cpp

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Shared_Object.cpp

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Signal.cpp

"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK.cpp

"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Acceptor.cpp

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_CODgram.cpp

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Sock_Connect.cpp

"$(INTDIR)\Sock_Connect.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Connector.cpp

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Dgram.cpp

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Dgram_Bcast.cpp

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Dgram_Mcast.cpp

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_IO.cpp

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SOCK_Stream.cpp

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SPIPE.cpp

"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SPIPE_Acceptor.cpp

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SPIPE_Addr.cpp

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SPIPE_Connector.cpp

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SPIPE_Stream.cpp

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SString.cpp

"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Stats.cpp

"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\String_Base_Const.cpp

"$(INTDIR)\String_Base_Const.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SV_Message.cpp

"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SV_Message_Queue.cpp

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SV_Semaphore_Complex.cpp

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SV_Semaphore_Simple.cpp

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SV_Shared_Memory.cpp

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Svc_Conf_l.cpp

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Svc_Conf_Lexer_Guard.cpp

"$(INTDIR)\Svc_Conf_Lexer_Guard.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Svc_Conf_y.cpp

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Svc_Handler.cpp

"$(INTDIR)\Svc_Handler.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Synch.cpp

"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Synch_Options.cpp

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\System_Time.cpp

"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Task.cpp

"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Template_Instantiations.cpp

"$(INTDIR)\Template_Instantiations.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Thread.cpp

"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Thread_Adapter.cpp

"$(INTDIR)\Thread_Adapter.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Thread_Control.cpp

"$(INTDIR)\Thread_Control.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Thread_Exit.cpp

"$(INTDIR)\Thread_Exit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Thread_Hook.cpp

"$(INTDIR)\Thread_Hook.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Thread_Manager.cpp

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Time_Request_Reply.cpp

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Time_Value.cpp

"$(INTDIR)\Time_Value.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Timeprobe.cpp

"$(INTDIR)\Timeprobe.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Timer_Hash.cpp

"$(INTDIR)\Timer_Hash.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Timer_Heap.cpp

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Timer_List.cpp

"$(INTDIR)\Timer_List.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Timer_Queue.cpp

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Timer_Queue_Adapters.cpp

"$(INTDIR)\Timer_Queue_Adapters.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\Timer_Wheel.cpp

"$(INTDIR)\Timer_Wheel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TLI.cpp

"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TLI_Acceptor.cpp

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TLI_Connector.cpp

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TLI_Stream.cpp

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Token.cpp

"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Token_Collection.cpp

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Token_Invariants.cpp

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Token_Manager.cpp

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Token_Request_Reply.cpp

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TP_Reactor.cpp

"$(INTDIR)\TP_Reactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Trace.cpp

"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TTY_IO.cpp

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Typed_SV_Message.cpp

"$(INTDIR)\Typed_SV_Message.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Typed_SV_Message_Queue.cpp

"$(INTDIR)\Typed_SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)" ".\Typed_SV_Message.cpp"


SOURCE=.\UNIX_Addr.cpp

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UPIPE_Acceptor.cpp

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\UPIPE_Connector.cpp

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\UPIPE_Stream.cpp

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\WFMO_Reactor.cpp

"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WIN32_Asynch_IO.cpp

"$(INTDIR)\WIN32_Asynch_IO.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\WIN32_Proactor.cpp

"$(INTDIR)\WIN32_Proactor.obj" : $(SOURCE) "$(INTDIR)" ".\Framework_Component_T.cpp"


SOURCE=.\XML_Svc_Conf.cpp

"$(INTDIR)\XML_Svc_Conf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\XtReactor.cpp

"$(INTDIR)\XtReactor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Acceptor.cpp
SOURCE=.\Active_Map_Manager_T.cpp
SOURCE=.\Asynch_Acceptor.cpp
SOURCE=.\Atomic_Op.cpp

!IF  "$(CFG)" == "ACE LIB - Win64 Static Release"


"$(INTDIR)\Atomic_Op.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE LIB - Win64 Static Debug"

!ENDIF 

SOURCE=.\Auto_Ptr.cpp
SOURCE=.\Based_Pointer_T.cpp
SOURCE=.\Cache_Map_Manager_T.cpp
SOURCE=.\Cached_Connect_Strategy_T.cpp
SOURCE=.\Caching_Strategies_T.cpp
SOURCE=.\Caching_Utility_T.cpp
SOURCE=.\Cleanup_Strategies_T.cpp
SOURCE=.\Connector.cpp
SOURCE=.\Containers_T.cpp
SOURCE=.\Dump_T.cpp
SOURCE=.\Framework_Component_T.cpp

!IF  "$(CFG)" == "ACE LIB - Win64 Static Release"


"$(INTDIR)\Framework_Component_T.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE LIB - Win64 Static Debug"

!ENDIF 

SOURCE=.\Free_List.cpp
SOURCE=.\Functor_T.cpp
SOURCE=.\Future.cpp
SOURCE=.\Hash_Cache_Map_Manager_T.cpp
SOURCE=.\Hash_Map_Manager_T.cpp
SOURCE=.\Hash_Map_With_Allocator_T.cpp
SOURCE=.\IOStream_T.cpp
SOURCE=.\Local_Name_Space_T.cpp
SOURCE=.\Malloc_T.cpp
SOURCE=.\Managed_Object.cpp
SOURCE=.\Map_Manager.cpp
SOURCE=.\Map_T.cpp
SOURCE=.\Message_Block_T.cpp
SOURCE=.\Message_Queue_T.cpp
SOURCE=.\Module.cpp
SOURCE=.\Obstack_T.cpp
SOURCE=.\Pair_T.cpp
SOURCE=.\RB_Tree.cpp
SOURCE=.\Select_Reactor_T.cpp
SOURCE=.\Singleton.cpp
SOURCE=.\Strategies_T.cpp
SOURCE=.\Stream.cpp
SOURCE=.\Stream_Modules.cpp
SOURCE=.\String_Base.cpp
SOURCE=.\Synch_T.cpp
SOURCE=.\Task_T.cpp
SOURCE=.\Test_and_Set.cpp
SOURCE=.\Timer_Hash_T.cpp
SOURCE=.\Timer_Heap_T.cpp
SOURCE=.\Timer_List_T.cpp
SOURCE=.\Timer_Queue_T.cpp
SOURCE=.\Timer_Wheel_T.cpp
SOURCE=.\ace.rc

"$(INTDIR)\ace.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

