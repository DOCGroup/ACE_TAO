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
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "ace - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\ace.dll"

CLEAN : 
<<<<<<< ace.mak
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
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
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
	-@erase "$(INTDIR)\Time_Value.obj"
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
	-@erase "$(OUTDIR)\ace.dll"
	-@erase "$(OUTDIR)\ace.exp"
	-@erase "$(OUTDIR)\ace.lib"
=======
	-@erase ".\ace.dll"
	-@erase ".\Release\TLI_Acceptor.obj"
	-@erase ".\Release\DEV_Addr.obj"
	-@erase ".\Release\SOCK_CODgram.obj"
	-@erase ".\Release\Activation_Queue.obj"
	-@erase ".\Release\Service_Object.obj"
	-@erase ".\Release\Dynamic.obj"
	-@erase ".\Release\ARGV.obj"
	-@erase ".\Release\Token.obj"
	-@erase ".\Release\SOCK_Dgram.obj"
	-@erase ".\Release\TTY_IO.obj"
	-@erase ".\Release\Name_Proxy.obj"
	-@erase ".\Release\OS.obj"
	-@erase ".\Release\System_Time.obj"
	-@erase ".\Release\Dump.obj"
	-@erase ".\Release\INET_Addr.obj"
	-@erase ".\Release\FIFO_Send_Msg.obj"
	-@erase ".\Release\DEV.obj"
	-@erase ".\Release\CORBA_Handler.obj"
	-@erase ".\Release\SPIPE_Addr.obj"
	-@erase ".\Release\Service_Repository.obj"
	-@erase ".\Release\Token_Manager.obj"
	-@erase ".\Release\FILE_Connector.obj"
	-@erase ".\Release\UPIPE_Connector.obj"
	-@erase ".\Release\Process.obj"
	-@erase ".\Release\LSOCK.obj"
	-@erase ".\Release\FILE_IO.obj"
	-@erase ".\Release\Log_Record.obj"
	-@erase ".\Release\Date_Time.obj"
	-@erase ".\Release\Service_Main.obj"
	-@erase ".\Release\Synch_Options.obj"
	-@erase ".\Release\SOCK_Stream.obj"
	-@erase ".\Release\Service_Record.obj"
	-@erase ".\Release\SV_Message_Queue.obj"
	-@erase ".\Release\Get_Opt.obj"
	-@erase ".\Release\LSOCK_Stream.obj"
	-@erase ".\Release\Service_Config.obj"
	-@erase ".\Release\TLI_Stream.obj"
	-@erase ".\Release\UNIX_Addr.obj"
	-@erase ".\Release\SOCK.obj"
	-@erase ".\Release\High_Res_Timer.obj"
	-@erase ".\Release\SOCK_Connector.obj"
	-@erase ".\Release\Read_Buffer.obj"
	-@erase ".\Release\SString.obj"
	-@erase ".\Release\SOCK_Acceptor.obj"
	-@erase ".\Release\Profile_Timer.obj"
	-@erase ".\Release\Addr.obj"
	-@erase ".\Release\ReactorEx.obj"
	-@erase ".\Release\Thread.obj"
	-@erase ".\Release\LSOCK_Dgram.obj"
	-@erase ".\Release\Token_Invariants.obj"
	-@erase ".\Release\Log_Msg.obj"
	-@erase ".\Release\Mem_Map.obj"
	-@erase ".\Release\LSOCK_CODgram.obj"
	-@erase ".\Release\Token_Collection.obj"
	-@erase ".\Release\Name_Request_Reply.obj"
	-@erase ".\Release\SOCK_Dgram_Bcast.obj"
	-@erase ".\Release\Synch.obj"
	-@erase ".\Release\Shared_Memory_MM.obj"
	-@erase ".\Release\TLI.obj"
	-@erase ".\Release\FIFO.obj"
	-@erase ".\Release\Malloc.obj"
	-@erase ".\Release\Service_Manager.obj"
	-@erase ".\Release\IO_Cntl_Msg.obj"
	-@erase ".\Release\TLI_Connector.obj"
	-@erase ".\Release\SV_Shared_Memory.obj"
	-@erase ".\Release\Name_Space.obj"
	-@erase ".\Release\Svc_Conf_l.obj"
	-@erase ".\Release\DEV_IO.obj"
	-@erase ".\Release\FIFO_Recv.obj"
	-@erase ".\Release\LSOCK_Connector.obj"
	-@erase ".\Release\SV_Semaphore_Simple.obj"
	-@erase ".\Release\Memory_Pool.obj"
	-@erase ".\Release\Local_Tokens.obj"
	-@erase ".\Release\Local_Name_Space.obj"
	-@erase ".\Release\FIFO_Recv_Msg.obj"
	-@erase ".\Release\Method_Object.obj"
	-@erase ".\Release\Registry.obj"
	-@erase ".\Release\Svc_Conf_y.obj"
	-@erase ".\Release\SPIPE_Connector.obj"
	-@erase ".\Release\Parse_Node.obj"
	-@erase ".\Release\Process_Manager.obj"
	-@erase ".\Release\LSOCK_Acceptor.obj"
	-@erase ".\Release\ACE.obj"
	-@erase ".\Release\Shared_Object.obj"
	-@erase ".\Release\SPIPE_Stream.obj"
	-@erase ".\Release\Time_Value.obj"
	-@erase ".\Release\Signal.obj"
	-@erase ".\Release\Handle_Set.obj"
	-@erase ".\Release\Shared_Memory_SV.obj"
	-@erase ".\Release\Event_Handler.obj"
	-@erase ".\Release\Time_Request_Reply.obj"
	-@erase ".\Release\Remote_Name_Space.obj"
	-@erase ".\Release\Trace.obj"
	-@erase ".\Release\IPC_SAP.obj"
	-@erase ".\Release\Obstack.obj"
	-@erase ".\Release\SV_Message.obj"
	-@erase ".\Release\SPIPE.obj"
	-@erase ".\Release\UPIPE_Stream.obj"
	-@erase ".\Release\Proactor.obj"
	-@erase ".\Release\SOCK_Dgram_Mcast.obj"
	-@erase ".\Release\UPIPE_Acceptor.obj"
	-@erase ".\Release\Token_Request_Reply.obj"
	-@erase ".\Release\SPIPE_Acceptor.obj"
	-@erase ".\Release\DEV_Connector.obj"
	-@erase ".\Release\Pipe.obj"
	-@erase ".\Release\Multiplexor.obj"
	-@erase ".\Release\SOCK_IO.obj"
	-@erase ".\Release\FILE_Addr.obj"
	-@erase ".\Release\Message_Block.obj"
	-@erase ".\Release\IO_SAP.obj"
	-@erase ".\Release\Naming_Context.obj"
	-@erase ".\Release\Reactor.obj"
	-@erase ".\Release\Remote_Tokens.obj"
	-@erase ".\Release\Timer_Queue.obj"
	-@erase ".\Release\FILE.obj"
	-@erase ".\Release\Thread_Manager.obj"
	-@erase ".\Release\Task.obj"
	-@erase ".\Release\FIFO_Send.obj"
	-@erase ".\Release\SV_Semaphore_Complex.obj"
	-@erase ".\Release\Strategies.obj"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"
