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
	-@erase ".\ace.dll"
	-@erase ".\Release\DEV_IO.obj"
	-@erase ".\Release\SOCK_Acceptor.obj"
	-@erase ".\Release\Profile_Timer.obj"
	-@erase ".\Release\Read_Buffer.obj"
	-@erase ".\Release\Time_Request_Reply.obj"
	-@erase ".\Release\LSOCK_Acceptor.obj"
	-@erase ".\Release\Time_Value.obj"
	-@erase ".\Release\LSOCK_Dgram.obj"
	-@erase ".\Release\Process.obj"
	-@erase ".\Release\ReactorEx.obj"
	-@erase ".\Release\Handle_Set.obj"
	-@erase ".\Release\FILE_IO.obj"
	-@erase ".\Release\Token_Collection.obj"
	-@erase ".\Release\LSOCK_CODgram.obj"
	-@erase ".\Release\SOCK.obj"
	-@erase ".\Release\LSOCK.obj"
	-@erase ".\Release\SOCK_Dgram_Bcast.obj"
	-@erase ".\Release\SPIPE_Connector.obj"
	-@erase ".\Release\FILE_Addr.obj"
	-@erase ".\Release\SV_Message.obj"
	-@erase ".\Release\TLI.obj"
	-@erase ".\Release\Addr.obj"
	-@erase ".\Release\Process_Manager.obj"
	-@erase ".\Release\Service_Main.obj"
	-@erase ".\Release\SPIPE.obj"
	-@erase ".\Release\TLI_Connector.obj"
	-@erase ".\Release\Get_Opt.obj"
	-@erase ".\Release\IO_Cntl_Msg.obj"
	-@erase ".\Release\Pipe.obj"
	-@erase ".\Release\UPIPE_Acceptor.obj"
	-@erase ".\Release\Signal.obj"
	-@erase ".\Release\FIFO.obj"
	-@erase ".\Release\LSOCK_Stream.obj"
	-@erase ".\Release\FIFO_Send.obj"
	-@erase ".\Release\SString.obj"
	-@erase ".\Release\Naming_Context.obj"
	-@erase ".\Release\Memory_Pool.obj"
	-@erase ".\Release\Log_Msg.obj"
	-@erase ".\Release\Mem_Map.obj"
	-@erase ".\Release\SV_Semaphore_Complex.obj"
	-@erase ".\Release\FIFO_Recv_Msg.obj"
	-@erase ".\Release\Synch.obj"
	-@erase ".\Release\Thread_Manager.obj"
	-@erase ".\Release\SV_Message_Queue.obj"
	-@erase ".\Release\TLI_Acceptor.obj"
	-@erase ".\Release\Shared_Object.obj"
	-@erase ".\Release\SOCK_Dgram.obj"
	-@erase ".\Release\ACE.obj"
	-@erase ".\Release\Service_Object.obj"
	-@erase ".\Release\Token.obj"
	-@erase ".\Release\SOCK_CODgram.obj"
	-@erase ".\Release\Name_Proxy.obj"
	-@erase ".\Release\Service_Repository.obj"
	-@erase ".\Release\Event_Handler.obj"
	-@erase ".\Release\Date_Time.obj"
	-@erase ".\Release\SPIPE_Addr.obj"
	-@erase ".\Release\Malloc.obj"
	-@erase ".\Release\SOCK_Dgram_Mcast.obj"
	-@erase ".\Release\Token_Invariants.obj"
	-@erase ".\Release\DEV_Connector.obj"
	-@erase ".\Release\FILE_Connector.obj"
	-@erase ".\Release\Log_Record.obj"
	-@erase ".\Release\Multiplexor.obj"
	-@erase ".\Release\Shared_Memory_MM.obj"
	-@erase ".\Release\Message_Block.obj"
	-@erase ".\Release\Proactor.obj"
	-@erase ".\Release\Remote_Tokens.obj"
	-@erase ".\Release\Service_Record.obj"
	-@erase ".\Release\Timer_Queue.obj"
	-@erase ".\Release\SV_Shared_Memory.obj"
	-@erase ".\Release\Remote_Name_Space.obj"
	-@erase ".\Release\UPIPE_Connector.obj"
	-@erase ".\Release\SPIPE_Stream.obj"
	-@erase ".\Release\TLI_Stream.obj"
	-@erase ".\Release\OS.obj"
	-@erase ".\Release\Service_Config.obj"
	-@erase ".\Release\IPC_SAP.obj"
	-@erase ".\Release\Trace.obj"
	-@erase ".\Release\Obstack.obj"
	-@erase ".\Release\High_Res_Timer.obj"
	-@erase ".\Release\Name_Request_Reply.obj"
	-@erase ".\Release\SOCK_Connector.obj"
	-@erase ".\Release\Local_Name_Space.obj"
	-@erase ".\Release\FILE.obj"
	-@erase ".\Release\DEV_Addr.obj"
	-@erase ".\Release\UPIPE_Stream.obj"
	-@erase ".\Release\SOCK_IO.obj"
	-@erase ".\Release\SPIPE_Acceptor.obj"
	-@erase ".\Release\FIFO_Send_Msg.obj"
	-@erase ".\Release\System_Time.obj"
	-@erase ".\Release\INET_Addr.obj"
	-@erase ".\Release\FIFO_Recv.obj"
	-@erase ".\Release\CORBA_Handler.obj"
	-@erase ".\Release\Reactor.obj"
	-@erase ".\Release\DEV.obj"
	-@erase ".\Release\ARGV.obj"
	-@erase ".\Release\Token_Request_Reply.obj"
	-@erase ".\Release\Token_Manager.obj"
	-@erase ".\Release\Name_Space.obj"
	-@erase ".\Release\Thread.obj"
	-@erase ".\Release\Dump.obj"
	-@erase ".\Release\Svc_Conf_l.obj"
	-@erase ".\Release\Shared_Memory_SV.obj"
	-@erase ".\Release\Synch_Options.obj"
	-@erase ".\Release\SOCK_Stream.obj"
	-@erase ".\Release\Service_Manager.obj"
	-@erase ".\Release\IO_SAP.obj"
	-@erase ".\Release\Dynamic.obj"
	-@erase ".\Release\SV_Semaphore_Simple.obj"
	-@erase ".\Release\Local_Tokens.obj"
	-@erase ".\Release\Svc_Conf_y.obj"
	-@erase ".\Release\Parse_Node.obj"
	-@erase ".\Release\UNIX_Addr.obj"
	-@erase ".\Release\LSOCK_Connector.obj"
	-@erase ".\Release\TTY_IO.obj"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /c 
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
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/TTY_IO.obj"

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
	-@erase ".\Debug\Date_Time.obj"
	-@erase ".\Debug\Token_Collection.obj"
	-@erase ".\Debug\LSOCK_Connector.obj"
	-@erase ".\Debug\SOCK_Dgram_Bcast.obj"
	-@erase ".\Debug\LSOCK_Dgram.obj"
	-@erase ".\Debug\SPIPE_Acceptor.obj"
	-@erase ".\Debug\Event_Handler.obj"
	-@erase ".\Debug\Synch_Options.obj"
	-@erase ".\Debug\Process.obj"
	-@erase ".\Debug\FILE_IO.obj"
	-@erase ".\Debug\Trace.obj"
	-@erase ".\Debug\Signal.obj"
	-@erase ".\Debug\TLI_Acceptor.obj"
	-@erase ".\Debug\DEV_Addr.obj"
	-@erase ".\Debug\SOCK_CODgram.obj"
	-@erase ".\Debug\SPIPE_Connector.obj"
	-@erase ".\Debug\ACE.obj"
	-@erase ".\Debug\Remote_Name_Space.obj"
	-@erase ".\Debug\IO_Cntl_Msg.obj"
	-@erase ".\Debug\Process_Manager.obj"
	-@erase ".\Debug\SOCK.obj"
	-@erase ".\Debug\Timer_Queue.obj"
	-@erase ".\Debug\Name_Space.obj"
	-@erase ".\Debug\Get_Opt.obj"
	-@erase ".\Debug\Svc_Conf_l.obj"
	-@erase ".\Debug\Message_Block.obj"
	-@erase ".\Debug\Service_Repository.obj"
	-@erase ".\Debug\Addr.obj"
	-@erase ".\Debug\Remote_Tokens.obj"
	-@erase ".\Debug\Pipe.obj"
	-@erase ".\Debug\SString.obj"
	-@erase ".\Debug\IO_SAP.obj"
	-@erase ".\Debug\FIFO.obj"
	-@erase ".\Debug\Svc_Conf_y.obj"
	-@erase ".\Debug\Log_Msg.obj"
	-@erase ".\Debug\Mem_Map.obj"
	-@erase ".\Debug\SV_Message_Queue.obj"
	-@erase ".\Debug\Token_Request_Reply.obj"
	-@erase ".\Debug\Parse_Node.obj"
	-@erase ".\Debug\SV_Semaphore_Complex.obj"
	-@erase ".\Debug\System_Time.obj"
	-@erase ".\Debug\INET_Addr.obj"
	-@erase ".\Debug\LSOCK_Acceptor.obj"
	-@erase ".\Debug\FIFO_Recv.obj"
	-@erase ".\Debug\Time_Value.obj"
	-@erase ".\Debug\Malloc.obj"
	-@erase ".\Debug\FILE_Connector.obj"
	-@erase ".\Debug\LSOCK_Stream.obj"
	-@erase ".\Debug\Handle_Set.obj"
	-@erase ".\Debug\FIFO_Send_Msg.obj"
	-@erase ".\Debug\CORBA_Handler.obj"
	-@erase ".\Debug\SOCK_Dgram_Mcast.obj"
	-@erase ".\Debug\SV_Message.obj"
	-@erase ".\Debug\SOCK_Stream.obj"
	-@erase ".\Debug\Token_Invariants.obj"
	-@erase ".\Debug\Token_Manager.obj"
	-@erase ".\Debug\SV_Semaphore_Simple.obj"
	-@erase ".\Debug\Shared_Memory_MM.obj"
	-@erase ".\Debug\OS.obj"
	-@erase ".\Debug\UPIPE_Acceptor.obj"
	-@erase ".\Debug\Service_Config.obj"
	-@erase ".\Debug\LSOCK.obj"
	-@erase ".\Debug\Multiplexor.obj"
	-@erase ".\Debug\SV_Shared_Memory.obj"
	-@erase ".\Debug\DEV.obj"
	-@erase ".\Debug\SOCK_Connector.obj"
	-@erase ".\Debug\SPIPE.obj"
	-@erase ".\Debug\UNIX_Addr.obj"
	-@erase ".\Debug\Naming_Context.obj"
	-@erase ".\Debug\Read_Buffer.obj"
	-@erase ".\Debug\Thread_Manager.obj"
	-@erase ".\Debug\ReactorEx.obj"
	-@erase ".\Debug\UPIPE_Connector.obj"
	-@erase ".\Debug\Local_Name_Space.obj"
	-@erase ".\Debug\SOCK_Acceptor.obj"
	-@erase ".\Debug\Profile_Timer.obj"
	-@erase ".\Debug\IPC_SAP.obj"
	-@erase ".\Debug\FILE_Addr.obj"
	-@erase ".\Debug\Service_Object.obj"
	-@erase ".\Debug\Obstack.obj"
	-@erase ".\Debug\DEV_Connector.obj"
	-@erase ".\Debug\SOCK_Dgram.obj"
	-@erase ".\Debug\LSOCK_CODgram.obj"
	-@erase ".\Debug\Local_Tokens.obj"
	-@erase ".\Debug\Name_Proxy.obj"
	-@erase ".\Debug\Thread.obj"
	-@erase ".\Debug\Synch.obj"
	-@erase ".\Debug\SOCK_IO.obj"
	-@erase ".\Debug\TLI_Connector.obj"
	-@erase ".\Debug\SPIPE_Addr.obj"
	-@erase ".\Debug\FILE.obj"
	-@erase ".\Debug\FIFO_Send.obj"
	-@erase ".\Debug\Shared_Memory_SV.obj"
	-@erase ".\Debug\Time_Request_Reply.obj"
	-@erase ".\Debug\SPIPE_Stream.obj"
	-@erase ".\Debug\Token.obj"
	-@erase ".\Debug\Reactor.obj"
	-@erase ".\Debug\Memory_Pool.obj"
	-@erase ".\Debug\Log_Record.obj"
	-@erase ".\Debug\Service_Main.obj"
	-@erase ".\Debug\TLI.obj"
	-@erase ".\Debug\ARGV.obj"
	-@erase ".\Debug\Service_Record.obj"
	-@erase ".\Debug\UPIPE_Stream.obj"
	-@erase ".\Debug\DEV_IO.obj"
	-@erase ".\Debug\Name_Request_Reply.obj"
	-@erase ".\Debug\Dump.obj"
	-@erase ".\Debug\Service_Manager.obj"
	-@erase ".\Debug\FIFO_Recv_Msg.obj"
	-@erase ".\Debug\Proactor.obj"
	-@erase ".\Debug\Dynamic.obj"
	-@erase ".\Debug\TLI_Stream.obj"
	-@erase ".\Debug\High_Res_Timer.obj"
	-@erase ".\Debug\Shared_Object.obj"
	-@erase ".\Debug\TTY_IO.obj"
	-@erase ".\ace.ilk"
	-@erase ".\ace.lib"
	-@erase ".\ace.exp"
	-@erase ".\ace.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ace.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
	"$(INTDIR)/Date_Time.obj" \
	"$(INTDIR)/Token_Collection.obj" \
	"$(INTDIR)/LSOCK_Connector.obj" \
	"$(INTDIR)/SOCK_Dgram_Bcast.obj" \
	"$(INTDIR)/LSOCK_Dgram.obj" \
	"$(INTDIR)/SPIPE_Acceptor.obj" \
	"$(INTDIR)/Event_Handler.obj" \
	"$(INTDIR)/Synch_Options.obj" \
	"$(INTDIR)/Process.obj" \
	"$(INTDIR)/FILE_IO.obj" \
	"$(INTDIR)/Trace.obj" \
	"$(INTDIR)/Signal.obj" \
	"$(INTDIR)/TLI_Acceptor.obj" \
	"$(INTDIR)/DEV_Addr.obj" \
	"$(INTDIR)/SOCK_CODgram.obj" \
	"$(INTDIR)/SPIPE_Connector.obj" \
	"$(INTDIR)/ACE.obj" \
	"$(INTDIR)/Remote_Name_Space.obj" \
	"$(INTDIR)/IO_Cntl_Msg.obj" \
	"$(INTDIR)/Process_Manager.obj" \
	"$(INTDIR)/SOCK.obj" \
	"$(INTDIR)/Timer_Queue.obj" \
	"$(INTDIR)/Name_Space.obj" \
	"$(INTDIR)/Get_Opt.obj" \
	"$(INTDIR)/Svc_Conf_l.obj" \
	"$(INTDIR)/Message_Block.obj" \
	"$(INTDIR)/Service_Repository.obj" \
	"$(INTDIR)/Addr.obj" \
	"$(INTDIR)/Remote_Tokens.obj" \
	"$(INTDIR)/Pipe.obj" \
	"$(INTDIR)/SString.obj" \
	"$(INTDIR)/IO_SAP.obj" \
	"$(INTDIR)/FIFO.obj" \
	"$(INTDIR)/Svc_Conf_y.obj" \
	"$(INTDIR)/Log_Msg.obj" \
	"$(INTDIR)/Mem_Map.obj" \
	"$(INTDIR)/SV_Message_Queue.obj" \
	"$(INTDIR)/Token_Request_Reply.obj" \
	"$(INTDIR)/Parse_Node.obj" \
	"$(INTDIR)/SV_Semaphore_Complex.obj" \
	"$(INTDIR)/System_Time.obj" \
	"$(INTDIR)/INET_Addr.obj" \
	"$(INTDIR)/LSOCK_Acceptor.obj" \
	"$(INTDIR)/FIFO_Recv.obj" \
	"$(INTDIR)/Time_Value.obj" \
	"$(INTDIR)/Malloc.obj" \
	"$(INTDIR)/FILE_Connector.obj" \
	"$(INTDIR)/LSOCK_Stream.obj" \
	"$(INTDIR)/Handle_Set.obj" \
	"$(INTDIR)/FIFO_Send_Msg.obj" \
	"$(INTDIR)/CORBA_Handler.obj" \
	"$(INTDIR)/SOCK_Dgram_Mcast.obj" \
	"$(INTDIR)/SV_Message.obj" \
	"$(INTDIR)/SOCK_Stream.obj" \
	"$(INTDIR)/Token_Invariants.obj" \
	"$(INTDIR)/Token_Manager.obj" \
	"$(INTDIR)/SV_Semaphore_Simple.obj" \
	"$(INTDIR)/Shared_Memory_MM.obj" \
	"$(INTDIR)/OS.obj" \
	"$(INTDIR)/UPIPE_Acceptor.obj" \
	"$(INTDIR)/Service_Config.obj" \
	"$(INTDIR)/LSOCK.obj" \
	"$(INTDIR)/Multiplexor.obj" \
	"$(INTDIR)/SV_Shared_Memory.obj" \
	"$(INTDIR)/DEV.obj" \
	"$(INTDIR)/SOCK_Connector.obj" \
	"$(INTDIR)/SPIPE.obj" \
	"$(INTDIR)/UNIX_Addr.obj" \
	"$(INTDIR)/Naming_Context.obj" \
	"$(INTDIR)/Read_Buffer.obj" \
	"$(INTDIR)/Thread_Manager.obj" \
	"$(INTDIR)/ReactorEx.obj" \
	"$(INTDIR)/UPIPE_Connector.obj" \
	"$(INTDIR)/Local_Name_Space.obj" \
	"$(INTDIR)/SOCK_Acceptor.obj" \
	"$(INTDIR)/Profile_Timer.obj" \
	"$(INTDIR)/IPC_SAP.obj" \
	"$(INTDIR)/FILE_Addr.obj" \
	"$(INTDIR)/Service_Object.obj" \
	"$(INTDIR)/Obstack.obj" \
	"$(INTDIR)/DEV_Connector.obj" \
	"$(INTDIR)/SOCK_Dgram.obj" \
	"$(INTDIR)/LSOCK_CODgram.obj" \
	"$(INTDIR)/Local_Tokens.obj" \
	"$(INTDIR)/Name_Proxy.obj" \
	"$(INTDIR)/Thread.obj" \
	"$(INTDIR)/Synch.obj" \
	"$(INTDIR)/SOCK_IO.obj" \
	"$(INTDIR)/TLI_Connector.obj" \
	"$(INTDIR)/SPIPE_Addr.obj" \
	"$(INTDIR)/FILE.obj" \
	"$(INTDIR)/FIFO_Send.obj" \
	"$(INTDIR)/Shared_Memory_SV.obj" \
	"$(INTDIR)/Time_Request_Reply.obj" \
	"$(INTDIR)/SPIPE_Stream.obj" \
	"$(INTDIR)/Token.obj" \
	"$(INTDIR)/Reactor.obj" \
	"$(INTDIR)/Memory_Pool.obj" \
	"$(INTDIR)/Log_Record.obj" \
	"$(INTDIR)/Service_Main.obj" \
	"$(INTDIR)/TLI.obj" \
	"$(INTDIR)/ARGV.obj" \
	"$(INTDIR)/Service_Record.obj" \
	"$(INTDIR)/UPIPE_Stream.obj" \
	"$(INTDIR)/DEV_IO.obj" \
	"$(INTDIR)/Name_Request_Reply.obj" \
	"$(INTDIR)/Dump.obj" \
	"$(INTDIR)/Service_Manager.obj" \
	"$(INTDIR)/FIFO_Recv_Msg.obj" \
	"$(INTDIR)/Proactor.obj" \
	"$(INTDIR)/Dynamic.obj" \
	"$(INTDIR)/TLI_Stream.obj" \
	"$(INTDIR)/High_Res_Timer.obj" \
	"$(INTDIR)/Shared_Object.obj" \
	"$(INTDIR)/TTY_IO.obj"

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
NODEP_CPP_UPIPE=\
	".\ace\UPIPE_Stream.h"\
	".\ace\UPIPE_Stream.i"\
	

