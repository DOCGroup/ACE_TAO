# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=ace - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ace - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ace - Win32 Release" && "$(CFG)" != "ace - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace.mak" CFG="ace - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ace - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ace - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "ace - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "ace - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\ace-r.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\CORBA_Handler.obj"
	-@erase "$(INTDIR)\Date_Time.obj"
	-@erase "$(INTDIR)\DEV.obj"
	-@erase "$(INTDIR)\DEV_Addr.obj"
	-@erase "$(INTDIR)\DEV_Connector.obj"
	-@erase "$(INTDIR)\DEV_IO.obj"
	-@erase "$(INTDIR)\Dump.obj"
	-@erase "$(INTDIR)\Dynamic.obj"
	-@erase "$(INTDIR)\Event_Handler.obj"
	-@erase "$(INTDIR)\FIFO.obj"
	-@erase "$(INTDIR)\FIFO_Recv.obj"
	-@erase "$(INTDIR)\FIFO_Recv_Msg.obj"
	-@erase "$(INTDIR)\FIFO_Send.obj"
	-@erase "$(INTDIR)\FIFO_Send_Msg.obj"
	-@erase "$(INTDIR)\FILE.obj"
	-@erase "$(INTDIR)\FILE_Addr.obj"
	-@erase "$(INTDIR)\FILE_Connector.obj"
	-@erase "$(INTDIR)\FILE_IO.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IPC_SAP.obj"
	-@erase "$(INTDIR)\Local_Name_Space.obj"
	-@erase "$(INTDIR)\Local_Tokens.obj"
	-@erase "$(INTDIR)\Log_Msg.obj"
	-@erase "$(INTDIR)\Log_Record.obj"
	-@erase "$(INTDIR)\LSOCK.obj"
	-@erase "$(INTDIR)\LSOCK_Acceptor.obj"
	-@erase "$(INTDIR)\LSOCK_CODgram.obj"
	-@erase "$(INTDIR)\LSOCK_Connector.obj"
	-@erase "$(INTDIR)\LSOCK_Dgram.obj"
	-@erase "$(INTDIR)\LSOCK_Stream.obj"
	-@erase "$(INTDIR)\Malloc.obj"
	-@erase "$(INTDIR)\Mem_Map.obj"
	-@erase "$(INTDIR)\Memory_Pool.obj"
	-@erase "$(INTDIR)\Message_Block.obj"
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\ReactorEx.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Main.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Record.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Shared_Memory_MM.obj"
	-@erase "$(INTDIR)\Shared_Memory_SV.obj"
	-@erase "$(INTDIR)\Shared_Object.obj"
	-@erase "$(INTDIR)\Signal.obj"
	-@erase "$(INTDIR)\SOCK.obj"
	-@erase "$(INTDIR)\SOCK_Acceptor.obj"
	-@erase "$(INTDIR)\SOCK_CODgram.obj"
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
	-@erase "$(INTDIR)\Strategies.obj"
	-@erase "$(INTDIR)\SV_Message.obj"
	-@erase "$(INTDIR)\SV_Message_Queue.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Complex.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Simple.obj"
	-@erase "$(INTDIR)\SV_Shared_Memory.obj"
	-@erase "$(INTDIR)\Svc_Conf_l.obj"
	-@erase "$(INTDIR)\Svc_Conf_y.obj"
	-@erase "$(INTDIR)\Synch.obj"
	-@erase "$(INTDIR)\Synch_Options.obj"
	-@erase "$(INTDIR)\System_Time.obj"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread_Manager.obj"
	-@erase "$(INTDIR)\Time_Request_Reply.obj"
	-@erase "$(INTDIR)\Timer_Heap.obj"
	-@erase "$(INTDIR)\Timer_List.obj"
	-@erase "$(INTDIR)\Timer_Queue.obj"
	-@erase "$(INTDIR)\TLI.obj"
	-@erase "$(INTDIR)\TLI_Acceptor.obj"
	-@erase "$(INTDIR)\TLI_Connector.obj"
	-@erase "$(INTDIR)\TLI_Stream.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Token_Collection.obj"
	-@erase "$(INTDIR)\Token_Invariants.obj"
	-@erase "$(INTDIR)\Token_Manager.obj"
	-@erase "$(INTDIR)\Token_Request_Reply.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(OUTDIR)\ace-r.dll"
	-@erase "$(OUTDIR)\ace-r.exp"
	-@erase "$(OUTDIR)\ace-r.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ace.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /entry:"" /subsystem:windows /dll /machine:I386 /out:"ace-r.dll"
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/ace-r.pdb" /machine:I386 /out:"$(OUTDIR)/ace-r.dll"\
 /implib:"$(OUTDIR)/ace-r.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\CORBA_Handler.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Memory_Pool.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\ReactorEx.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Main.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Record.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
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
	"$(INTDIR)\Strategies.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Svc_Conf_l.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Time_Request_Reply.obj" \
	"$(INTDIR)\Timer_Heap.obj" \
	"$(INTDIR)\Timer_List.obj" \
	"$(INTDIR)\Timer_Queue.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj"