>>>>>>> 4.8

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/ace.pdb" /machine:I386 /out:"$(OUTDIR)/ace.dll"\
 /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
<<<<<<< ace.mak
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
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
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
	"$(INTDIR)\Time_Value.obj" \
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
=======
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/Activation_Queue.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/TTY_IO.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/Method_Object.obj" \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/Task.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/Strategies.obj"
>>>>>>> 4.8

"$(OUTDIR)\ace.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\ace.dll"

CLEAN : 
<<<<<<< ace.mak
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
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
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
	-@erase "$(INTDIR)\Time_Value.obj"
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
=======
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\ace.dll"
	-@erase ".\Debug\DEV_Connector.obj"
	-@erase ".\Debug\Svc_Conf_y.obj"
	-@erase ".\Debug\SV_Semaphore_Complex.obj"
	-@erase ".\Debug\Dynamic.obj"
	-@erase ".\Debug\LSOCK_Stream.obj"
	-@erase ".\Debug\Malloc.obj"
	-@erase ".\Debug\Synch.obj"
	-@erase ".\Debug\SOCK_Dgram_Mcast.obj"
	-@erase ".\Debug\Time_Value.obj"
	-@erase ".\Debug\Token_Invariants.obj"
	-@erase ".\Debug\TLI_Connector.obj"
	-@erase ".\Debug\DEV_Addr.obj"
	-@erase ".\Debug\Handle_Set.obj"
	-@erase ".\Debug\UPIPE_Acceptor.obj"
	-@erase ".\Debug\Shared_Memory_MM.obj"
	-@erase ".\Debug\SPIPE_Acceptor.obj"
	-@erase ".\Debug\Date_Time.obj"
	-@erase ".\Debug\Token.obj"
	-@erase ".\Debug\UPIPE_Stream.obj"
	-@erase ".\Debug\Memory_Pool.obj"
	-@erase ".\Debug\Time_Request_Reply.obj"
	-@erase ".\Debug\DEV_IO.obj"
	-@erase ".\Debug\SV_Message.obj"
	-@erase ".\Debug\SV_Semaphore_Simple.obj"
	-@erase ".\Debug\Process.obj"
	-@erase ".\Debug\SV_Shared_Memory.obj"
	-@erase ".\Debug\FILE_IO.obj"
	-@erase ".\Debug\SOCK_Stream.obj"
	-@erase ".\Debug\Naming_Context.obj"
	-@erase ".\Debug\ACE.obj"
	-@erase ".\Debug\FILE.obj"
	-@erase ".\Debug\FILE_Addr.obj"
	-@erase ".\Debug\Task.obj"
	-@erase ".\Debug\Service_Manager.obj"
	-@erase ".\Debug\Thread_Manager.obj"
	-@erase ".\Debug\FIFO_Recv_Msg.obj"
	-@erase ".\Debug\OS.obj"
	-@erase ".\Debug\Method_Object.obj"
	-@erase ".\Debug\Get_Opt.obj"
	-@erase ".\Debug\LSOCK.obj"
	-@erase ".\Debug\Local_Name_Space.obj"
	-@erase ".\Debug\Service_Object.obj"
	-@erase ".\Debug\Shared_Object.obj"
	-@erase ".\Debug\Signal.obj"
	-@erase ".\Debug\LSOCK_Connector.obj"
	-@erase ".\Debug\FIFO_Send.obj"
	-@erase ".\Debug\LSOCK_Dgram.obj"
	-@erase ".\Debug\SString.obj"
	-@erase ".\Debug\Dump.obj"
	-@erase ".\Debug\Local_Tokens.obj"
	-@erase ".\Debug\Event_Handler.obj"
	-@erase ".\Debug\Synch_Options.obj"
	-@erase ".\Debug\Trace.obj"
	-@erase ".\Debug\Mem_Map.obj"
	-@erase ".\Debug\SOCK_Dgram.obj"
	-@erase ".\Debug\Name_Proxy.obj"
	-@erase ".\Debug\FILE_Connector.obj"
	-@erase ".\Debug\Shared_Memory_SV.obj"
	-@erase ".\Debug\IO_Cntl_Msg.obj"
	-@erase ".\Debug\SPIPE_Connector.obj"
	-@erase ".\Debug\Token_Request_Reply.obj"
	-@erase ".\Debug\INET_Addr.obj"
	-@erase ".\Debug\Process_Manager.obj"
	-@erase ".\Debug\SPIPE_Stream.obj"
	-@erase ".\Debug\Message_Block.obj"
	-@erase ".\Debug\IO_SAP.obj"
	-@erase ".\Debug\SPIPE_Addr.obj"
	-@erase ".\Debug\Service_Record.obj"
	-@erase ".\Debug\Service_Repository.obj"
	-@erase ".\Debug\Service_Main.obj"
	-@erase ".\Debug\Remote_Tokens.obj"
	-@erase ".\Debug\Log_Record.obj"
	-@erase ".\Debug\Service_Config.obj"
	-@erase ".\Debug\Parse_Node.obj"
	-@erase ".\Debug\LSOCK_CODgram.obj"
	-@erase ".\Debug\SOCK.obj"
	-@erase ".\Debug\High_Res_Timer.obj"
	-@erase ".\Debug\SOCK_Connector.obj"
	-@erase ".\Debug\System_Time.obj"
	-@erase ".\Debug\Addr.obj"
	-@erase ".\Debug\Token_Collection.obj"
	-@erase ".\Debug\TTY_IO.obj"
	-@erase ".\Debug\DEV.obj"
	-@erase ".\Debug\TLI_Stream.obj"
	-@erase ".\Debug\SOCK_Dgram_Bcast.obj"
	-@erase ".\Debug\Remote_Name_Space.obj"
	-@erase ".\Debug\Pipe.obj"
	-@erase ".\Debug\UNIX_Addr.obj"
	-@erase ".\Debug\Activation_Queue.obj"
	-@erase ".\Debug\FIFO.obj"
	-@erase ".\Debug\FIFO_Send_Msg.obj"
	-@erase ".\Debug\Registry.obj"
	-@erase ".\Debug\ReactorEx.obj"
	-@erase ".\Debug\CORBA_Handler.obj"
	-@erase ".\Debug\TLI_Acceptor.obj"
	-@erase ".\Debug\Token_Manager.obj"
	-@erase ".\Debug\IPC_SAP.obj"
	-@erase ".\Debug\SOCK_CODgram.obj"
	-@erase ".\Debug\Obstack.obj"
	-@erase ".\Debug\Name_Request_Reply.obj"
	-@erase ".\Debug\Multiplexor.obj"
	-@erase ".\Debug\Name_Space.obj"
	-@erase ".\Debug\SOCK_IO.obj"
	-@erase ".\Debug\ARGV.obj"
	-@erase ".\Debug\SPIPE.obj"
	-@erase ".\Debug\Svc_Conf_l.obj"
	-@erase ".\Debug\Read_Buffer.obj"
	-@erase ".\Debug\Timer_Queue.obj"
	-@erase ".\Debug\Proactor.obj"
	-@erase ".\Debug\FIFO_Recv.obj"
	-@erase ".\Debug\Reactor.obj"
	-@erase ".\Debug\SV_Message_Queue.obj"
	-@erase ".\Debug\Log_Msg.obj"
	-@erase ".\Debug\UPIPE_Connector.obj"
	-@erase ".\Debug\LSOCK_Acceptor.obj"
	-@erase ".\Debug\TLI.obj"
	-@erase ".\Debug\SOCK_Acceptor.obj"
	-@erase ".\Debug\Profile_Timer.obj"
	-@erase ".\Debug\Thread.obj"
	-@erase ".\Debug\Strategies.obj"
	-@erase ".\ace.ilk"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"
	-@erase ".\ace.pdb"
