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
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

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
	-@erase ".\Release\SPIPE_Stream.obj"
	-@erase ".\Release\INET_Addr.obj"
	-@erase ".\Release\LSOCK_Acceptor.obj"
	-@erase ".\Release\Read_Buffer.obj"
	-@erase ".\Release\SPIPE_Addr.obj"
	-@erase ".\Release\Process.obj"
	-@erase ".\Release\Synch.obj"
	-@erase ".\Release\LSOCK_Dgram.obj"
	-@erase ".\Release\FILE_Connector.obj"
	-@erase ".\Release\FILE_IO.obj"
	-@erase ".\Release\Message_Block.obj"
	-@erase ".\Release\Service_Manager.obj"
	-@erase ".\Release\Log_Record.obj"
	-@erase ".\Release\SV_Message_Queue.obj"
	-@erase ".\Release\Token.obj"
	-@erase ".\Release\UPIPE_Stream.obj"
	-@erase ".\Release\ACE.obj"
	-@erase ".\Release\Date_Time.obj"
	-@erase ".\Release\Remote_Tokens.obj"
	-@erase ".\Release\Dump.obj"
	-@erase ".\Release\IO_Cntl_Msg.obj"
	-@erase ".\Release\LSOCK_CODgram.obj"
	-@erase ".\Release\Service_Config.obj"
	-@erase ".\Release\TLI_Stream.obj"
	-@erase ".\Release\Obstack.obj"
	-@erase ".\Release\SString.obj"
	-@erase ".\Release\SOCK_Dgram_Mcast.obj"
	-@erase ".\Release\SOCK_Connector.obj"
	-@erase ".\Release\Registry.obj"
	-@erase ".\Release\Token_Invariants.obj"
	-@erase ".\Release\UNIX_Addr.obj"
	-@erase ".\Release\Naming_Context.obj"
	-@erase ".\Release\Shared_Memory_MM.obj"
	-@erase ".\Release\Memory_Pool.obj"
	-@erase ".\Release\SOCK_IO.obj"
	-@erase ".\Release\FIFO_Send_Msg.obj"
	-@erase ".\Release\Thread_Manager.obj"
	-@erase ".\Release\ReactorEx.obj"
	-@erase ".\Release\Thread.obj"
	-@erase ".\Release\Process_Manager.obj"
	-@erase ".\Release\CORBA_Handler.obj"
	-@erase ".\Release\SV_Shared_Memory.obj"
	-@erase ".\Release\Local_Tokens.obj"
	-@erase ".\Release\Service_Repository.obj"
	-@erase ".\Release\Service_Object.obj"
	-@erase ".\Release\SOCK.obj"
	-@erase ".\Release\Trace.obj"
	-@erase ".\Release\Malloc.obj"
	-@erase ".\Release\Method_Object.obj"
	-@erase ".\Release\Name_Space.obj"
	-@erase ".\Release\Activation_Queue.obj"
	-@erase ".\Release\Proactor.obj"
	-@erase ".\Release\Addr.obj"
	-@erase ".\Release\Svc_Conf_l.obj"
	-@erase ".\Release\Remote_Name_Space.obj"
	-@erase ".\Release\Local_Name_Space.obj"
	-@erase ".\Release\Pipe.obj"
	-@erase ".\Release\FIFO_Recv.obj"
	-@erase ".\Release\DEV_IO.obj"
	-@erase ".\Release\FIFO.obj"
	-@erase ".\Release\Service_Main.obj"
	-@erase ".\Release\Multiplexor.obj"
	-@erase ".\Release\DEV.obj"
	-@erase ".\Release\Token_Request_Reply.obj"
	-@erase ".\Release\Svc_Conf_y.obj"
	-@erase ".\Release\Parse_Node.obj"
	-@erase ".\Release\SOCK_Acceptor.obj"
	-@erase ".\Release\FILE.obj"
	-@erase ".\Release\Profile_Timer.obj"
	-@erase ".\Release\Service_Record.obj"
	-@erase ".\Release\LSOCK_Stream.obj"
	-@erase ".\Release\DEV_Addr.obj"
	-@erase ".\Release\Shared_Memory_SV.obj"
	-@erase ".\Release\Timer_Queue.obj"
	-@erase ".\Release\DEV_Connector.obj"
	-@erase ".\Release\Time_Value.obj"
	-@erase ".\Release\Name_Request_Reply.obj"
	-@erase ".\Release\Handle_Set.obj"
	-@erase ".\Release\IPC_SAP.obj"
	-@erase ".\Release\ARGV.obj"
	-@erase ".\Release\Signal.obj"
	-@erase ".\Release\High_Res_Timer.obj"
	-@erase ".\Release\TLI_Connector.obj"
	-@erase ".\Release\Get_Opt.obj"
	-@erase ".\Release\SV_Message.obj"
	-@erase ".\Release\SV_Semaphore_Simple.obj"
	-@erase ".\Release\UPIPE_Acceptor.obj"
	-@erase ".\Release\SPIPE_Acceptor.obj"
	-@erase ".\Release\TLI_Acceptor.obj"
	-@erase ".\Release\LSOCK_Connector.obj"
	-@erase ".\Release\System_Time.obj"
	-@erase ".\Release\SOCK_CODgram.obj"
	-@erase ".\Release\Reactor.obj"
	-@erase ".\Release\FILE_Addr.obj"
	-@erase ".\Release\LSOCK.obj"
	-@erase ".\Release\Token_Collection.obj"
	-@erase ".\Release\UPIPE_Connector.obj"
	-@erase ".\Release\SOCK_Dgram_Bcast.obj"
	-@erase ".\Release\Log_Msg.obj"
	-@erase ".\Release\Mem_Map.obj"
	-@erase ".\Release\IO_SAP.obj"
	-@erase ".\Release\FIFO_Recv_Msg.obj"
	-@erase ".\Release\SPIPE_Connector.obj"
	-@erase ".\Release\SPIPE.obj"
	-@erase ".\Release\TLI.obj"
	-@erase ".\Release\SOCK_Stream.obj"
	-@erase ".\Release\Dynamic.obj"
	-@erase ".\Release\SV_Semaphore_Complex.obj"
	-@erase ".\Release\FIFO_Send.obj"
	-@erase ".\Release\Shared_Object.obj"
	-@erase ".\Release\Token_Manager.obj"
	-@erase ".\Release\SOCK_Dgram.obj"
	-@erase ".\Release\Time_Request_Reply.obj"
	-@erase ".\Release\Event_Handler.obj"
	-@erase ".\Release\OS.obj"
	-@erase ".\Release\Synch_Options.obj"
	-@erase ".\Release\Name_Proxy.obj"
	-@erase ".\Release\TTY_IO.obj"
	-@erase ".\Release\Task.obj"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"