"$(OUTDIR)\ace-r.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\ace.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\CORBA_Handler.obj"
	-@erase "$(INTDIR)\Date_Time.obj"
	-@erase "$(INTDIR)\DEV.obj"
	-@erase "$(INTDIR)\DEV_Addr.obj"
	-@erase "$(INTDIR)\DEV_Connector.obj"
	-@erase "$(INTDIR)\DEV_IO.obj"
	-@erase "$(INTDIR)\Dump.obj"
	-@erase "$(INTDIR)\Dynamic.obj"
	-@erase "$(INTDIR)\Event_Handler.obj"
	-@erase "$(INTDIR)\FIFO.obj"
	-@erase "$(INTDIR)\FIFO_Recv.obj"
	-@erase "$(INTDIR)\FIFO_Recv_Msg.obj"
	-@erase "$(INTDIR)\FIFO_Send.obj"
	-@erase "$(INTDIR)\FIFO_Send_Msg.obj"
	-@erase "$(INTDIR)\FILE.obj"
	-@erase "$(INTDIR)\FILE_Addr.obj"
	-@erase "$(INTDIR)\FILE_Connector.obj"
	-@erase "$(INTDIR)\FILE_IO.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IPC_SAP.obj"
	-@erase "$(INTDIR)\Local_Name_Space.obj"
	-@erase "$(INTDIR)\Local_Tokens.obj"
	-@erase "$(INTDIR)\Log_Msg.obj"
	-@erase "$(INTDIR)\Log_Record.obj"
	-@erase "$(INTDIR)\LSOCK.obj"
	-@erase "$(INTDIR)\LSOCK_Acceptor.obj"
	-@erase "$(INTDIR)\LSOCK_CODgram.obj"
	-@erase "$(INTDIR)\LSOCK_Connector.obj"
	-@erase "$(INTDIR)\LSOCK_Dgram.obj"
	-@erase "$(INTDIR)\LSOCK_Stream.obj"
	-@erase "$(INTDIR)\Malloc.obj"
	-@erase "$(INTDIR)\Mem_Map.obj"
	-@erase "$(INTDIR)\Memory_Pool.obj"
	-@erase "$(INTDIR)\Message_Block.obj"
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\ReactorEx.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Main.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Record.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Shared_Memory_MM.obj"
	-@erase "$(INTDIR)\Shared_Memory_SV.obj"
	-@erase "$(INTDIR)\Shared_Object.obj"
	-@erase "$(INTDIR)\Signal.obj"
	-@erase "$(INTDIR)\SOCK.obj"
	-@erase "$(INTDIR)\SOCK_Acceptor.obj"
	-@erase "$(INTDIR)\SOCK_CODgram.obj"
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
	-@erase "$(INTDIR)\Strategies.obj"
	-@erase "$(INTDIR)\SV_Message.obj"
	-@erase "$(INTDIR)\SV_Message_Queue.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Complex.obj"
	-@erase "$(INTDIR)\SV_Semaphore_Simple.obj"
	-@erase "$(INTDIR)\SV_Shared_Memory.obj"
	-@erase "$(INTDIR)\Svc_Conf_l.obj"
	-@erase "$(INTDIR)\Svc_Conf_y.obj"
	-@erase "$(INTDIR)\Synch.obj"
	-@erase "$(INTDIR)\Synch_Options.obj"
	-@erase "$(INTDIR)\System_Time.obj"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread_Manager.obj"
	-@erase "$(INTDIR)\Time_Request_Reply.obj"
	-@erase "$(INTDIR)\Timer_Heap.obj"
	-@erase "$(INTDIR)\Timer_List.obj"
	-@erase "$(INTDIR)\Timer_Queue.obj"
	-@erase "$(INTDIR)\TLI.obj"
	-@erase "$(INTDIR)\TLI_Acceptor.obj"
	-@erase "$(INTDIR)\TLI_Connector.obj"
	-@erase "$(INTDIR)\TLI_Stream.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Token_Collection.obj"
	-@erase "$(INTDIR)\Token_Invariants.obj"
	-@erase "$(INTDIR)\Token_Manager.obj"
	-@erase "$(INTDIR)\Token_Request_Reply.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ace.dll"
	-@erase "$(OUTDIR)\ace.exp"
	-@erase "$(OUTDIR)\ace.ilk"
	-@erase "$(OUTDIR)\ace.lib"
	-@erase "$(OUTDIR)\ace.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ace.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 wsock32.lib /nologo /entry:"" /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=wsock32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/ace.pdb" /debug /machine:I386 /out:"$(OUTDIR)/ace.dll"\
 /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\CORBA_Handler.obj" \
	"$(INTDIR)\Date_Time.obj" \
	"$(INTDIR)\DEV.obj" \
	"$(INTDIR)\DEV_Addr.obj" \
	"$(INTDIR)\DEV_Connector.obj" \
	"$(INTDIR)\DEV_IO.obj" \
	"$(INTDIR)\Dump.obj" \
	"$(INTDIR)\Dynamic.obj" \
	"$(INTDIR)\Event_Handler.obj" \
	"$(INTDIR)\FIFO.obj" \
	"$(INTDIR)\FIFO_Recv.obj" \
	"$(INTDIR)\FIFO_Recv_Msg.obj" \
	"$(INTDIR)\FIFO_Send.obj" \
	"$(INTDIR)\FIFO_Send_Msg.obj" \
	"$(INTDIR)\FILE.obj" \
	"$(INTDIR)\FILE_Addr.obj" \
	"$(INTDIR)\FILE_Connector.obj" \
	"$(INTDIR)\FILE_IO.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IPC_SAP.obj" \
	"$(INTDIR)\Local_Name_Space.obj" \
	"$(INTDIR)\Local_Tokens.obj" \
	"$(INTDIR)\Log_Msg.obj" \
	"$(INTDIR)\Log_Record.obj" \
	"$(INTDIR)\LSOCK.obj" \
	"$(INTDIR)\LSOCK_Acceptor.obj" \
	"$(INTDIR)\LSOCK_CODgram.obj" \
	"$(INTDIR)\LSOCK_Connector.obj" \
	"$(INTDIR)\LSOCK_Dgram.obj" \
	"$(INTDIR)\LSOCK_Stream.obj" \
	"$(INTDIR)\Malloc.obj" \
	"$(INTDIR)\Mem_Map.obj" \
	"$(INTDIR)\Memory_Pool.obj" \
	"$(INTDIR)\Message_Block.obj" \
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\ReactorEx.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Main.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Record.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Shared_Memory_MM.obj" \
	"$(INTDIR)\Shared_Memory_SV.obj" \
	"$(INTDIR)\Shared_Object.obj" \
	"$(INTDIR)\Signal.obj" \
	"$(INTDIR)\SOCK.obj" \
	"$(INTDIR)\SOCK_Acceptor.obj" \
	"$(INTDIR)\SOCK_CODgram.obj" \
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
	"$(INTDIR)\Strategies.obj" \
	"$(INTDIR)\SV_Message.obj" \
	"$(INTDIR)\SV_Message_Queue.obj" \
	"$(INTDIR)\SV_Semaphore_Complex.obj" \
	"$(INTDIR)\SV_Semaphore_Simple.obj" \
	"$(INTDIR)\SV_Shared_Memory.obj" \
	"$(INTDIR)\Svc_Conf_l.obj" \
	"$(INTDIR)\Svc_Conf_y.obj" \
	"$(INTDIR)\Synch.obj" \
	"$(INTDIR)\Synch_Options.obj" \
	"$(INTDIR)\System_Time.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Thread_Manager.obj" \
	"$(INTDIR)\Time_Request_Reply.obj" \
	"$(INTDIR)\Timer_Heap.obj" \
	"$(INTDIR)\Timer_List.obj" \
	"$(INTDIR)\Timer_Queue.obj" \
	"$(INTDIR)\TLI.obj" \
	"$(INTDIR)\TLI_Acceptor.obj" \
	"$(INTDIR)\TLI_Connector.obj" \
	"$(INTDIR)\TLI_Stream.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Token_Collection.obj" \
	"$(INTDIR)\Token_Invariants.obj" \
	"$(INTDIR)\Token_Manager.obj" \
	"$(INTDIR)\Token_Request_Reply.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj"