"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
NODEP_CPP_UPIPE_=\
	".\ace\UPIPE_Connector.h"\
	

"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
NODEP_CPP_UPIPE_A=\
	".\ace\UPIPE_Acceptor.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
NODEP_CPP_UNIX_=\
	".\ace\UNIX_Addr.h"\
	".\ace\UNIX_Addr.i"\
	

"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
NODEP_CPP_TRACE=\
	".\ace\config.h"\
	".\ace\ACE.h"\
	".\ace\Log_Msg.h"\
	".\ace\Trace.h"\
	".\ace\Trace.i"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
NODEP_CPP_TOKEN=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Request_Reply.h"\
	".\ace\Token_Request_Reply.i"\
	

"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
NODEP_CPP_TOKEN_=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Manager.h"\
	".\ace\Token_Manager.i"\
	

"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
NODEP_CPP_TOKEN_C=\
	".\ace\Token_Collection.h"\
	".\ace\Log_Msg.h"\
	".\ace\Token_Collection.i"\
	

"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
NODEP_CPP_TOKEN_CP=\
	".\ace\Thread.h"\
	".\ace\Time_Value.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
NODEP_CPP_TLI_S=\
	".\ace\TLI_Stream.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
NODEP_CPP_TLI_C=\
	".\ace\Handle_Set.h"\
	".\ace\TLI_Connector.h"\
	".\ace\Time_Value.h"\
	