>>>>>>> 4.3

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D\
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
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/Method_Object.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/Activation_Queue.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/TTY_IO.obj" \
	"$(INTDIR)/Task.obj"
>>>>>>> 4.3

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
	-@erase ".\Debug\OS.obj"
	-@erase ".\Debug\Service_Manager.obj"
	-@erase ".\Debug\Log_Msg.obj"
	-@erase ".\Debug\Mem_Map.obj"
	-@erase ".\Debug\DEV_Addr.obj"
	-@erase ".\Debug\Thread.obj"
	-@erase ".\Debug\UPIPE_Stream.obj"
	-@erase ".\Debug\Name_Proxy.obj"
	-@erase ".\Debug\Event_Handler.obj"
	-@erase ".\Debug\Service_Object.obj"
	-@erase ".\Debug\Synch_Options.obj"
	-@erase ".\Debug\SPIPE_Addr.obj"
	-@erase ".\Debug\LSOCK_Connector.obj"
	-@erase ".\Debug\Token_Request_Reply.obj"
	-@erase ".\Debug\Token.obj"
	-@erase ".\Debug\FILE.obj"
	-@erase ".\Debug\SOCK_Stream.obj"
	-@erase ".\Debug\Log_Record.obj"
	-@erase ".\Debug\Message_Block.obj"
	-@erase ".\Debug\Parse_Node.obj"
	-@erase ".\Debug\UPIPE_Connector.obj"
	-@erase ".\Debug\DEV_IO.obj"
	-@erase ".\Debug\ARGV.obj"
	-@erase ".\Debug\Remote_Tokens.obj"
	-@erase ".\Debug\SPIPE_Connector.obj"
	-@erase ".\Debug\Dump.obj"
	-@erase ".\Debug\TLI_Stream.obj"
	-@erase ".\Debug\LSOCK.obj"
	-@erase ".\Debug\ACE.obj"
	-@erase ".\Debug\Local_Tokens.obj"
	-@erase ".\Debug\Service_Record.obj"
	-@erase ".\Debug\SV_Semaphore_Simple.obj"
	-@erase ".\Debug\LSOCK_Dgram.obj"
	-@erase ".\Debug\FIFO_Recv.obj"
	-@erase ".\Debug\High_Res_Timer.obj"
	-@erase ".\Debug\SPIPE_Stream.obj"
	-@erase ".\Debug\IO_Cntl_Msg.obj"
	-@erase ".\Debug\Signal.obj"
	-@erase ".\Debug\FIFO_Send_Msg.obj"
	-@erase ".\Debug\IPC_SAP.obj"
	-@erase ".\Debug\Obstack.obj"
	-@erase ".\Debug\SOCK_Dgram_Mcast.obj"
	-@erase ".\Debug\UPIPE_Acceptor.obj"
	-@erase ".\Debug\Token_Invariants.obj"
	-@erase ".\Debug\CORBA_Handler.obj"
	-@erase ".\Debug\SPIPE_Acceptor.obj"
	-@erase ".\Debug\Service_Main.obj"
	-@erase ".\Debug\Token_Manager.obj"
	-@erase ".\Debug\Shared_Memory_MM.obj"
	-@erase ".\Debug\Name_Space.obj"
	-@erase ".\Debug\Svc_Conf_l.obj"
	-@erase ".\Debug\SOCK.obj"
	-@erase ".\Debug\SOCK_IO.obj"
	-@erase ".\Debug\Synch.obj"
	-@erase ".\Debug\LSOCK_Stream.obj"
	-@erase ".\Debug\Memory_Pool.obj"
	-@erase ".\Debug\SV_Shared_Memory.obj"
	-@erase ".\Debug\Addr.obj"
	-@erase ".\Debug\Reactor.obj"
	-@erase ".\Debug\IO_SAP.obj"
	-@erase ".\Debug\UNIX_Addr.obj"
	-@erase ".\Debug\System_Time.obj"
	-@erase ".\Debug\Activation_Queue.obj"
	-@erase ".\Debug\FIFO.obj"
	-@erase ".\Debug\ReactorEx.obj"
	-@erase ".\Debug\Svc_Conf_y.obj"
	-@erase ".\Debug\Local_Name_Space.obj"
	-@erase ".\Debug\Name_Request_Reply.obj"
	-@erase ".\Debug\SOCK_Acceptor.obj"
	-@erase ".\Debug\Profile_Timer.obj"
	-@erase ".\Debug\Malloc.obj"
	-@erase ".\Debug\FILE_Addr.obj"
	-@erase ".\Debug\Dynamic.obj"
	-@erase ".\Debug\TLI_Acceptor.obj"
	-@erase ".\Debug\Time_Value.obj"
	-@erase ".\Debug\Registry.obj"
	-@erase ".\Debug\Process_Manager.obj"
	-@erase ".\Debug\DEV_Connector.obj"
	-@erase ".\Debug\SOCK_CODgram.obj"
	-@erase ".\Debug\DEV.obj"
	-@erase ".\Debug\Handle_Set.obj"
	-@erase ".\Debug\LSOCK_CODgram.obj"
	-@erase ".\Debug\TTY_IO.obj"
	-@erase ".\Debug\LSOCK_Acceptor.obj"
	-@erase ".\Debug\SV_Message.obj"
	-@erase ".\Debug\FILE_Connector.obj"
	-@erase ".\Debug\Multiplexor.obj"
	-@erase ".\Debug\TLI_Connector.obj"
	-@erase ".\Debug\FIFO_Send.obj"
	-@erase ".\Debug\Shared_Memory_SV.obj"
	-@erase ".\Debug\SV_Semaphore_Complex.obj"
	-@erase ".\Debug\Process.obj"
	-@erase ".\Debug\FILE_IO.obj"
	-@erase ".\Debug\Read_Buffer.obj"
	-@erase ".\Debug\Timer_Queue.obj"
	-@erase ".\Debug\SV_Message_Queue.obj"
	-@erase ".\Debug\Trace.obj"
	-@erase ".\Debug\Proactor.obj"
	-@erase ".\Debug\SPIPE.obj"
	-@erase ".\Debug\Remote_Name_Space.obj"
	-@erase ".\Debug\INET_Addr.obj"
	-@erase ".\Debug\Service_Config.obj"
	-@erase ".\Debug\Get_Opt.obj"
	-@erase ".\Debug\Service_Repository.obj"
	-@erase ".\Debug\FIFO_Recv_Msg.obj"
	-@erase ".\Debug\Method_Object.obj"
	-@erase ".\Debug\SOCK_Connector.obj"
	-@erase ".\Debug\Naming_Context.obj"
	-@erase ".\Debug\SString.obj"
	-@erase ".\Debug\Time_Request_Reply.obj"
	-@erase ".\Debug\Token_Collection.obj"
	-@erase ".\Debug\Date_Time.obj"
	-@erase ".\Debug\Shared_Object.obj"
	-@erase ".\Debug\SOCK_Dgram_Bcast.obj"
	-@erase ".\Debug\Thread_Manager.obj"
	-@erase ".\Debug\Pipe.obj"
	-@erase ".\Debug\SOCK_Dgram.obj"
	-@erase ".\Debug\TLI.obj"
	-@erase ".\Debug\Task.obj"
	-@erase ".\ace.ilk"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"
	-@erase ".\ace.pdb"
