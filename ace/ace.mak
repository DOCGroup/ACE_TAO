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

NODEP_CPP_UPIPE=\
	".\ace\UPIPE_Stream.h"\
	".\ace\UPIPE_Stream.i"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_UPIPE_=\
	".\ace\UPIPE_Connector.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_UPIPE_A=\
	".\ace\UPIPE_Acceptor.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_A=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_UNIX_=\
	".\ace\UNIX_Addr.h"\
	".\ace\UNIX_Addr.i"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TRACE=\
	".\ace\ACE.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Trace.h"\
	".\ace\Trace.i"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TOKEN=\
	".\ace\Token_Request_Reply.h"\
	".\ace\Token_Request_Reply.i"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TOKEN_=\
	".\ace\Token_Manager.h"\
	".\ace\Token_Manager.i"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TOKEN_C=\
	".\ace\Token_Collection.h"\
	".\ace\Token_Collection.i"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TOKEN_CP=\
	".\ace\Thread.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TLI_S=\
	".\ace\TLI_Stream.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TLI_C=\
	".\ace\Handle_Set.h"\
	".\ace\TLI_Connector.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TLI_A=\
	".\ace\TLI_Acceptor.h"\
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TLI_CP=\
	".\ace\TLI.h"\
	

"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TIMER=\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue.i"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TIME_=\
	".\ace\Time_Request_Reply.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_THREA=\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_THREAD=\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SYSTE=\
	".\ace\System_Time.h"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SYNCH=\
	".\ace\Synch_Options.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SYNCH_=\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Thread.h"\
	

"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SVC_C=\
	".\ace\ARGV.h"\
	".\ace\Module.h"\
	".\ace\Stream.h"\
	".\ace\Svc_Conf.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SVC_CO=\
	".\ace\config.h"\
	".\ace\OS.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Svc_Conf_Tokens.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_CO=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SV_SH=\
	".\ace\SV_Shared_Memory.h"\
	".\ace\SV_Shared_Memory.i"\
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SV_SE=\
	".\ace\SV_Semaphore_Simple.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SV_SEM=\
	".\ace\SV_Semaphore_Complex.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SV_ME=\
	".\ace\SV_Message_Queue.h"\
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SV_MES=\
	".\ace\SV_Message.h"\
	".\ace\SV_Message.i"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SSTRI=\
	".\ace\Malloc.h"\
	".\ace\Service_Config.h"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SSTRI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SPIPE=\
	".\ace\SPIPE_Stream.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SPIPE_=\
	".\ace\SPIPE_Connector.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SPIPE_A=\
	".\ace\SPIPE_Addr.h"\
	".\ace\SPIPE_Addr.i"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SPIPE_AC=\
	".\ace\SPIPE_Acceptor.h"\
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SPIPE_C=\
	".\ace\SPIPE.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_=\
	".\ace\SOCK_Stream.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_I=\
	".\ace\SOCK_IO.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_D=\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Dgram_Mcast.h"\
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_DG=\
	".\ace\SOCK_Dgram_Bcast.h"\
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_DGR=\
	".\ace\SOCK_Dgram.h"\
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_C=\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Connector.h"\
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_CO=\
	".\ace\SOCK_CODgram.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_A=\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Acceptor.i"\
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SIGNA=\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SHARE=\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SHARED=\
	".\ace\Shared_Memory_SV.h"\
	".\ace\Shared_Memory_SV.i"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SHARED_=\
	".\ace\Shared_Memory_MM.h"\
	".\ace\Shared_Memory_MM.i"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVI=\
	".\ace\Service_Repository.h"\
	".\ace\Service_Repository.i"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVIC=\
	".\ace\Service_Record.h"\
	".\ace\Service_Record.i"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVIC=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVICE=\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVICE_=\
	".\ace\Get_Opt.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Manager.i"\
	".\ace\Service_Repository.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVICE_M=\
	".\ace\Service_Config.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_M=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SERVICE_C=\
	".\ace\ARGV.h"\
	".\ace\Auto_Ptr.h"\
	".\ace\Get_Opt.h"\
	".\ace\Malloc.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Record.h"\
	".\ace\Service_Repository.h"\
	".\ace\Set.h"\
	".\ace\Svc_Conf.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REMOT=\
	".\ace\Remote_Tokens.h"\
	".\ace\Remote_Tokens.i"\
	".\ace\Singleton.h"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REMOT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REMOTE=\
	".\ace\Remote_Name_Space.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REMOTE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_READ_=\
	".\ace\Read_Buffer.h"\
	".\ace\Service_Config.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_READ_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REACT=\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Synch_T.h"\
	".\ace\Timer_List.h"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REACT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PROFI=\
	".\ace\Profile_Timer.h"\
	".\ace\Profile_Timer.i"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PROCE=\
	".\ace\Process_Manager.h"\
	".\ace\Process_Manager.i"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PIPE_=\
	".\ace\Pipe.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PARSE=\
	".\ace\Parse_Node.h"\
	".\ace\Parse_Node.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\ace\Task.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PARSE=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_OS_CP=\
	".\ace\ARGV.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Scheduling_Params.h"\
	".\ace\Set.h"\
	".\ace\Synch.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	".\Scheduling_Params.h"\
	".\Scheduling_Params.i"\
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
	".\Thread_Priority.h"\
	".\Thread_Priority.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_OBSTA=\
	".\ace\Obstack.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_NAMIN=\
	".\ace\Get_Opt.h"\
	".\ace\Local_Name_Space.h"\
	".\ace\Naming_Context.h"\
	".\ace\Registry_Name_Space.h"\
	".\ace\Remote_Name_Space.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	