"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
NODEP_CPP_TLI_A=\
	".\ace\TLI_Acceptor.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
NODEP_CPP_TLI_CP=\
	".\ace\TLI.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
NODEP_CPP_TIMER=\
	".\ace\Timer_Queue.h"\
	

"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Value.cpp
NODEP_CPP_TIME_=\
	".\ace\ACE.h"\
	".\ace\Time_Value.h"\
	".\ace\Time_Value.i"\
	

"$(INTDIR)\Time_Value.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
NODEP_CPP_TIME_R=\
	".\ace\Log_Msg.h"\
	".\ace\Time_Request_Reply.h"\
	

"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
NODEP_CPP_THREA=\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	

"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
NODEP_CPP_THREAD=\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
NODEP_CPP_SYSTE=\
	".\ace\Time_Value.h"\
	".\ace\System_Time.h"\
	

"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
NODEP_CPP_SYNCH=\
	".\ace\Synch_Options.h"\
	

"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
NODEP_CPP_SYNCH_=\
	".\ace\Thread.h"\
	".\ace\Synch.h"\
	".\ace\Log_Msg.h"\
	".\ace\Time_Value.h"\
	".\ace\Synch.i"\
	

"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
NODEP_CPP_SVC_C=\
	".\ace\ARGV.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Module.h"\
	".\ace\Stream.h"\
	