>>>>>>> 4.3

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
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/Activation_Queue.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/Registry.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/TTY_IO.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/Method_Object.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/Task.obj"
>>>>>>> 4.3

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

DEP_CPP_UPIPE=\
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
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
NODEP_CPP_UPIPE=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

=======

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE=\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_UPIPE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\config.h"\
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
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
>>>>>>> 4.3
NODEP_CPP_UPIPE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_=\
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
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
NODEP_CPP_UPIPE_=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

=======

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_=\
	{$(INCLUDE)}"\.\UPIPE_Connector.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\UPIPE_Connector.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_UPIPE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\config.h"\
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
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	
>>>>>>> 4.3
NODEP_CPP_UPIPE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
<<<<<<< ace.mak

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
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
NODEP_CPP_UPIPE_A=\
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

=======

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\UPIPE_Stream.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SPIPE.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_UPIPE_A=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_UPIPE_A=\
	{$(INCLUDE)}"\.\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\config.h"\
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
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\SPIPE_Stream.h"\
	{$(INCLUDE)}"\.\SPIPE_Stream.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
>>>>>>> 4.3
NODEP_CPP_UPIPE_A=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\UPIPE_Acceptor.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_UNIX_=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\UNIX_Addr.h"\
	{$(INCLUDE)}"\ace\UNIX_Addr.i"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_UNIX_=\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
