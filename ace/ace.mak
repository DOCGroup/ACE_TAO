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
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

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

ALL : "$(OUTDIR)\acer.dll"

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
	-@erase "$(INTDIR)\Scheduling_Params.obj"
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
	-@erase "$(INTDIR)\Thread_Priority.obj"
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
	-@erase "$(OUTDIR)\acer.dll"
	-@erase "$(OUTDIR)\acer.exp"
	-@erase "$(OUTDIR)\acer.lib"

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
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"acer.dll"
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/acer.pdb" /machine:I386 /out:"$(OUTDIR)/acer.dll"\
 /implib:"$(OUTDIR)/acer.lib" 
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
	"$(INTDIR)\Scheduling_Params.obj" \
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
	"$(INTDIR)\Thread_Priority.obj" \
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

"$(OUTDIR)\acer.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
	-@erase "$(INTDIR)\Scheduling_Params.obj"
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
	-@erase "$(INTDIR)\Thread_Priority.obj"
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
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /dll /debug /machine:I386
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
	"$(INTDIR)\Scheduling_Params.obj" \
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
	"$(INTDIR)\Thread_Priority.obj" \
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
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.i"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.i"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UNIX_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UNIX_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TRACE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Trace.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TRACE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Trace.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Collection.h"\
	{$(INCLUDE)}"\.\Token_Collection.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Collection.h"\
	{$(INCLUDE)}"\.\Token_Collection.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TLI_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Connector.h"\
	{$(INCLUDE)}"\.\TLI_Connector.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TLI_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Connector.h"\
	{$(INCLUDE)}"\.\TLI_Connector.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Acceptor.h"\
	{$(INCLUDE)}"\.\TLI_Acceptor.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TLI_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\TLI_Acceptor.h"\
	{$(INCLUDE)}"\.\TLI_Acceptor.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TLI_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TIMER=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TIMER=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TIME_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Request_Reply.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TIME_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Request_Reply.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_THREA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_THREA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_THREAD=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_THREAD=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SYSTE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\System_Time.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SYSTE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\System_Time.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SYNCH=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SYNCH=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SYNCH_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SYNCH_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SV_SH=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SV_SH=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SV_SE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SV_SE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SV_SEM=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SV_SEM=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SV_ME=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\SV_Message_Queue.h"\
	{$(INCLUDE)}"\.\SV_Message_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SV_ME=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\SV_Message_Queue.h"\
	{$(INCLUDE)}"\.\SV_Message_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SV_MES=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SV_MES=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SSTRI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SSTRI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SPIPE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SPIPE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SPIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SPIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SPIPE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SPIPE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SPIPE_AC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SPIPE_AC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SPIPE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SPIPE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_D=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_D=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_DG=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_DG=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_DGR=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_DGR=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SIGNA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SIGNA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SHARE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SHARE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SHARED=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\Shared_Memory_SV.h"\
	{$(INCLUDE)}"\.\Shared_Memory_SV.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SHARED=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\Shared_Memory_SV.h"\
	{$(INCLUDE)}"\.\Shared_Memory_SV.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SHARED_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\Shared_Memory_MM.h"\
	{$(INCLUDE)}"\.\Shared_Memory_MM.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SHARED_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\Shared_Memory_MM.h"\
	{$(INCLUDE)}"\.\Shared_Memory_MM.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVIC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVIC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\.\Auto_Ptr.h"\
	{$(INCLUDE)}"\.\Auto_Ptr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\.\Auto_Ptr.h"\
	{$(INCLUDE)}"\.\Auto_Ptr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REMOT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Remote_Tokens.h"\
	{$(INCLUDE)}"\.\Remote_Tokens.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Singleton.cpp"\
	{$(INCLUDE)}"\.\Singleton.h"\
	{$(INCLUDE)}"\.\Singleton.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REMOT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Remote_Tokens.h"\
	{$(INCLUDE)}"\.\Remote_Tokens.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Singleton.cpp"\
	{$(INCLUDE)}"\.\Singleton.h"\
	{$(INCLUDE)}"\.\Singleton.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REMOTE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REMOTE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_READ_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Read_Buffer.h"\
	{$(INCLUDE)}"\.\Read_Buffer.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_READ_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Read_Buffer.h"\
	{$(INCLUDE)}"\.\Read_Buffer.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REACT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REACT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PROFI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Profile_Timer.h"\
	{$(INCLUDE)}"\.\Profile_Timer.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROFI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Profile_Timer.h"\
	{$(INCLUDE)}"\.\Profile_Timer.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PROCE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Process_Manager.h"\
	{$(INCLUDE)}"\.\Process_Manager.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROCE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Process_Manager.h"\
	{$(INCLUDE)}"\.\Process_Manager.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PIPE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PARSE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PARSE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_OS_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Scheduling_Params.h"\
	{$(INCLUDE)}"\.\Scheduling_Params.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Priority.h"\
	{$(INCLUDE)}"\.\Thread_Priority.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_OS_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Scheduling_Params.h"\
	{$(INCLUDE)}"\.\Scheduling_Params.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Priority.h"\
	{$(INCLUDE)}"\.\Thread_Priority.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_OBSTA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_OBSTA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_NAMIN=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\Registry_Name_Space.h"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_NAMIN=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAMIN=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\Registry_Name_Space.h"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_NAMIN=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_NAME_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAME_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_NAME_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAME_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_NAME_P=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAME_P=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MULTI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Multiplexor.h"\
	{$(INCLUDE)}"\.\Multiplexor.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MULTI=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Multiplexor.h"\
	{$(INCLUDE)}"\.\Multiplexor.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MESSA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MESSA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MEMOR=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MEMOR=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MEM_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MEM_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MALLO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MALLO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Dgram.h"\
	{$(INCLUDE)}"\.\LSOCK_Dgram.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Dgram.h"\
	{$(INCLUDE)}"\.\LSOCK_Dgram.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Connector.h"\
	{$(INCLUDE)}"\.\LSOCK_Connector.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Connector.h"\
	{$(INCLUDE)}"\.\LSOCK_Connector.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.h"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_CO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.h"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LOG_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOG_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LOG_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOG_M=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LOCAL=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOCAL=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LOCAL_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOCAL_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Hash_Map_Manager.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_IPC_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_IPC_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_IO_SA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_IO_SA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INET_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_INET_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_INET_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_HIGH_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_HIGH_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_HANDL=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_HANDL=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_GET_O=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_GET_O=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FILE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FILE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FILE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\FILE_Connector.h"\
	{$(INCLUDE)}"\.\FILE_Connector.i"\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FILE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\FILE_Connector.h"\
	{$(INCLUDE)}"\.\FILE_Connector.i"\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FILE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FILE_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FILE_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FILE_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FIFO_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FIFO_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FIFO_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FIFO_S=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FIFO_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FIFO_R=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FIFO_RE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FIFO_RE=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_EVENT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_EVENT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DYNAM=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Dynamic.h"\
	{$(INCLUDE)}"\.\Dynamic.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DYNAM=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Dynamic.h"\
	{$(INCLUDE)}"\.\Dynamic.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DUMP_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Dump.h"\
	{$(INCLUDE)}"\.\Dump_T.cpp"\
	{$(INCLUDE)}"\.\Dump_T.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DUMP_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Dump.h"\
	{$(INCLUDE)}"\.\Dump_T.cpp"\
	{$(INCLUDE)}"\.\Dump_T.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DEV_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DEV_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DEV_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DEV_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DEV_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DEV_A=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DEV_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DEV_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_DATE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Date_Time.h"\
	{$(INCLUDE)}"\.\Date_Time.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_DATE_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Date_Time.h"\
	{$(INCLUDE)}"\.\Date_Time.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_CORBA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_CORBA=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_ARGV_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ARGV_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_ADDR_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ADDR_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_ACE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ACE_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_CP=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_FIFO_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_FIFO_C=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PROAC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROAC=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REACTO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REACTO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Invariants.h"\
	{$(INCLUDE)}"\.\Token_Invariants.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Token_Invariants.h"\
	{$(INCLUDE)}"\.\Token_Invariants.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PROCES=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Process.h"\
	{$(INCLUDE)}"\.\Process.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROCES=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Process.h"\
	{$(INCLUDE)}"\.\Process.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TTY_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\TTY_IO.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TTY_I=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\TTY_IO.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_ACTIV=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Activation_Queue.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ACTIV=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Activation_Queue.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_METHO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_METHO=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REGIS=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REGIS=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TASK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TASK_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_STRAT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_STRAT=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_REGIST=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\Registry_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_REGIST=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REGIST=\
	"..\STL\algobase.h"\
	"..\STL\bool.h"\
	"..\STL\bstring.h"\
	"..\STL\defalloc.h"\
	"..\STL\function.h"\
	"..\STL\iterator.h"\
	"..\STL\pair.h"\
	"..\STL\vector.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\Registry_Name_Space.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\iostream.h"\
	
NODEP_CPP_REGIST=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_List.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TIMER_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_List.obj" : $(SOURCE) $(DEP_CPP_TIMER_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TIMER_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_List.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_List.obj" : $(SOURCE) $(DEP_CPP_TIMER_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Heap.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TIMER_H=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Heap.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) $(DEP_CPP_TIMER_H) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TIMER_H=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Timer_Heap.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) $(DEP_CPP_TIMER_H) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Scheduling_Params.cpp
DEP_CPP_SCHED=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Scheduling_Params.h"\
	{$(INCLUDE)}"\.\Scheduling_Params.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Thread_Priority.h"\
	{$(INCLUDE)}"\.\Thread_Priority.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Scheduling_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Priority.cpp
DEP_CPP_THREAD_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\Thread_Priority.h"\
	{$(INCLUDE)}"\.\Thread_Priority.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	

"$(INTDIR)\Thread_Priority.obj" : $(SOURCE) $(DEP_CPP_THREAD_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