"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
NODEP_CPP_SVC_CO=\
	".\ace\Svc_Conf.h"\
	".\ace\Svc_Conf_Tokens.h"\
	

"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
NODEP_CPP_SV_SH=\
	".\ace\SV_Shared_Memory.h"\
	".\ace\Log_Msg.h"\
	".\ace\SV_Shared_Memory.i"\
	

"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
NODEP_CPP_SV_SE=\
	".\ace\Log_Msg.h"\
	".\ace\SV_Semaphore_Simple.h"\
	

"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
NODEP_CPP_SV_SEM=\
	".\ace\Log_Msg.h"\
	".\ace\SV_Semaphore_Complex.h"\
	

"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
NODEP_CPP_SV_ME=\
	".\ace\SV_Message_Queue.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
NODEP_CPP_SV_MES=\
	".\ace\SV_Message.h"\
	".\ace\SV_Message.i"\
	

"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
NODEP_CPP_SSTRI=\
	".\ace\Malloc.h"\
	".\ace\Service_Config.h"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	

"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
NODEP_CPP_SPIPE=\
	".\ace\SPIPE_Stream.h"\
	

"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
NODEP_CPP_SPIPE_=\
	".\ace\SPIPE_Connector.h"\
	

"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
NODEP_CPP_SPIPE_A=\
	".\ace\SPIPE_Addr.h"\
	".\ace\SPIPE_Addr.i"\
	