>>>>>>> 4.3
NODEP_CPP_UNIX_=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_UNIX_=\
	".\ace\UNIX_Addr.h"\
	".\ace\UNIX_Addr.i"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TRACE=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Trace.i"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TRACE=\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Trace.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
>>>>>>> 4.3
NODEP_CPP_TRACE=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TRACE=\
	".\ace\ACE.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Trace.h"\
	".\ace\Trace.i"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
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
	{$(INCLUDE)}"\ace\Token_Request_Reply.h"\
	{$(INCLUDE)}"\ace\Token_Request_Reply.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TOKEN=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
>>>>>>> 4.3
NODEP_CPP_TOKEN=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TOKEN=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Request_Reply.h"\
	".\ace\Token_Request_Reply.i"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
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
	{$(INCLUDE)}"\ace\Token_Manager.h"\
	{$(INCLUDE)}"\ace\Token_Manager.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TOKEN_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Token_Manager.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
>>>>>>> 4.3
NODEP_CPP_TOKEN_=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TOKEN_=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Manager.h"\
	".\ace\Token_Manager.i"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_C=\
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
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
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
	{$(INCLUDE)}"\ace\Token_Collection.h"\
	{$(INCLUDE)}"\ace\Token_Collection.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TOKEN_C=\
	{$(INCLUDE)}"\.\Token_Collection.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
>>>>>>> 4.3
NODEP_CPP_TOKEN_C=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\ace\Sync_T.h"\
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TOKEN_C=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Collection.h"\
	".\ace\Token_Collection.i"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TOKEN_CP=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TOKEN_CP=\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\config.h"\
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
	
>>>>>>> 4.3
NODEP_CPP_TOKEN_CP=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TOKEN_CP=\
	".\ace\Thread.h"\
	".\ace\Time_Value.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_S=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\TLI.h"\
	{$(INCLUDE)}"\ace\TLI.i"\
	{$(INCLUDE)}"\ace\TLI_Stream.h"\
	{$(INCLUDE)}"\ace\TLI_Stream.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TLI_S=\
	{$(INCLUDE)}"\.\TLI_Stream.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
>>>>>>> 4.3
NODEP_CPP_TLI_S=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TLI_S=\
	".\ace\Log_Msg.h"\
	".\ace\TLI_Stream.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
<<<<<<< ace.mak

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TLI_C=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\TLI.h"\
	{$(INCLUDE)}"\ace\TLI.i"\
	{$(INCLUDE)}"\ace\TLI_Connector.h"\
	{$(INCLUDE)}"\ace\TLI_Connector.i"\
	{$(INCLUDE)}"\ace\TLI_Stream.h"\
	{$(INCLUDE)}"\ace\TLI_Stream.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\malloc.h"\
	{$(INCLUDE)}"\process.h"\
	{$(INCLUDE)}"\signal.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
=======
DEP_CPP_TLI_C=\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\TLI_Connector.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	
>>>>>>> 4.3
NODEP_CPP_TLI_C=\
<<<<<<< ace.mak
	"c:\ace_wrappers\ace\semLib.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

NODEP_CPP_TLI_C=\
	".\ace\Handle_Set.h"\
	".\ace\Time_Value.h"\
	".\ace\TLI_Connector.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
	{$(INCLUDE)}"\.\TLI_Acceptor.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\TLI_Stream.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_TLI_A=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\TLI_Acceptor.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
	{$(INCLUDE)}"\.\TLI.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\TLI.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_TLI_CP=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\TLI.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
DEP_CPP_TIMER=\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\config.h"\
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
	
NODEP_CPP_TIMER=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Value.cpp
DEP_CPP_TIME_=\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\config.h"\
	
NODEP_CPP_TIME_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Time_Value.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_R=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Time_Request_Reply.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	
NODEP_CPP_TIME_R=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	
NODEP_CPP_THREA=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREAD=\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_THREAD=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
DEP_CPP_SYSTE=\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\System_Time.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	
NODEP_CPP_SYSTE=\
<<<<<<< ace.mak
	".\ace\System_Time.h"\
	".\ace\Time_Value.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
DEP_CPP_SYNCH=\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SYNCH=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	
NODEP_CPP_SYNCH_=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Thread.h"\
	".\ace\Time_Value.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_SVC_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	