>>>>>>> 4.8

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /Zi /Od /I "..\STL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /Zi /Od /I "..\STL" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib advapi32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/ace.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ace.dll" /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
<<<<<<< ace.mak
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
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
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
	"$(INTDIR)\Time_Value.obj" \
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
=======
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/Task.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/Method_Object.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/TTY_IO.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/Activation_Queue.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/Strategies.obj"
>>>>>>> 4.8

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
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

=======
>>>>>>> 4.8
DEP_CPP_UPIPE=\
<<<<<<< ace.mak
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SPIPE.h"\
	{$(INCLUDE)}"\ace\SPIPE.i"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stream.cpp"\
	{$(INCLUDE)}"\ace\Stream.h"\
	{$(INCLUDE)}"\ace\Stream.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Task.cpp"\
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\UPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


<<<<<<< ace.mak
!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

=======
>>>>>>> 4.8
# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
DEP_CPP_UPIPE_=\
<<<<<<< ace.mak
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SPIPE.h"\
	{$(INCLUDE)}"\ace\SPIPE.i"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.i"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stream.cpp"\
	{$(INCLUDE)}"\ace\Stream.h"\
	{$(INCLUDE)}"\ace\Stream.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Task.cpp"\
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\UPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\UPIPE_Connector.h"\
	{$(INCLUDE)}"\ace\UPIPE_Connector.i"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE_=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\UPIPE_Connector.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