"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
NODEP_CPP_SPIPE_AC=\
	".\ace\SPIPE_Acceptor.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
NODEP_CPP_SPIPE_C=\
	".\ace\SPIPE.h"\
	

"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
NODEP_CPP_SOCK_=\
	".\ace\SOCK_Stream.h"\
	

"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
NODEP_CPP_SOCK_I=\
	".\ace\SOCK_IO.h"\
	

"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
NODEP_CPP_SOCK_D=\
	".\ace\SOCK_Dgram_Mcast.h"\
	".\ace\INET_Addr.h"\
	

"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
NODEP_CPP_SOCK_DG=\
	".\ace\SOCK_Dgram_Bcast.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
NODEP_CPP_SOCK_DGR=\
	".\ace\SOCK_Dgram.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
NODEP_CPP_SOCK_C=\
	".\ace\SOCK_Connector.h"\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	

"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
NODEP_CPP_SOCK_CO=\
	".\ace\SOCK_CODgram.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
NODEP_CPP_SOCK_A=\
	".\ace\SOCK_Acceptor.h"\
	".\ace\Log_Msg.h"\
	".\ace\SOCK_Acceptor.i"\
	

"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
NODEP_CPP_SIGNA=\
	".\ace\Log_Msg.h"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	

"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
NODEP_CPP_SHARE=\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	