NODEP_CPP_SVC_C=\
<<<<<<< ace.mak
	".\ace\ARGV.h"\
	".\ace\Module.h"\
	".\ace\Stream.h"\
	".\ace\Svc_Conf.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

<<<<<<< ace.mak
"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"
=======
"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak

=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
<<<<<<< ace.mak
NODEP_CPP_SVC_CO=\
	".\ace\Svc_Conf.h"\
	".\ace\Svc_Conf_Tokens.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"
=======

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SVC_CO=\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_SVC_CO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SVC_CO=\
	{$(INCLUDE)}"\.\Svc_Conf.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Parse_Node.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Stream.i"\
	{$(INCLUDE)}"\.\Stream.cpp"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	
NODEP_CPP_SVC_CO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak

=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	{$(INCLUDE)}"\.\SV_Shared_Memory.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SV_SH=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SV_Shared_Memory.h"\
	".\ace\SV_Shared_Memory.i"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	
NODEP_CPP_SV_SE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	
NODEP_CPP_SV_SEM=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	{$(INCLUDE)}"\.\SV_Message_Queue.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message_Queue.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SV_ME=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SV_Message_Queue.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	{$(INCLUDE)}"\.\SV_Message.h"\
	{$(INCLUDE)}"\.\SV_Message.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SV_MES=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	
NODEP_CPP_SSTRI=\
	".\ace\Sync_T.h"\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_SPIPE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_SPIPE_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SPIPE_A=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	{$(INCLUDE)}"\.\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SPIPE_AC=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SPIPE_Acceptor.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SPIPE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_SPIPE_C=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	
NODEP_CPP_SOCK_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	
NODEP_CPP_SOCK_I=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	
NODEP_CPP_SOCK_D=\
<<<<<<< ace.mak
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Dgram_Mcast.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram_Bcast.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SOCK_DG=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SOCK_Dgram_Bcast.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	{$(INCLUDE)}"\.\SOCK_Dgram.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Dgram.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SOCK_DGR=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SOCK_Dgram.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	
NODEP_CPP_SOCK_C=\
<<<<<<< ace.mak
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Connector.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SOCK_CO=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SOCK_CODgram.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SOCK_A=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Acceptor.i"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Set.h"\
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
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	
NODEP_CPP_SIGNA=\
	".\..\STL\mutex.h"\
	

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SHARE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Shared_Memory.i"\
	
NODEP_CPP_SHARED=\
	".\..\STL\mutex.h"\
	

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	
NODEP_CPP_SHARED_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

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
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_SERVI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Record.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVIC=\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_SERVIC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVIC=\
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVIC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

<<<<<<< ace.mak
"$(INTDIR)\Service_Record.obj" : $(SOURCE) "$(INTDIR)"

=======
"$(INTDIR)\Service_Record.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak
=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVICE=\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SERVICE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
<<<<<<< ace.mak
NODEP_CPP_SERVICE_=\
	".\ace\Get_Opt.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Manager.i"\
	".\ace\Service_Repository.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"
=======

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SERVICE_=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVICE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_SERVICE_=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVICE_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak

=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp
DEP_CPP_SERVICE_M=\
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
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	
NODEP_CPP_SERVICE_M=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVICE_C=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Parse_Node.i"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
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
	
NODEP_CPP_SERVICE_C=\
<<<<<<< ace.mak
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
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

<<<<<<< ace.mak
"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"

=======
"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak
=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Remote_Tokens.h"\
	{$(INCLUDE)}"\.\Singleton.h"\
	{$(INCLUDE)}"\.\Remote_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Token_Request_Reply.h"\
	{$(INCLUDE)}"\.\Synch.h"\
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
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\Token_Request_Reply.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Singleton.i"\
	{$(INCLUDE)}"\.\Singleton.cpp"\
	
NODEP_CPP_REMOT=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\Remote_Tokens.h"\
	".\ace\Remote_Tokens.i"\
	".\ace\Singleton.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Name_Space.h"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_REMOTE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
DEP_CPP_READ_=\
	{$(INCLUDE)}"\.\Read_Buffer.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Read_Buffer.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	
NODEP_CPP_READ_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	{$(INCLUDE)}"\.\ReactorEx.i"\
	
NODEP_CPP_REACT=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\Reactor.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Synch_T.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
DEP_CPP_PROFI=\
	{$(INCLUDE)}"\.\Profile_Timer.h"\
	{$(INCLUDE)}"\.\Profile_Timer.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	
NODEP_CPP_PROFI=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
	{$(INCLUDE)}"\.\Process_Manager.h"\
	{$(INCLUDE)}"\.\Process_Manager.i"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	
NODEP_CPP_PROCE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	{$(INCLUDE)}"\.\Pipe.i"\
	
NODEP_CPP_PIPE_=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\Pipe.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_PARSE=\
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
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_PARSE=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_PARSE=\
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
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	{$(INCLUDE)}"\.\Service_Record.h"\
	{$(INCLUDE)}"\.\Service_Repository.i"\
	{$(INCLUDE)}"\.\Stream.h"\
	{$(INCLUDE)}"\.\Service_Record.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
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
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	
NODEP_CPP_PARSE=\
<<<<<<< ace.mak
	".\ace\Parse_Node.h"\
	".\ace\Parse_Node.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\ace\Task.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