>>>>>>> 4.8
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


<<<<<<< ace.mak
!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
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
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File
=======
# End Source File
################################################################################
# Begin Source File
>>>>>>> 4.8

<<<<<<< ace.mak
SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SPIPE.h"\
	{$(INCLUDE)}"\ace\SPIPE.i"\
	{$(INCLUDE)}"\ace\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.i"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stream.cpp"\
	{$(INCLUDE)}"\ace\Stream.h"\
	{$(INCLUDE)}"\ace\Stream.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Task.cpp"\
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\ace\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\ace\UPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE_A=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_A=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
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
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UPIPE_A=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\UPIPE_Acceptor.cpp
DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\UPIPE_Addr.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
>>>>>>> 4.8
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\UNIX_Addr.cpp
DEP_CPP_UNIX_=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_UNIX_=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\UNIX_Addr.cpp
DEP_CPP_UNIX_=\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
DEP_CPP_TRACE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	".\Trace.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TRACE=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Trace.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
DEP_CPP_TOKEN=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TOKEN=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
DEP_CPP_TOKEN_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TOKEN_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
DEP_CPP_TOKEN_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Collection.h"\
	".\Token_Collection.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TOKEN_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Token_Collection.h"\
	{$(INCLUDE)}"\.\Token_Collection.i"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\SString.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