"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
NODEP_CPP_SHARED=\
	".\ace\Shared_Memory_SV.h"\
	".\ace\Shared_Memory_SV.i"\
	

"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
NODEP_CPP_SHARED_=\
	".\ace\Shared_Memory_MM.h"\
	".\ace\Shared_Memory_MM.i"\
	

"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
NODEP_CPP_SERVI=\
	".\ace\Service_Repository.h"\
	".\ace\Service_Repository.i"\
	

"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Record.cpp
NODEP_CPP_SERVIC=\
	".\ace\Service_Record.h"\
	".\ace\Service_Record.i"\
	

"$(INTDIR)\Service_Record.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
NODEP_CPP_SERVICE=\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	

"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
NODEP_CPP_SERVICE_=\
	".\ace\Get_Opt.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Manager.i"\
	

"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Main.cpp
NODEP_CPP_SERVICE_M=\
	".\ace\Service_Config.h"\
	

"$(INTDIR)\Service_Main.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp
NODEP_CPP_SERVICE_C=\
	".\ace\Svc_Conf.h"\
	".\ace\Get_Opt.h"\
	".\ace\ARGV.h"\
	".\ace\Malloc.h"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Record.h"\
	".\ace\Set.h"\
	".\ace\Auto_Ptr.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	

"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
NODEP_CPP_REMOT=\
	".\ace\Log_Msg.h"\
	".\ace\Remote_Tokens.h"\
	".\ace\Singleton.h"\
	".\ace\Remote_Tokens.i"\
	