<<<<<<< ace.mak
"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"
=======
"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"

>>>>>>> 4.3

<<<<<<< ace.mak

=======
!ENDIF 

>>>>>>> 4.3
# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	
NODEP_CPP_OS_CP=\
<<<<<<< ace.mak
	".\ace\ARGV.h"\
	".\ace\Log_Msg.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Set.h"\
	".\ace\Synch.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
DEP_CPP_OBSTA=\
	{$(INCLUDE)}"\.\Obstack.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_OBSTA=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\Remote_Name_Space.h"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_NAMIN=\
<<<<<<< ace.mak
	".\ace\Get_Opt.h"\
	".\ace\Local_Name_Space.h"\
	".\ace\Log_Msg.h"\
	".\ace\Naming_Context.h"\
	".\ace\Remote_Name_Space.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
DEP_CPP_NAME_=\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_NAME_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SString.h"\
	{$(INCLUDE)}"\.\SString.i"\
	
NODEP_CPP_NAME_R=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_NAME_P=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_MULTI=\
	{$(INCLUDE)}"\.\Multiplexor.h"\
	{$(INCLUDE)}"\.\Multiplexor.i"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\Message_Block.h"\
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
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
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
	
NODEP_CPP_MULTI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_MULTI=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	
NODEP_CPP_MULTI=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	
NODEP_CPP_MESSA=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
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
	
NODEP_CPP_MEMOR=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
DEP_CPP_MEM_M=\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_MEM_M=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	
NODEP_CPP_MALLO=\
	".\ace\Sync_T.h"\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
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
	{$(INCLUDE)}"\.\LSOCK.i"\
	
NODEP_CPP_LSOCK=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
	{$(INCLUDE)}"\.\LSOCK_Dgram.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_LSOCK_=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\LSOCK_Dgram.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
DEP_CPP_LSOCK_C=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	
NODEP_CPP_LSOCK_C=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
DEP_CPP_LSOCK_CO=\
	{$(INCLUDE)}"\.\LSOCK_CODgram.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\SOCK_CODgram.i"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_LSOCK_CO=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\LSOCK_CODgram.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\LSOCK_Acceptor.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\.\UNIX_Addr.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.h"\
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
	{$(INCLUDE)}"\.\UNIX_Addr.i"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\LSOCK_Stream.i"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	
NODEP_CPP_LSOCK_A=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\LSOCK.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\LSOCK.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	
NODEP_CPP_LSOCK_CP=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_LOG_R=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
DEP_CPP_LOG_M=\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\SPIPE_Connector.h"\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
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
	
NODEP_CPP_LOG_M=\
<<<<<<< ace.mak
	".\ace\ACE.h"\
	".\ace\config.h"\
	".\ace\FIFO_Send_Msg.h"\
	".\ace\Log_Msg.h"\
	".\ace\SPIPE_Connector.h"\
	".\ace\Synch.h"\
	".\ace\Thread.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Token_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_LOCAL=\
<<<<<<< ace.mak
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Log_Msg.h"\
	".\ace\Thread.h"\
	".\ace\Token_Manager.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Local_Name_Space.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Name_Space.h"\
	{$(INCLUDE)}"\.\Naming_Context.h"\
	{$(INCLUDE)}"\.\Local_Name_Space_T.cpp"\
	{$(INCLUDE)}"\.\Name_Proxy.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.h"\
	{$(INCLUDE)}"\.\Name_Request_Reply.h"\
	{$(INCLUDE)}"\.\SOCK_Connector.i"\
	
NODEP_CPP_LOCAL_=\
<<<<<<< ace.mak
	".\ace\ACE.h"\
	".\ace\Local_Name_Space.h"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	
NODEP_CPP_IPC_S=\
<<<<<<< ace.mak
	".\ace\IPC_SAP.h"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	
NODEP_CPP_IO_SA=\
<<<<<<< ace.mak
	".\ace\IO_SAP.h"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

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
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_INET_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.h"\
	{$(INCLUDE)}"\.\High_Res_Timer.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_HIGH_=\
<<<<<<< ace.mak
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_HANDL=\
<<<<<<< ace.mak
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Get_Opt.h"\
	{$(INCLUDE)}"\.\Get_Opt.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_GET_O=\
<<<<<<< ace.mak
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
	{$(INCLUDE)}"\.\FILE_IO.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FILE_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
DEP_CPP_FILE_C=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_FILE_C=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_FILE_A=\
<<<<<<< ace.mak
	".\ace\FILE_Addr.h"\
	".\ace\FILE_Addr.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