DEP_CPP_TOKEN_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TOKEN_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
DEP_CPP_TLI_S=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TLI_S=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
DEP_CPP_TLI_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Connector.h"\
	".\TLI_Connector.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TLI_C=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\TLI_Connector.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Connector.i"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Acceptor.h"\
	".\TLI_Acceptor.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TLI_A=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\TLI_Acceptor.h"\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\TLI_Acceptor.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\TLI.h"\
	".\TLI.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TLI_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
DEP_CPP_TIMER=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TIMER=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Value.cpp
DEP_CPP_TIME_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TIME_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Time_Value.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_R=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Request_Reply.h"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TIME_R=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Time_Request_Reply.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_THREA=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREAD=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_THREAD=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
DEP_CPP_SYSTE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SYSTE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\System_Time.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
>>>>>>> 4.8
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
DEP_CPP_SYNCH=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Synch_Options.h"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SYNCH=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SYNCH_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_C=\
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SVC_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_C=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SVC_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\Svc_Conf_y.cpp
DEP_CPP_SVC_C=\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_CO=\
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SVC_CO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\Svc_Conf_l.cpp
DEP_CPP_SVC_CO=\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_CO=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SVC_CO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SV_SH=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SV_SE=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SV_SEM=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SV_ME=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	{$(INCLUDE)}"\.\SV_Message_Queue.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Message.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SV_MES=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SSTRI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SPIPE_Stream.cpp
DEP_CPP_SPIPE=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SPIPE=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SPIPE_Stream.cpp
DEP_CPP_SPIPE=\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SPIPE_Connector.cpp
DEP_CPP_SPIPE_=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SPIPE_=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SPIPE_Connector.cpp
DEP_CPP_SPIPE_=\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SPIPE_Addr.cpp
DEP_CPP_SPIPE_A=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SPIPE_A=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SPIPE_Addr.cpp
DEP_CPP_SPIPE_A=\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SPIPE_AC=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SPIPE.cpp
DEP_CPP_SPIPE_C=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SPIPE_C=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SPIPE.cpp
DEP_CPP_SPIPE_C=\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_Stream.cpp
DEP_CPP_SOCK_=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_Stream.cpp
DEP_CPP_SOCK_=\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_IO.cpp
DEP_CPP_SOCK_I=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_I=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_IO.cpp
DEP_CPP_SOCK_I=\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_Dgram_Mcast.cpp
DEP_CPP_SOCK_D=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_D=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_Dgram_Mcast.cpp
DEP_CPP_SOCK_D=\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Mcast.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_DG=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_DGR=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\SOCK_Connector.cpp
DEP_CPP_SOCK_C=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_C=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\SOCK_Connector.cpp
DEP_CPP_SOCK_C=\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	
<<<<<<< ace.mak

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
DEP_CPP_SOCK_CO=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
=======

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
>>>>>>> 4.8
	