"$(OUTDIR)\ace.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "ace - Win32 Release"
# Name "ace - Win32 Debug"

!IF  "$(CFG)" == "ace - Win32 Release"

!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\E\Malloc.h"\
	".\E\Signal.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\UPIPE_Connector.h"\
	".\UPIPE_Connector.i"\
	".\UPIPE_Stream.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Connector.h"\
	".\UPIPE_Connector.i"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\UPIPE_Acceptor.h"\
	".\UPIPE_Acceptor.i"\
	".\UPIPE_Stream.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_A=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\UPIPE_Acceptor.h"\
	".\UPIPE_Acceptor.i"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
DEP_CPP_UNIX_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
DEP_CPP_TOKEN=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
DEP_CPP_TOKEN_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
DEP_CPP_TOKEN_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Collection.h"\
	".\Token_Collection.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
DEP_CPP_TOKEN_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
DEP_CPP_TLI_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
DEP_CPP_TLI_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Connector.h"\
	".\TLI_Connector.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Acceptor.h"\
	".\TLI_Acceptor.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
DEP_CPP_TIMER=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Request_Reply.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
DEP_CPP_SYSTE=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Synch_Options.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_CO=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_CO=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
DEP_CPP_SPIPE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
DEP_CPP_SPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
DEP_CPP_SPIPE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
DEP_CPP_SPIPE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
DEP_CPP_SHARED=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_SV.h"\
	".\Shared_Memory_SV.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