DEP_CPP_FILE_CP=\
	{$(INCLUDE)}"\.\FILE.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\FILE_Addr.h"\
	{$(INCLUDE)}"\.\FILE.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\FILE_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_FILE_CP=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
	{$(INCLUDE)}"\.\FIFO_Send_Msg.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FIFO_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
	{$(INCLUDE)}"\.\FIFO_Send.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Send.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FIFO_S=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
	{$(INCLUDE)}"\.\FIFO_Recv_Msg.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FIFO_R=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
	{$(INCLUDE)}"\.\FIFO_Recv.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO_Recv.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FIFO_RE=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
DEP_CPP_EVENT=\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	
NODEP_CPP_EVENT=\
<<<<<<< ace.mak
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Message_Block.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
DEP_CPP_DYNAM=\
	{$(INCLUDE)}"\.\Dynamic.h"\
	{$(INCLUDE)}"\.\Dynamic.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_DYNAM=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
DEP_CPP_DUMP_=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	
NODEP_CPP_DUMP_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_DEV_I=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
DEP_CPP_DEV_C=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_DEV_C=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_DEV_A=\
<<<<<<< ace.mak
	".\ace\DEV_Addr.h"\
	".\ace\DEV_Addr.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
DEP_CPP_DEV_CP=\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	
NODEP_CPP_DEV_CP=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
DEP_CPP_DATE_=\
	{$(INCLUDE)}"\.\Date_Time.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Date_Time.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_DATE_=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.h"\
	{$(INCLUDE)}"\.\CORBA_Handler.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
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
	
NODEP_CPP_CORBA=\
<<<<<<< ace.mak
	".\ace\CORBA_Handler.h"\
	".\ace\CORBA_Handler.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\ARGV.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_ARGV_=\
<<<<<<< ace.mak
	".\ace\ARGV.h"\
	".\ace\ARGV.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	
NODEP_CPP_ADDR_=\
<<<<<<< ace.mak
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Log_Msg.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
DEP_CPP_ACE_C=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\ReactorEx.i"\
	
NODEP_CPP_ACE_C=\
<<<<<<< ace.mak
	".\ace\ACE.h"\
	".\ace\Handle_Set.h"\
	".\ace\IPC_SAP.h"\
	".\ace\Reactor.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Time_Value.h"\
=======
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
>>>>>>> 4.3
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_SOCK_CP=\
<<<<<<< ace.mak
	".\ace\Log_Msg.h"\
	".\ace\SOCK.h"\
=======
	".\..\STL\mutex.h"\
>>>>>>> 4.3
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
DEP_CPP_FIFO_C=\
	{$(INCLUDE)}"\.\FIFO.h"\
	{$(INCLUDE)}"\.\FIFO.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	
NODEP_CPP_FIFO_C=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
DEP_CPP_PROAC=\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Thread.i"\
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
	
NODEP_CPP_PROAC=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp
DEP_CPP_REACTO=\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Token.i"\
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
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Message_Block.i"\
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
	
NODEP_CPP_REACTO=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Token_Invariants.h"\
	{$(INCLUDE)}"\.\Token_Invariants.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
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
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
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
	
NODEP_CPP_TOKEN_I=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
DEP_CPP_PROCES=\
	{$(INCLUDE)}"\.\Process.h"\
	{$(INCLUDE)}"\.\ARGV.h"\
	{$(INCLUDE)}"\.\Process.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\ARGV.i"\
	
NODEP_CPP_PROCES=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
	{$(INCLUDE)}"\.\TTY_IO.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\DEV_Addr.h"\
	{$(INCLUDE)}"\.\DEV_Connector.h"\
	{$(INCLUDE)}"\.\DEV_IO.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\DEV_Addr.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\DEV_Connector.i"\
	{$(INCLUDE)}"\.\DEV.h"\
	{$(INCLUDE)}"\.\DEV_IO.i"\
	{$(INCLUDE)}"\.\IO_SAP.h"\
	{$(INCLUDE)}"\.\DEV.i"\
	{$(INCLUDE)}"\.\IO_SAP.i"\
	
NODEP_CPP_TTY_I=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
DEP_CPP_ACTIV=\
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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
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
	
NODEP_CPP_ACTIV=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
DEP_CPP_METHO=\
	{$(INCLUDE)}"\.\Method_Object.h"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	