<<<<<<< ace.mak
NODEP_CPP_SOCK_CO=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_A=\
	".\..\STL\mutex.h"\
=======

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SIGNA=\
	".\..\STL\mutex.h"\
=======
SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
>>>>>>> 4.8
	
<<<<<<< ace.mak

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
DEP_CPP_SHARE=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SHARE=\
	".\..\STL\mutex.h"\
=======

"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
DEP_CPP_SHARE=\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Shared_Memory_SV.cpp
DEP_CPP_SHARED=\
	".\..\STL\bstring.h"\
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
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SHARED=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
DEP_CPP_SHARED_=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SHARED_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVI=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\IO_Cntl_Msg.h"\
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
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.h"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Set.cpp"\
	".\Set.h"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVI=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\Shared_Memory_SV.cpp
DEP_CPP_SHARED=\
	{$(INCLUDE)}"\.\Shared_Memory_SV.h"\
	{$(INCLUDE)}"\.\Shared_Memory_SV.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
DEP_CPP_SHARED_=\
	{$(INCLUDE)}"\.\Shared_Memory_MM.h"\
	{$(INCLUDE)}"\.\Shared_Memory_MM.i"\
	{$(INCLUDE)}"\.\Shared_Memory.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
DEP_CPP_SERVI=\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Service_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVIC=\
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\IO_Cntl_Msg.h"\
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
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.h"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Record.h"\
	".\Service_Record.i"\
	".\Set.cpp"\
	".\Set.h"\
	".\Set.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVIC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\Service_Record.cpp
DEP_CPP_SERVIC=\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
>>>>>>> 4.8
	
<<<<<<< ace.mak

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVIC=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVIC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVICE=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE=\
	".\..\STL\mutex.h"\
=======

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVICE=\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

=======
SOURCE=.\Service_Manager.cpp
>>>>>>> 4.8
DEP_CPP_SERVICE_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


<<<<<<< ace.mak
!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

=======
>>>>>>> 4.8
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp
DEP_CPP_SERVICE_M=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE_M=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_C=\
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_C=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SERVICE_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
SOURCE=.\Service_Config.cpp
DEP_CPP_SERVICE_C=\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Manager.h"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Auto_Ptr.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Manager.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Auto_Ptr.i"\
	{$(INCLUDE)}"\.\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_REMOT=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Remote_Tokens.h"\
	{$(INCLUDE)}"\.\Singleton.h"\
	{$(INCLUDE)}"\.\Remote_Tokens.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Singleton.i"\
	{$(INCLUDE)}"\.\Singleton.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_REMOTE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
DEP_CPP_READ_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_READ_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Read_Buffer.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Read_Buffer.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_REACT=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
DEP_CPP_PROFI=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PROFI=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Profile_Timer.h"\
	{$(INCLUDE)}"\.\Profile_Timer.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PROCE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PIPE_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Process_Manager.h"\
	{$(INCLUDE)}"\.\Process_Manager.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PARSE=\
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task.h"\
	".\Task.i"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PARSE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

=======
SOURCE=.\Parse_Node.cpp
>>>>>>> 4.8
DEP_CPP_PARSE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PARSE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Service_Repository.h"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.cpp"\
	".\Set.h"\
	".\Set.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_OS_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
DEP_CPP_OBSTA=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_OBSTA=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Set.cpp"\
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_NAMIN=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
DEP_CPP_NAME_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_NAME_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_NAME_R=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_NAME_P=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

<<<<<<< ace.mak
SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MULTI=\
	".\..\STL\bstring.h"\
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
	".\Module.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MULTI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

=======
SOURCE=.\Multiplexor.cpp
>>>>>>> 4.8
DEP_CPP_MULTI=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MULTI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Multiplexor.h"\
	{$(INCLUDE)}"\.\Multiplexor.i"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MESSA=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MEMOR=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
