# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
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

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\STL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/ace.pdb" /machine:I386 /out:"$(OUTDIR)/ace.dll"\
 /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
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

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /Zi /Od /I "..\STL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /Zi /Od /I "..\STL" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ace.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib advapi32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/ace.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ace.dll" /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
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
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
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
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
DEP_CPP_TRACE=\
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
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
DEP_CPP_TOKEN=\
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
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
DEP_CPP_TOKEN_=\
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
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
DEP_CPP_TOKEN_C=\
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
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
DEP_CPP_TOKEN_CP=\
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
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
DEP_CPP_TLI_S=\
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
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
DEP_CPP_TLI_C=\
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
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
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
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
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
	{$(INCLUDE)}"\.\stdcpp.h"\
	{$(INCLUDE)}"\.\ws2tcpip.h"\
	{$(INCLUDE)}"\.\Trace.h"\
	{$(INCLUDE)}"\.\Log_Msg.h"\
	{$(INCLUDE)}"\.\OS.i"\
	{$(INCLUDE)}"\.\config.h"\
	{$(INCLUDE)}"\.\Log_Record.h"\
	{$(INCLUDE)}"\.\Log_Priority.h"\
	{$(INCLUDE)}"\.\Log_Record.i"\
	

"$(INTDIR)\Time_Value.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_R=\
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
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
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
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
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
	

"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

DEP_CPP_SOCK_CO=\
	{$(INCLUDE)}"\.\SOCK_CODgram.h"\
	

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
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
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
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


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
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) $(DEP_CPP_SERVICE_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

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
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
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
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
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
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
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
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
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
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
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
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
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
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
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
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
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
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
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
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
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
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
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
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
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
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
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
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
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
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
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
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ace - Win32 Release"

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
	{$(INCLUDE)}"\.\Addr.i"\
	

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
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
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
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
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
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
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
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
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
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
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
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
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
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
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
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
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
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
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
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
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
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
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
	

"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
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
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
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
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
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
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
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
	

"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
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
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
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
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
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
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
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
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
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
	

"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
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
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp
DEP_CPP_REACTO=\
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
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
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
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
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
	

"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
DEP_CPP_METHO=\
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
	
NODEP_CPP_REGIS=\
	".\..\STL\mutex.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
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
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


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


# End Source File
# End Target
# End Project
################################################################################