DEP_CPP_SHARED_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_MM.h"\
	".\Shared_Memory_MM.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVIC=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp
DEP_CPP_SERVICE_M=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Remote_Tokens.h"\
	".\Remote_Tokens.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
DEP_CPP_READ_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Read_Buffer.h"\
	".\Read_Buffer.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_List.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PARSE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PARSE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	"..\STL\vector.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	{$(INCLUDE)}"\IOSTREAM.H"\
	
NODEP_CPP_NAMIN=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
DEP_CPP_NAME_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Name_Request_Reply.h"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
DEP_CPP_MULTI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Multiplexor.h"\
	".\Multiplexor.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Dgram.h"\
	".\LSOCK_Dgram.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
DEP_CPP_LSOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Connector.h"\
	".\LSOCK_Connector.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
DEP_CPP_LSOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_CODgram.h"\
	".\LSOCK_CODgram.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Acceptor.h"\
	".\LSOCK_Acceptor.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
DEP_CPP_LOG_M=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\FIFO_Send_Msg.h"\
	".\FIFO_Send_Msg.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INET_Addr.cpp
DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
DEP_CPP_FILE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_Connector.h"\
	".\FILE_Connector.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
DEP_CPP_FILE_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\FIFO_Send_Msg.h"\
	".\FIFO_Send_Msg.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\FIFO_Recv_Msg.h"\
	".\FIFO_Recv_Msg.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
DEP_CPP_EVENT=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
DEP_CPP_DEV_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
DEP_CPP_DEV_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
DEP_CPP_DATE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Date_Time.h"\
	".\Date_Time.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\CORBA_Handler.h"\
	".\CORBA_Handler.i"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
DEP_CPP_ACE_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
DEP_CPP_FIFO_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
DEP_CPP_PROAC=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\AcceptorEx.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Svc_Handler.cpp"\
	".\Svc_Handler.h"\
	".\Svc_Handler.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_List.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp
DEP_CPP_REACTO=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_List.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\E\Process.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\TTY_IO.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
DEP_CPP_ACTIV=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Method_Object.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Method_Object.h"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIS=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Registry.h"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\IOSTREAM.H"\
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
DEP_CPP_TASK_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
DEP_CPP_STRAT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
DEP_CPP_REGIST=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	"..\STL\vector.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Dispatcher.h"\
	".\Dispatcher.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\ReactorEx.h"\
	".\ReactorEx.i"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stack.cpp"\
	".\Stack.i"\
	".\stdcpp.h"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\e\MALLOC.H"\
	{$(INCLUDE)}"\e\SIGNAL.H"\
	{$(INCLUDE)}"\IOSTREAM.H"\
	
NODEP_CPP_REGIST=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_List.cpp
DEP_CPP_TIMER_=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_List.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Timer_List.obj" : $(SOURCE) $(DEP_CPP_TIMER_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Heap.cpp
DEP_CPP_TIMER_H=\
	"..\STL\Set.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) $(DEP_CPP_TIMER_H) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sched_Params.cpp
DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