NODEP_CPP_NAMIN=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_NAME_=\
	".\ace\Name_Space.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAME_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_NAME_R=\
	".\ace\Name_Request_Reply.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_NAME_P=\
	".\ace\Name_Proxy.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_NAME_P=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_MULTI=\
	".\ace\Multiplexor.h"\
	".\ace\Multiplexor.i"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MULTI=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_MESSA=\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\Service_Config.h"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MESSA=\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_MEMOR=\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_MEM_M=\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_MALLO=\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK=\
	".\ace\LSOCK_Stream.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK_=\
	".\ace\LSOCK_Dgram.h"\
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK_C=\
	".\ace\LSOCK_Connector.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK_CO=\
	".\ace\LSOCK_CODgram.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK_A=\
	".\ace\LSOCK_Acceptor.h"\
	".\ace\LSOCK_Acceptor.i"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LSOCK_CP=\
	".\ace\LSOCK.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LOG_R=\
	".\ace\Log_Record.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LOG_M=\
	".\ace\ACE.h"\
	".\ace\config.h"\
	".\ace\FIFO_Send_Msg.h"\
	".\ace\Signal.h"\
	".\ace\SPIPE_Connector.h"\
	".\ace\Synch.h"\
	".\ace\Thread.h"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LOCAL=\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Thread.h"\
	".\ace\Token_Manager.h"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOCAL=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_LOCAL_=\
	".\ace\ACE.h"\
	".\ace\Local_Name_Space.h"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LOCAL_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_IPC_S=\
	".\ace\IPC_SAP.h"\
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_IO_SA=\
	".\ace\IO_SAP.h"\
	

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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

NODEP_CPP_INET_=\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_HIGH_=\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_HANDL=\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_GET_O=\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FILE_=\
	".\ace\FILE_IO.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FILE_C=\
	".\ace\FILE_Connector.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FILE_A=\
	".\ace\FILE_Addr.h"\
	".\ace\FILE_Addr.i"\
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FILE_CP=\
	".\ace\FILE.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FIFO_=\
	".\ace\FIFO_Send_Msg.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FIFO_S=\
	".\ace\FIFO_Send.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FIFO_R=\
	".\ace\FIFO_Recv_Msg.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FIFO_RE=\
	".\ace\FIFO_Recv.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_EVENT=\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Message_Block.h"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DYNAM=\
	".\ace\Dynamic.h"\
	".\ace\Dynamic.i"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DUMP_=\
	".\ace\Dump.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DEV_I=\
	".\ace\DEV_IO.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DEV_C=\
	".\ace\DEV_Connector.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DEV_A=\
	".\ace\DEV_Addr.h"\
	".\ace\DEV_Addr.i"\
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DEV_CP=\
	".\ace\DEV.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_DATE_=\
	".\ace\Date_Time.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_CORBA=\
	".\ace\CORBA_Handler.h"\
	".\ace\CORBA_Handler.i"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_ARGV_=\
	".\ace\ARGV.h"\
	".\ace\ARGV.i"\
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_ADDR_=\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	

"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_ACE_C=\
	".\ace\ACE.h"\
	".\ace\Handle_Set.h"\
	".\ace\IPC_SAP.h"\
	".\ace\Reactor.h"\
	".\ace\Thread_Manager.h"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ACE_C=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_SOCK_CP=\
	".\ace\SOCK.h"\
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_FIFO_C=\
	".\ace\FIFO.h"\
	".\ace\FIFO.i"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PROAC=\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Timer_List.h"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PROAC=\
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
	".\Proactor.h"\
	".\Proactor.i"\
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
	".\Time_Value.h"\
	".\Timer_List.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REACTO=\
	".\ace\ReactorEx.h"\
	".\ace\ReactorEx.i"\
	".\ace\Thread.h"\
	".\ace\Timer_List.h"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_REACTO=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TOKEN_I=\
	".\ace\Token_Invariants.h"\
	".\ace\Token_Invariants.i"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TOKEN_I=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_PROCES=\
	".\ace\ARGV.h"\
	".\ace\Process.h"\
	".\ace\Process.i"\
	".\ace\SString.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TTY_I=\
	".\ace\TTY_IO.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_ACTIV=\
	".\ace\Activation_Queue.h"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_ACTIV=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_METHO=\
	".\ace\Method_Object.h"\
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REGIS=\
	".\ace\Registry.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


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
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TASK_=\
	".\ace\Module.h"\
	".\ace\Service_Config.h"\
	".\ace\Task.h"\
	".\ace\Task.i"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TASK_=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_STRAT=\
	".\ace\Reactor.h"\
	".\ace\ReactorEx.h"\
	".\ace\Strategies.h"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_STRAT=\
	"..\STL\Set.h"\
	"..\STL\Stack.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\config.h"\
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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_REGIST=\
	".\ace\Registry_Name_Space.h"\
	

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\E\Malloc.h"\
	{$(INCLUDE)}"\E\Signal.h"\
	
NODEP_CPP_REGIST=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_List.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TIMER_=\
	".\ace\Timer_List.h"\
	

"$(INTDIR)\Timer_List.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Heap.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

NODEP_CPP_TIMER_H=\
	".\ace\Timer_Heap.h"\
	

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TIMER_H=\
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
	".\Timer_Heap.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Timer_Heap.obj" : $(SOURCE) $(DEP_CPP_TIMER_H) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Scheduling_Params.cpp
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
	".\Scheduling_Params.h"\
	".\Scheduling_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\stdcpp.h"\
	".\Thread_Priority.h"\
	".\Thread_Priority.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Scheduling_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Priority.cpp
DEP_CPP_THREAD_=\
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
	".\Thread_Priority.h"\
	".\Thread_Priority.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

"$(INTDIR)\Thread_Priority.obj" : $(SOURCE) $(DEP_CPP_THREAD_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