DEP_CPP_MEM_M=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MEM_M=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_MALLO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LSOCK=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
>>>>>>> 4.8
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LSOCK_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\LSOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Dgram.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
>>>>>>> 4.8
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
DEP_CPP_LSOCK_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LSOCK_C=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\LSOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\LSOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_LSOCK_CO=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
=======
	{$(INCLUDE)}"\.\LSOCK_CODgram.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	
<<<<<<< ace.mak
NODEP_CPP_LSOCK_CO=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LSOCK_A=\
	".\..\STL\mutex.h"\
=======

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_LSOCK_CO=\
	{$(INCLUDE)}"\.\LSOCK_CODgram.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\LSOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
>>>>>>> 4.8
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LSOCK_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
>>>>>>> 4.8
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\config.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\OS.h"\
	".\OS.i"\
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LOG_R=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
DEP_CPP_LOG_M=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LOG_M=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\FIFO.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LOCAL=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_LOCAL_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.h"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_IPC_S=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_IO_SA=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

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
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_INET_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_HIGH_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\High_Res_Timer.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_HANDL=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_GET_O=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FILE_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
DEP_CPP_FILE_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FILE_C=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FILE_Connector.h"\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\FILE_Connector.i"\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FILE_A=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
DEP_CPP_FILE_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FILE_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FIFO_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.i"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FIFO_S=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FIFO_R=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.i"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FIFO_RE=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
DEP_CPP_EVENT=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_EVENT=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
DEP_CPP_DYNAM=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DYNAM=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Dynamic.h"\
	{$(INCLUDE)}"\.\Dynamic.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
DEP_CPP_DUMP_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DUMP_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Dump.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Dump_T.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Dump_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DEV_I=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
DEP_CPP_DEV_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DEV_C=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DEV_A=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
DEP_CPP_DEV_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DEV_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
DEP_CPP_DATE_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DATE_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Date_Time.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Date_Time.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_CORBA=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\CORBA_Handler.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.i"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
>>>>>>> 4.8
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_ARGV_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_ADDR_=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
DEP_CPP_ACE_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_ACE_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_SOCK_CP=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
>>>>>>> 4.8
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
DEP_CPP_FIFO_C=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_FIFO_C=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
>>>>>>> 4.8
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
DEP_CPP_PROAC=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PROAC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp
DEP_CPP_REACTO=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_REACTO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TOKEN_I=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Token_Invariants.h"\
	{$(INCLUDE)}"\.\Token_Invariants.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
>>>>>>> 4.8
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
DEP_CPP_PROCES=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PROCES=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Process.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\Process.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	".\TTY_IO.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TTY_I=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\TTY_IO.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
>>>>>>> 4.8
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
DEP_CPP_ACTIV=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\IO_Cntl_Msg.h"\
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
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Method_Object.h"\
	".\OS.h"\
	".\OS.i"\
	".\Set.cpp"\
	".\Set.h"\
	".\Set.i"\
	".\Signal.i"\
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
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_ACTIV=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Activation_Queue.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
DEP_CPP_METHO=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_METHO=\
	".\..\STL\mutex.h"\
=======
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIS=\
	".\..\STL\bstring.h"\
<<<<<<< ace.mak
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
	".\Time_Value.h"\
	".\Time_Value.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
=======
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
>>>>>>> 4.8
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

=======
>>>>>>> 4.8
DEP_CPP_TASK_=\
<<<<<<< ace.mak
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TASK_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
=======
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
>>>>>>> 4.8
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


<<<<<<< ace.mak
!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TASK_=\
	".\..\STL\bstring.h"\
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
	".\Set.h"\
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
	".\Stack.cpp"\
	".\Stack.h"\
	".\Stack.i"\
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
	".\Time_Value.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_TASK_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 
=======
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
DEP_CPP_STRAT=\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Strategies.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Strategies_T.h"\
	{$(INCLUDE)}"\.\Strategies_T.cpp"\
	

"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"

>>>>>>> 4.8

# End Source File
# End Target
# End Project
################################################################################