"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
NODEP_CPP_REMOTE=\
	".\ace\Log_Msg.h"\
	".\ace\Remote_Name_Space.h"\
	

"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
NODEP_CPP_READ_=\
	".\ace\Read_Buffer.h"\
	".\ace\Service_Config.h"\
	

"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
NODEP_CPP_REACT=\
	".\ace\Log_Msg.h"\
	".\ace\Synch_T.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Reactor.h"\
	

"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
NODEP_CPP_PROFI=\
	".\ace\Profile_Timer.h"\
	".\ace\Profile_Timer.i"\
	

"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
NODEP_CPP_PROCE=\
	".\ace\Process_Manager.h"\
	".\ace\Process_Manager.i"\
	

"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
NODEP_CPP_PIPE_=\
	".\ace\Log_Msg.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Pipe.h"\
	

"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
NODEP_CPP_PARSE=\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\ace\Task.h"\
	".\ace\Parse_Node.h"\
	".\ace\Parse_Node.i"\
	

"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
NODEP_CPP_OS_CP=\
	".\ace\OS.h"\
	".\ace\Log_Msg.h"\
	".\ace\ARGV.h"\
	".\ace\OS.i"\
	".\ace\Synch.h"\
	".\ace\Set.h"\
	

"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
NODEP_CPP_OBSTA=\
	".\ace\Obstack.h"\
	

"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
NODEP_CPP_NAMIN=\
	".\ace\Log_Msg.h"\
	".\ace\Get_Opt.h"\
	".\ace\Naming_Context.h"\
	".\ace\Remote_Name_Space.h"\
	".\ace\Local_Name_Space.h"\
	

"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
NODEP_CPP_NAME_=\
	".\ace\Name_Space.h"\
	

"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
NODEP_CPP_NAME_R=\
	".\ace\Log_Msg.h"\
	".\ace\Name_Request_Reply.h"\
	

"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
NODEP_CPP_NAME_P=\
	".\ace\Log_Msg.h"\
	".\ace\Name_Proxy.h"\
	

"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
NODEP_CPP_MULTI=\
	".\ace\Multiplexor.h"\
	".\ace\Multiplexor.i"\
	

"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
NODEP_CPP_MESSA=\
	".\ace\Log_Msg.h"\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	

"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
NODEP_CPP_MEMOR=\
	".\ace\Log_Msg.h"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	

"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
NODEP_CPP_MEM_M=\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	

"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
NODEP_CPP_MALLO=\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	

"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
NODEP_CPP_LSOCK=\
	".\ace\Log_Msg.h"\
	".\ace\LSOCK_Stream.h"\
	

"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
NODEP_CPP_LSOCK_=\
	".\ace\LSOCK_Dgram.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
NODEP_CPP_LSOCK_C=\
	".\ace\LSOCK_Connector.h"\
	

"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
NODEP_CPP_LSOCK_CO=\
	".\ace\LSOCK_CODgram.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
NODEP_CPP_LSOCK_A=\
	".\ace\Log_Msg.h"\
	".\ace\LSOCK_Acceptor.h"\
	".\ace\LSOCK_Acceptor.i"\
	

"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
NODEP_CPP_LSOCK_CP=\
	".\ace\Log_Msg.h"\
	".\ace\LSOCK.h"\
	

"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
NODEP_CPP_LOG_R=\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	

"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
NODEP_CPP_LOG_M=\
	".\ace\config.h"\
	".\ace\ACE.h"\
	".\ace\Thread.h"\
	".\ace\Synch.h"\
	".\ace\Log_Msg.h"\
	".\ace\SPIPE_Connector.h"\
	".\ace\FIFO_Send_Msg.h"\
	