NODEP_CPP_METHO=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Method_Object.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIS=\
	{$(INCLUDE)}"\.\Registry.h"\
	{$(INCLUDE)}"\.\OS.h"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\ACE.i"\
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_TASK_=\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Module.i"\
	{$(INCLUDE)}"\.\Module.cpp"\
	{$(INCLUDE)}"\.\Set.h"\
	{$(INCLUDE)}"\.\Proactor.h"\
	{$(INCLUDE)}"\.\ReactorEx.h"\
	{$(INCLUDE)}"\.\Service_Config.i"\
	{$(INCLUDE)}"\.\Reactor.h"\
	{$(INCLUDE)}"\.\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\Thread.h"\
	{$(INCLUDE)}"\.\Synch.h"\
	{$(INCLUDE)}"\.\Thread_Manager.i"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Synch_T.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Stream_Modules.h"\
	{$(INCLUDE)}"\.\Set.i"\
	{$(INCLUDE)}"\.\Set.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
	{$(INCLUDE)}"\.\Timer_Queue.h"\
	{$(INCLUDE)}"\.\Proactor.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\.\Token.h"\
	{$(INCLUDE)}"\.\Local_Tokens.h"\
	{$(INCLUDE)}"\.\ReactorEx.i"\
	{$(INCLUDE)}"\.\Handle_Set.h"\
	{$(INCLUDE)}"\.\Signal.h"\
	{$(INCLUDE)}"\.\Pipe.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.h"\
	{$(INCLUDE)}"\.\Reactor.i"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
	{$(INCLUDE)}"\.\Thread.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\.\Synch.i"\
	{$(INCLUDE)}"\.\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\.\Message_Queue.i"\
	{$(INCLUDE)}"\.\Message_Queue.cpp"\
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\Stream_Modules.i"\
	{$(INCLUDE)}"\.\Stream_Modules.cpp"\
	{$(INCLUDE)}"\.\Malloc.h"\
	{$(INCLUDE)}"\.\Message_Block.i"\
	{$(INCLUDE)}"\.\Timer_Queue.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\.\Token.i"\
	{$(INCLUDE)}"\.\Stack.h"\
	{$(INCLUDE)}"\.\Synch_Options.h"\
	{$(INCLUDE)}"\.\Map_Manager.h"\
	{$(INCLUDE)}"\.\Local_Tokens.i"\
	{$(INCLUDE)}"\.\Handle_Set.i"\
	{$(INCLUDE)}"\.\Signal.i"\
	{$(INCLUDE)}"\.\Pipe.i"\
	{$(INCLUDE)}"\.\SOCK_IO.h"\
	{$(INCLUDE)}"\.\INET_Addr.h"\
	{$(INCLUDE)}"\.\SOCK_Stream.i"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\.\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Malloc.i"\
	{$(INCLUDE)}"\.\Malloc_T.h"\
	{$(INCLUDE)}"\.\Memory_Pool.h"\
	{$(INCLUDE)}"\.\Stack.i"\
	{$(INCLUDE)}"\.\Stack.cpp"\
	{$(INCLUDE)}"\.\Map_Manager.i"\
	{$(INCLUDE)}"\.\Map_Manager.cpp"\
	{$(INCLUDE)}"\.\SOCK.h"\
	{$(INCLUDE)}"\.\SOCK_IO.i"\
	{$(INCLUDE)}"\.\Addr.h"\
	{$(INCLUDE)}"\.\INET_Addr.i"\
	{$(INCLUDE)}"\.\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\.\Malloc_T.i"\
	{$(INCLUDE)}"\.\Malloc_T.cpp"\
	{$(INCLUDE)}"\.\Mem_Map.h"\
	{$(INCLUDE)}"\.\Memory_Pool.i"\
	{$(INCLUDE)}"\.\IPC_SAP.h"\
	{$(INCLUDE)}"\.\SOCK.i"\
	{$(INCLUDE)}"\.\Addr.i"\
	{$(INCLUDE)}"\.\Mem_Map.i"\
	{$(INCLUDE)}"\.\IPC_SAP.i"\
	
NODEP_CPP_TASK_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

DEP_CPP_TASK_=\
	{$(INCLUDE)}"\.\Task.h"\
	{$(INCLUDE)}"\.\Module.h"\
	{$(INCLUDE)}"\.\Service_Config.h"\
	{$(INCLUDE)}"\.\Task.i"\
	{$(INCLUDE)}"\.\Service_Object.h"\
	{$(INCLUDE)}"\.\Thread_Manager.h"\
	{$(INCLUDE)}"\.\Task_T.h"\
	{$(INCLUDE)}"\.\Shared_Object.h"\
	{$(INCLUDE)}"\.\Event_Handler.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\Service_Object.i"\
	{$(INCLUDE)}"\.\ACE.h"\
	{$(INCLUDE)}"\.\Shared_Object.i"\
	{$(INCLUDE)}"\.\OS.h"\
	{$(INCLUDE)}"\.\ACE.i"\
	{$(INCLUDE)}"\.\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\SIGNAL.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\PROCESS.H"\
	".\..\STL\bstring.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Time_Value.i"\
	{$(INCLUDE)}"\MALLOC.H"\
	{$(INCLUDE)}"\.\Event_Handler.i"\
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
	{$(INCLUDE)}"\.\Synch_T.i"\
	{$(INCLUDE)}"\.\Synch_T.cpp"\
	{$(INCLUDE)}"\.\Message_Queue.h"\
	{$(INCLUDE)}"\.\Task_T.i"\
	{$(INCLUDE)}"\.\Task_T.cpp"\
	{$(INCLUDE)}"\.\Message_Block.h"\
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
	
NODEP_CPP_TASK_=\
	".\..\STL\mutex.h"\
	".\ace\Sync_T.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