"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
NODEP_CPP_LOCAL=\
	".\ace\Log_Msg.h"\
	".\ace\Thread.h"\
	".\ace\Local_Tokens.h"\
	".\ace\Token_Manager.h"\
	".\ace\Local_Tokens.i"\
	

"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
NODEP_CPP_LOCAL_=\
	".\ace\Log_Msg.h"\
	".\ace\ACE.h"\
	".\ace\Local_Name_Space.h"\
	

"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
NODEP_CPP_IPC_S=\
	".\ace\Log_Msg.h"\
	".\ace\IPC_SAP.h"\
	

"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
NODEP_CPP_IO_SA=\
	".\ace\Log_Msg.h"\
	".\ace\IO_SAP.h"\
	

"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INET_Addr.cpp
NODEP_CPP_INET_=\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	

"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
NODEP_CPP_HIGH_=\
	".\ace\Log_Msg.h"\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	

"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
NODEP_CPP_HANDL=\
	".\ace\Log_Msg.h"\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	

"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
NODEP_CPP_GET_O=\
	".\ace\Log_Msg.h"\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	

"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
NODEP_CPP_FILE_=\
	".\ace\FILE_IO.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
NODEP_CPP_FILE_C=\
	".\ace\FILE_Connector.h"\
	

"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
NODEP_CPP_FILE_A=\
	".\ace\Log_Msg.h"\
	".\ace\FILE_Addr.h"\
	".\ace\FILE_Addr.i"\
	

"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
NODEP_CPP_FILE_CP=\
	".\ace\FILE.h"\
	

"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
NODEP_CPP_FIFO_=\
	".\ace\FIFO_Send_Msg.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
NODEP_CPP_FIFO_S=\
	".\ace\FIFO_Send.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
NODEP_CPP_FIFO_R=\
	".\ace\FIFO_Recv_Msg.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
NODEP_CPP_FIFO_RE=\
	".\ace\FIFO_Recv.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
NODEP_CPP_EVENT=\
	".\ace\Event_Handler.h"\
	".\ace\Message_Block.h"\
	".\ace\Event_Handler.i"\
	

"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
NODEP_CPP_DYNAM=\
	".\ace\Dynamic.h"\
	".\ace\Dynamic.i"\
	

"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
NODEP_CPP_DUMP_=\
	".\ace\Dump.h"\
	

"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
NODEP_CPP_DEV_I=\
	".\ace\DEV_IO.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
NODEP_CPP_DEV_C=\
	".\ace\DEV_Connector.h"\
	

"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
NODEP_CPP_DEV_A=\
	".\ace\Log_Msg.h"\
	".\ace\DEV_Addr.h"\
	".\ace\DEV_Addr.i"\
	

"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
NODEP_CPP_DEV_CP=\
	".\ace\DEV.h"\
	

"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
NODEP_CPP_DATE_=\
	".\ace\Date_Time.h"\
	

"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
NODEP_CPP_CORBA=\
	".\ace\Log_Msg.h"\
	".\ace\CORBA_Handler.h"\
	".\ace\CORBA_Handler.i"\
	

"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
NODEP_CPP_ARGV_=\
	".\ace\Log_Msg.h"\
	".\ace\ARGV.h"\
	".\ace\ARGV.i"\
	

"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
NODEP_CPP_ADDR_=\
	".\ace\Log_Msg.h"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	

"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
NODEP_CPP_ACE_C=\
	".\ace\IPC_SAP.h"\
	".\ace\Time_Value.h"\
	".\ace\Handle_Set.h"\
	".\ace\ACE.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Reactor.h"\
	

"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
NODEP_CPP_SOCK_CP=\
	".\ace\SOCK.h"\
	".\ace\Log_Msg.h"\
	

"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
NODEP_CPP_FIFO_C=\
	".\ace\FIFO.h"\
	".\ace\FIFO.i"\
	

"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
NODEP_CPP_PROAC=\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	

"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReactorEx.cpp
NODEP_CPP_REACTO=\
	".\ace\ReactorEx.h"\
	".\ace\ReactorEx.i"\
	

"$(INTDIR)\ReactorEx.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
NODEP_CPP_TOKEN_I=\
	".\ace\Log_Msg.h"\
	".\ace\Token_Invariants.h"\
	".\ace\Token_Invariants.i"\
	

"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
NODEP_CPP_PROCES=\
	".\ace\Process.h"\
	".\ace\Process.i"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
NODEP_CPP_TTY_I=\
	".\ace\TTY_IO.h"\
	

"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
