# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=ACE dynamic library - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to ACE dynamic library - Win32\
 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "ACE dynamic library - Win32 Debug" && "$(CFG)" !=\
 "ACE static library - Win32 Release" && "$(CFG)" !=\
 "ACE static library - Win32 Debug" && "$(CFG)" !=\
 "ACE static library - Win32 Unicode Debug" && "$(CFG)" !=\
 "ACE static library - Win32 Unicode Release" && "$(CFG)" !=\
 "ACE dynamic library - Win32 Unicode Debug" && "$(CFG)" !=\
 "ACE dynamic library - Win32 Unicode Release" && "$(CFG)" !=\
 "ACE dynamic library - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ACE_Library.mak"\
 CFG="ACE dynamic library - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE dynamic library - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE static library - Win32 Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Unicode Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Unicode Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "ACE dynamic library - Win32 Unicode Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Unicode Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "ACE dynamic library - Win32 Release"

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE dynamic library\Debug"
# PROP BASE Intermediate_Dir "ACE dynamic library\Debug"
# PROP BASE Target_Dir "ACE dynamic library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Target_Dir "ACE dynamic library"
OUTDIR=.
INTDIR=.\DLL\Debug

ALL : "$(OUTDIR)\aced.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\aced.dll"
	-@erase "$(OUTDIR)\aced.exp"
	-@erase "$(OUTDIR)\aced.ilk"
	-@erase "$(OUTDIR)\aced.lib"
	-@erase "$(OUTDIR)\aced.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\DLL\Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE dynamic library.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"aced.dll"
LINK32_FLAGS=advapi32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/aced.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/aced.dll" /implib:"$(OUTDIR)/aced.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\aced.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE static library\Release"
# PROP BASE Intermediate_Dir "ACE static library\Release"
# PROP BASE Target_Dir "ACE static library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir "ACE static library"
OUTDIR=.
INTDIR=.\LIB\Release

ALL : "$(OUTDIR)\aces.lib"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\aces.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\LIB\Release/
CPP_SBRS=.\.

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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE static library.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"aces.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/aces.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\aces.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE static library\Debug"
# PROP BASE Intermediate_Dir "ACE static library\Debug"
# PROP BASE Target_Dir "ACE static library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir "ACE static library"
OUTDIR=.
INTDIR=.\LIB\Debug

ALL : "$(OUTDIR)\acesd.lib"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\acesd.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MTd /W3 /GX /Z7 /Od /I ".." /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\LIB\Debug/
CPP_SBRS=.\.

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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE static library.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"acesd.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/acesd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\acesd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE static library\Unicode Debug"
# PROP BASE Intermediate_Dir "ACE static library\Unicode Debug"
# PROP BASE Target_Dir "ACE static library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Debug"
# PROP Target_Dir "ACE static library"
OUTDIR=.
INTDIR=.\LIB\Unicode_Debug

ALL : "$(OUTDIR)\acesud.lib"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\acesud.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I ".." /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MTd /W3 /GX /Z7 /Od /I ".." /D "_DEBUG" /D "UNICODE" /D\
 "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\LIB\Unicode_Debug/
CPP_SBRS=.\.

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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE static library.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"acesud.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/acesud.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\acesud.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE static library\Unicode Release"
# PROP BASE Intermediate_Dir "ACE static library\Unicode Release"
# PROP BASE Target_Dir "ACE static library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Release"
# PROP Target_Dir "ACE static library"
OUTDIR=.
INTDIR=.\LIB\Unicode_Release

ALL : "$(OUTDIR)\acesu.lib"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\acesu.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I ".." /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /I ".." /D "NDEBUG" /D "UNICODE" /D\
 "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\LIB\Unicode_Release/
CPP_SBRS=.\.

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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE static library.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"acesu.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/acesu.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\acesu.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE dynamic library\Unicode Debug"
# PROP BASE Intermediate_Dir "ACE dynamic library\Unicode Debug"
# PROP BASE Target_Dir "ACE dynamic library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Unicode_Debug"
# PROP Target_Dir "ACE dynamic library"
OUTDIR=.
INTDIR=.\DLL\Unicode_Debug

ALL : "$(OUTDIR)\aceud.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\aceud.dll"
	-@erase "$(OUTDIR)\aceud.exp"
	-@erase "$(OUTDIR)\aceud.ilk"
	-@erase "$(OUTDIR)\aceud.lib"
	-@erase "$(OUTDIR)\aceud.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "UNICODE"\
 /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\DLL\Unicode_Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE dynamic library.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"aceud.dll"
LINK32_FLAGS=advapi32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/aceud.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/aceud.dll" /implib:"$(OUTDIR)/aceud.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\aceud.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE dynamic library\Unicode Release"
# PROP BASE Intermediate_Dir "ACE dynamic library\Unicode Release"
# PROP BASE Target_Dir "ACE dynamic library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Unicode_Release"
# PROP Target_Dir "ACE dynamic library"
OUTDIR=.
INTDIR=.\DLL\Unicode_Release

ALL : "$(OUTDIR)\aceu.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\aceu.dll"
	-@erase "$(OUTDIR)\aceu.exp"
	-@erase "$(OUTDIR)\aceu.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "UNICODE" /D\
 "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\DLL\Unicode_Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE dynamic library.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"aceu.dll"
LINK32_FLAGS=advapi32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/aceu.pdb" /machine:I386\
 /out:"$(OUTDIR)/aceu.dll" /implib:"$(OUTDIR)/aceu.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\aceu.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE dynamic library\Release"
# PROP BASE Intermediate_Dir "ACE dynamic library\Release"
# PROP BASE Target_Dir "ACE dynamic library"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Target_Dir "ACE dynamic library"
OUTDIR=.
INTDIR=.\DLL\Release

ALL : "$(OUTDIR)\ace.dll"

CLEAN : 
	-@erase "$(INTDIR)\ACE.obj"
	-@erase "$(INTDIR)\Activation_Queue.obj"
	-@erase "$(INTDIR)\Addr.obj"
	-@erase "$(INTDIR)\Arg_Shifter.obj"
	-@erase "$(INTDIR)\ARGV.obj"
	-@erase "$(INTDIR)\Asynch_IO.obj"
	-@erase "$(INTDIR)\Basic_Types.obj"
	-@erase "$(INTDIR)\Containers.obj"
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
	-@erase "$(INTDIR)\Filecache.obj"
	-@erase "$(INTDIR)\Get_Opt.obj"
	-@erase "$(INTDIR)\Handle_Set.obj"
	-@erase "$(INTDIR)\High_Res_Timer.obj"
	-@erase "$(INTDIR)\INET_Addr.obj"
	-@erase "$(INTDIR)\IO_Cntl_Msg.obj"
	-@erase "$(INTDIR)\IO_SAP.obj"
	-@erase "$(INTDIR)\IOStream.obj"
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
	-@erase "$(INTDIR)\Message_Queue.obj"
	-@erase "$(INTDIR)\Method_Request.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
	-@erase "$(INTDIR)\NT_Service.obj"
	-@erase "$(INTDIR)\Object_Manager.obj"
	-@erase "$(INTDIR)\Obstack.obj"
	-@erase "$(INTDIR)\OS.obj"
	-@erase "$(INTDIR)\Parse_Node.obj"
	-@erase "$(INTDIR)\Pipe.obj"
	-@erase "$(INTDIR)\Priority_Reactor.obj"
	-@erase "$(INTDIR)\Proactor.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process_Manager.obj"
	-@erase "$(INTDIR)\Profile_Timer.obj"
	-@erase "$(INTDIR)\Reactor.obj"
	-@erase "$(INTDIR)\Read_Buffer.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\Registry_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Name_Space.obj"
	-@erase "$(INTDIR)\Remote_Tokens.obj"
	-@erase "$(INTDIR)\Sched_Params.obj"
	-@erase "$(INTDIR)\Select_Reactor.obj"
	-@erase "$(INTDIR)\Select_Reactor_Base.obj"
	-@erase "$(INTDIR)\Service_Config.obj"
	-@erase "$(INTDIR)\Service_Manager.obj"
	-@erase "$(INTDIR)\Service_Object.obj"
	-@erase "$(INTDIR)\Service_Repository.obj"
	-@erase "$(INTDIR)\Service_Types.obj"
	-@erase "$(INTDIR)\Shared_Memory.obj"
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
	-@erase "$(INTDIR)\Stats.obj"
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
	-@erase "$(INTDIR)\TP_Reactor.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\TTY_IO.obj"
	-@erase "$(INTDIR)\UNIX_Addr.obj"
	-@erase "$(INTDIR)\UPIPE_Acceptor.obj"
	-@erase "$(INTDIR)\UPIPE_Connector.obj"
	-@erase "$(INTDIR)\UPIPE_Stream.obj"
	-@erase "$(INTDIR)\WFMO_Reactor.obj"
	-@erase "$(OUTDIR)\ace.dll"
	-@erase "$(OUTDIR)\ace.exp"
	-@erase "$(OUTDIR)\ace.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\DLL\Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ACE dynamic library.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ace.dll"
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ace.dll"
LINK32_FLAGS=advapi32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/ace.pdb" /machine:I386 /out:"$(OUTDIR)/ace.dll"\
 /implib:"$(OUTDIR)/ace.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ACE.obj" \
	"$(INTDIR)\Activation_Queue.obj" \
	"$(INTDIR)\Addr.obj" \
	"$(INTDIR)\Arg_Shifter.obj" \
	"$(INTDIR)\ARGV.obj" \
	"$(INTDIR)\Asynch_IO.obj" \
	"$(INTDIR)\Basic_Types.obj" \
	"$(INTDIR)\Containers.obj" \
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
	"$(INTDIR)\Filecache.obj" \
	"$(INTDIR)\Get_Opt.obj" \
	"$(INTDIR)\Handle_Set.obj" \
	"$(INTDIR)\High_Res_Timer.obj" \
	"$(INTDIR)\INET_Addr.obj" \
	"$(INTDIR)\IO_Cntl_Msg.obj" \
	"$(INTDIR)\IO_SAP.obj" \
	"$(INTDIR)\IOStream.obj" \
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
	"$(INTDIR)\Message_Queue.obj" \
	"$(INTDIR)\Method_Request.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
	"$(INTDIR)\NT_Service.obj" \
	"$(INTDIR)\Object_Manager.obj" \
	"$(INTDIR)\Obstack.obj" \
	"$(INTDIR)\OS.obj" \
	"$(INTDIR)\Parse_Node.obj" \
	"$(INTDIR)\Pipe.obj" \
	"$(INTDIR)\Priority_Reactor.obj" \
	"$(INTDIR)\Proactor.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Process_Manager.obj" \
	"$(INTDIR)\Profile_Timer.obj" \
	"$(INTDIR)\Reactor.obj" \
	"$(INTDIR)\Read_Buffer.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\Registry_Name_Space.obj" \
	"$(INTDIR)\Remote_Name_Space.obj" \
	"$(INTDIR)\Remote_Tokens.obj" \
	"$(INTDIR)\Sched_Params.obj" \
	"$(INTDIR)\Select_Reactor.obj" \
	"$(INTDIR)\Select_Reactor_Base.obj" \
	"$(INTDIR)\Service_Config.obj" \
	"$(INTDIR)\Service_Manager.obj" \
	"$(INTDIR)\Service_Object.obj" \
	"$(INTDIR)\Service_Repository.obj" \
	"$(INTDIR)\Service_Types.obj" \
	"$(INTDIR)\Shared_Memory.obj" \
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
	"$(INTDIR)\Stats.obj" \
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
	"$(INTDIR)\TP_Reactor.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\TTY_IO.obj" \
	"$(INTDIR)\UNIX_Addr.obj" \
	"$(INTDIR)\UPIPE_Acceptor.obj" \
	"$(INTDIR)\UPIPE_Connector.obj" \
	"$(INTDIR)\UPIPE_Stream.obj" \
	"$(INTDIR)\WFMO_Reactor.obj"

"$(OUTDIR)\ace.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "ACE dynamic library - Win32 Debug"
# Name "ACE dynamic library - Win32 Unicode Debug"
# Name "ACE dynamic library - Win32 Unicode Release"
# Name "ACE dynamic library - Win32 Release"

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp
DEP_CPP_UPIPE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
DEP_CPP_UPIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Connector.h"\
	"..\ace/UPIPE_Connector.i"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
DEP_CPP_UPIPE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Acceptor.h"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Acceptor.h"\
	"..\ace/UPIPE_Acceptor.i"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
DEP_CPP_UNIX_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_Connector.h"\
	"..\ace/DEV_Connector.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/TTY_IO.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
DEP_CPP_TRACE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/Trace.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
DEP_CPP_TOKEN=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
DEP_CPP_TOKEN_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Invariants.h"\
	"..\ace/Token_Invariants.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
DEP_CPP_TOKEN_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Collection.h"\
	"..\ace/Token_Collection.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
DEP_CPP_TOKEN_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
DEP_CPP_TLI_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
DEP_CPP_TLI_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Connector.h"\
	"..\ace/TLI_Connector.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Acceptor.h"\
	"..\ace/TLI_Acceptor.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
DEP_CPP_TIMER=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Hash.h"\
	"..\ace/Timer_Hash_T.cpp"\
	"..\ace/Timer_Hash_T.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Request_Reply.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREAD=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
DEP_CPP_TASK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
DEP_CPP_SYSTE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/System_Time.h"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
DEP_CPP_SYNCH=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
DEP_CPP_SVC_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
DEP_CPP_SVC_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Shared_Memory.h"\
	"..\ace/SV_Shared_Memory.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Message.h"\
	"..\ace/SV_Message.i"\
	"..\ace/SV_Message_Queue.h"\
	"..\ace/SV_Message_Queue.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Message.h"\
	"..\ace/SV_Message.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
DEP_CPP_STRAT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
DEP_CPP_SPIPE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
DEP_CPP_SPIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Connector.h"\
	"..\ace/SPIPE_Connector.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
DEP_CPP_SPIPE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Acceptor.h"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
DEP_CPP_SPIPE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
DEP_CPP_SOCK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
DEP_CPP_SOCK_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
DEP_CPP_SOCK_D=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SOCK_Dgram_Mcast.h"\
	"..\ace/SOCK_Dgram_Mcast.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SOCK_Dgram_Bcast.h"\
	"..\ace/SOCK_Dgram_Bcast.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
DEP_CPP_SOCK_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
DEP_CPP_SOCK_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_CODgram.h"\
	"..\ace/SOCK_CODgram.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
DEP_CPP_SHARE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
DEP_CPP_SHARED=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/Shared_Memory_SV.h"\
	"..\ace/Shared_Memory_SV.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Shared_Memory.h"\
	"..\ace/SV_Shared_Memory.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
DEP_CPP_SHARED_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/Shared_Memory_MM.h"\
	"..\ace/Shared_Memory_MM.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
DEP_CPP_SERVI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVIC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
DEP_CPP_SERVICE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp
DEP_CPP_SERVICE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sched_Params.cpp
DEP_CPP_SCHED=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Sched_Params.h"\
	"..\ace/Sched_Params.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Tokens.h"\
	"..\ace/Remote_Tokens.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
DEP_CPP_REGIS=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Registry.h"\
	"..\ace/Registry_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIST=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Registry.h"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
DEP_CPP_READ_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Read_Buffer.h"\
	"..\ace/Read_Buffer.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Msg_WFMO_Reactor.h"\
	"..\ace/Msg_WFMO_Reactor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/TP_Reactor.h"\
	"..\ace/TP_Reactor.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
DEP_CPP_PROFI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/Process_Manager.h"\
	"..\ace/Process_Manager.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
DEP_CPP_PROCES=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
DEP_CPP_PROAC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
DEP_CPP_PARSE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Sched_Params.h"\
	"..\ace/Sched_Params.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
DEP_CPP_OBSTA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_Manager.cpp
DEP_CPP_OBJEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Registry.h"\
	"..\ace/Registry_Name_Space.h"\
	"..\ace/Remote_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
DEP_CPP_NAME_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
DEP_CPP_MULTI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Multiplexor.h"\
	"..\ace/Multiplexor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Request.cpp
DEP_CPP_METHO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Method_Request.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timeprobe.h"\
	"..\ace/Timeprobe.i"\
	"..\ace/Timeprobe_T.cpp"\
	"..\ace/Timeprobe_T.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
DEP_CPP_MEM_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Dgram.h"\
	"..\ace/LSOCK_Dgram.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
DEP_CPP_LSOCK_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Connector.h"\
	"..\ace/LSOCK_Connector.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
DEP_CPP_LSOCK_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_CODgram.h"\
	"..\ace/LSOCK_CODgram.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_CODgram.h"\
	"..\ace/SOCK_CODgram.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Acceptor.h"\
	"..\ace/LSOCK_Acceptor.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
DEP_CPP_LOG_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Connector.h"\
	"..\ace/SPIPE_Connector.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IOStream.cpp
DEP_CPP_IOSTR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IOStream.h"\
	"..\ace/IOStream_T.cpp"\
	"..\ace/IOStream_T.h"\
	"..\ace/IOStream_T.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\INET_Addr.cpp
DEP_CPP_INET_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Stats.h"\
	"..\ace/Stats.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Filecache.cpp
DEP_CPP_FILEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Filecache.h"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/FILE_IO.h"\
	"..\ace/FILE_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
DEP_CPP_FILE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/FILE_Connector.h"\
	"..\ace/FILE_Connector.i"\
	"..\ace/FILE_IO.h"\
	"..\ace/FILE_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
DEP_CPP_FILE_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Send.h"\
	"..\ace/FIFO_Send.i"\
	"..\ace/FIFO_Send_Msg.h"\
	"..\ace/FIFO_Send_Msg.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Send.h"\
	"..\ace/FIFO_Send.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Recv.h"\
	"..\ace/FIFO_Recv.i"\
	"..\ace/FIFO_Recv_Msg.h"\
	"..\ace/FIFO_Recv_Msg.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Recv.h"\
	"..\ace/FIFO_Recv.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
DEP_CPP_FIFO_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
DEP_CPP_EVENT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
DEP_CPP_DYNAM=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
DEP_CPP_DUMP_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Dump.h"\
	"..\ace/Dump_T.cpp"\
	"..\ace/Dump_T.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
DEP_CPP_DEV_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_Connector.h"\
	"..\ace/DEV_Connector.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
DEP_CPP_DEV_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
DEP_CPP_DATE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Date_Time.h"\
	"..\ace/Date_Time.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/CORBA_Handler.h"\
	"..\ace/CORBA_Handler.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Asynch_IO.cpp
DEP_CPP_ASYNC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
DEP_CPP_ACTIV=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Activation_Queue.h"\
	"..\ace/Activation_Queue.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Method_Request.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
DEP_CPP_ACE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/Version.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Types.cpp
DEP_CPP_SERVICE_T=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor.cpp
DEP_CPP_SELEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp
DEP_CPP_WFMO_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Priority_Reactor.cpp
DEP_CPP_PRIOR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Priority_Reactor.h"\
	"..\ace/Priority_Reactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Basic_Types.cpp
DEP_CPP_BASIC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Filecache.h"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Tokens.h"\
	"..\ace/Remote_Tokens.i"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Template_Instantiations.cpp"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Hash.h"\
	"..\ace/Timer_Hash_T.cpp"\
	"..\ace/Timer_Hash_T.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Token_Invariants.h"\
	"..\ace/Token_Invariants.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory.cpp
DEP_CPP_SHARED_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp
DEP_CPP_MSG_W=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Msg_WFMO_Reactor.h"\
	"..\ace/Msg_WFMO_Reactor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Arg_Shifter.cpp
DEP_CPP_ARG_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Arg_Shifter.h"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Stats.cpp
DEP_CPP_STATS=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Stats.h"\
	"..\ace/Stats.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TP_Reactor.cpp
DEP_CPP_TP_RE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/TP_Reactor.h"\
	"..\ace/TP_Reactor.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TP_Reactor.obj" : $(SOURCE) $(DEP_CPP_TP_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TP_Reactor.obj" : $(SOURCE) $(DEP_CPP_TP_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TP_Reactor.obj" : $(SOURCE) $(DEP_CPP_TP_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TP_Reactor.obj" : $(SOURCE) $(DEP_CPP_TP_RE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Containers.cpp
DEP_CPP_CONTA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor_Base.cpp
DEP_CPP_SELECT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NT_Service.cpp
DEP_CPP_NT_SE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/NT_Service.h"\
	"..\ace/NT_Service.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Queue.cpp
DEP_CPP_MESSAG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "ACE static library - Win32 Release"
# Name "ACE static library - Win32 Debug"
# Name "ACE static library - Win32 Unicode Debug"
# Name "ACE static library - Win32 Unicode Release"

!IF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp
DEP_CPP_UPIPE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
DEP_CPP_UPIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Connector.h"\
	"..\ace/UPIPE_Connector.i"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_UPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
DEP_CPP_UPIPE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Acceptor.h"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/UPIPE_Acceptor.h"\
	"..\ace/UPIPE_Acceptor.i"\
	"..\ace/UPIPE_Addr.h"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_UPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
DEP_CPP_UNIX_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) $(DEP_CPP_UNIX_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
DEP_CPP_TTY_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_Connector.h"\
	"..\ace/DEV_Connector.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/TTY_IO.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) $(DEP_CPP_TTY_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
DEP_CPP_TRACE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/Trace.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
DEP_CPP_TOKEN=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
DEP_CPP_TOKEN_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) $(DEP_CPP_TOKEN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
DEP_CPP_TOKEN_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Invariants.h"\
	"..\ace/Token_Invariants.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) $(DEP_CPP_TOKEN_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
DEP_CPP_TOKEN_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Collection.h"\
	"..\ace/Token_Collection.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) $(DEP_CPP_TOKEN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
DEP_CPP_TOKEN_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
DEP_CPP_TLI_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) $(DEP_CPP_TLI_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
DEP_CPP_TLI_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Connector.h"\
	"..\ace/TLI_Connector.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) $(DEP_CPP_TLI_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
DEP_CPP_TLI_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/TLI_Acceptor.h"\
	"..\ace/TLI_Acceptor.i"\
	"..\ace/TLI_Stream.h"\
	"..\ace/TLI_Stream.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) $(DEP_CPP_TLI_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
DEP_CPP_TLI_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/TLI.h"\
	"..\ace/TLI.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) $(DEP_CPP_TLI_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
DEP_CPP_TIMER=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Hash.h"\
	"..\ace/Timer_Hash_T.cpp"\
	"..\ace/Timer_Hash_T.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
DEP_CPP_TIME_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Request_Reply.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
DEP_CPP_THREA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREAD=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
DEP_CPP_TASK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
DEP_CPP_SYSTE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/System_Time.h"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
DEP_CPP_SYNCH=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) $(DEP_CPP_SYNCH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
DEP_CPP_SYNCH_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) $(DEP_CPP_SYNCH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
DEP_CPP_SVC_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) $(DEP_CPP_SVC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
DEP_CPP_SVC_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) $(DEP_CPP_SVC_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
DEP_CPP_SV_SH=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Shared_Memory.h"\
	"..\ace/SV_Shared_Memory.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
DEP_CPP_SV_SE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) $(DEP_CPP_SV_SE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
DEP_CPP_SV_SEM=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) $(DEP_CPP_SV_SEM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
DEP_CPP_SV_ME=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Message.h"\
	"..\ace/SV_Message.i"\
	"..\ace/SV_Message_Queue.h"\
	"..\ace/SV_Message_Queue.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) $(DEP_CPP_SV_ME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
DEP_CPP_SV_MES=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Message.h"\
	"..\ace/SV_Message.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) $(DEP_CPP_SV_MES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
DEP_CPP_STRAT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) $(DEP_CPP_STRAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
DEP_CPP_SSTRI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SString.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
DEP_CPP_SPIPE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
DEP_CPP_SPIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Connector.h"\
	"..\ace/SPIPE_Connector.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) $(DEP_CPP_SPIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
DEP_CPP_SPIPE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) $(DEP_CPP_SPIPE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
DEP_CPP_SPIPE_AC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Acceptor.h"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SPIPE_AC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
DEP_CPP_SPIPE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) $(DEP_CPP_SPIPE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
DEP_CPP_SOCK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
DEP_CPP_SOCK_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) $(DEP_CPP_SOCK_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
DEP_CPP_SOCK_D=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SOCK_Dgram_Mcast.h"\
	"..\ace/SOCK_Dgram_Mcast.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
DEP_CPP_SOCK_DG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SOCK_Dgram_Bcast.h"\
	"..\ace/SOCK_Dgram_Bcast.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) $(DEP_CPP_SOCK_DG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
DEP_CPP_SOCK_DGR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_DGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
DEP_CPP_SOCK_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
DEP_CPP_SOCK_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_CODgram.h"\
	"..\ace/SOCK_CODgram.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_SOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
DEP_CPP_SOCK_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_SOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
DEP_CPP_SOCK_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) $(DEP_CPP_SOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
DEP_CPP_SIGNA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) $(DEP_CPP_SIGNA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
DEP_CPP_SHARE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
DEP_CPP_SHARED=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/Shared_Memory_SV.h"\
	"..\ace/Shared_Memory_SV.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Shared_Memory.h"\
	"..\ace/SV_Shared_Memory.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) $(DEP_CPP_SHARED) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
DEP_CPP_SHARED_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/Shared_Memory_MM.h"\
	"..\ace/Shared_Memory_MM.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) $(DEP_CPP_SHARED_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
DEP_CPP_SERVI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
DEP_CPP_SERVIC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) $(DEP_CPP_SERVIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
DEP_CPP_SERVICE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp
DEP_CPP_SERVICE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf.h"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) $(DEP_CPP_SERVICE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sched_Params.cpp
DEP_CPP_SCHED=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Sched_Params.h"\
	"..\ace/Sched_Params.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) $(DEP_CPP_SCHED) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
DEP_CPP_REMOT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Tokens.h"\
	"..\ace/Remote_Tokens.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) $(DEP_CPP_REMOT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
DEP_CPP_REMOTE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REMOTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
DEP_CPP_REGIS=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Registry.h"\
	"..\ace/Registry_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIST=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Registry.h"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
DEP_CPP_READ_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Read_Buffer.h"\
	"..\ace/Read_Buffer.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) $(DEP_CPP_READ_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
DEP_CPP_REACT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Msg_WFMO_Reactor.h"\
	"..\ace/Msg_WFMO_Reactor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/TP_Reactor.h"\
	"..\ace/TP_Reactor.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
DEP_CPP_PROFI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
DEP_CPP_PROCE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/Process_Manager.h"\
	"..\ace/Process_Manager.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
DEP_CPP_PROCES=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
DEP_CPP_PROAC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) $(DEP_CPP_PROAC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
DEP_CPP_PIPE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
DEP_CPP_PARSE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Parse_Node.h"\
	"..\ace/Parse_Node.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
DEP_CPP_OS_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Sched_Params.h"\
	"..\ace/Sched_Params.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\OS.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
DEP_CPP_OBSTA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) $(DEP_CPP_OBSTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_Manager.cpp
DEP_CPP_OBJEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Manager.h"\
	"..\ace/Service_Manager.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
DEP_CPP_NAMIN=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Registry.h"\
	"..\ace/Registry_Name_Space.h"\
	"..\ace/Remote_Name_Space.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
DEP_CPP_NAME_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
DEP_CPP_NAME_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) $(DEP_CPP_NAME_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
DEP_CPP_NAME_P=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) $(DEP_CPP_NAME_P) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
DEP_CPP_MULTI=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Multiplexor.h"\
	"..\ace/Multiplexor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Request.cpp
DEP_CPP_METHO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Method_Request.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Method_Request.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
DEP_CPP_MESSA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timeprobe.h"\
	"..\ace/Timeprobe.i"\
	"..\ace/Timeprobe_T.cpp"\
	"..\ace/Timeprobe_T.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
DEP_CPP_MEMOR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) $(DEP_CPP_MEMOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
DEP_CPP_MEM_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
DEP_CPP_MALLO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
DEP_CPP_LSOCK=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) $(DEP_CPP_LSOCK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
DEP_CPP_LSOCK_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Dgram.h"\
	"..\ace/LSOCK_Dgram.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Dgram.h"\
	"..\ace/SOCK_Dgram.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
DEP_CPP_LSOCK_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Connector.h"\
	"..\ace/LSOCK_Connector.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) $(DEP_CPP_LSOCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
DEP_CPP_LSOCK_CO=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_CODgram.h"\
	"..\ace/LSOCK_CODgram.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_CODgram.h"\
	"..\ace/SOCK_CODgram.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
DEP_CPP_LSOCK_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/LSOCK_Acceptor.h"\
	"..\ace/LSOCK_Acceptor.i"\
	"..\ace/LSOCK_Stream.h"\
	"..\ace/LSOCK_Stream.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/UNIX_Addr.h"\
	"..\ace/UNIX_Addr.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) $(DEP_CPP_LSOCK_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
DEP_CPP_LSOCK_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/LSOCK.h"\
	"..\ace/LSOCK.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) $(DEP_CPP_LSOCK_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
DEP_CPP_LOG_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) $(DEP_CPP_LOG_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
DEP_CPP_LOG_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Addr.h"\
	"..\ace/SPIPE_Addr.i"\
	"..\ace/SPIPE_Connector.h"\
	"..\ace/SPIPE_Connector.i"\
	"..\ace/SPIPE_Stream.h"\
	"..\ace/SPIPE_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) $(DEP_CPP_LOG_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
DEP_CPP_LOCAL=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Manager.h"\
	"..\ace/Token_Manager.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
DEP_CPP_LOCAL_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) $(DEP_CPP_LOCAL_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
DEP_CPP_IPC_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) $(DEP_CPP_IPC_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IOStream.cpp
DEP_CPP_IOSTR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IOStream.h"\
	"..\ace/IOStream_T.cpp"\
	"..\ace/IOStream_T.h"\
	"..\ace/IOStream_T.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
DEP_CPP_IO_SA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) $(DEP_CPP_IO_SA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IO_Cntl_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\INET_Addr.cpp
DEP_CPP_INET_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) $(DEP_CPP_INET_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
DEP_CPP_HIGH_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Stats.h"\
	"..\ace/Stats.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
DEP_CPP_HANDL=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
DEP_CPP_GET_O=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) $(DEP_CPP_GET_O) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Filecache.cpp
DEP_CPP_FILEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Filecache.h"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
DEP_CPP_FILE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/FILE_IO.h"\
	"..\ace/FILE_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
DEP_CPP_FILE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/FILE_Connector.h"\
	"..\ace/FILE_Connector.i"\
	"..\ace/FILE_IO.h"\
	"..\ace/FILE_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) $(DEP_CPP_FILE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
DEP_CPP_FILE_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) $(DEP_CPP_FILE_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
DEP_CPP_FILE_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FILE.h"\
	"..\ace/FILE.i"\
	"..\ace/FILE_Addr.h"\
	"..\ace/FILE_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) $(DEP_CPP_FILE_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
DEP_CPP_FIFO_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Send.h"\
	"..\ace/FIFO_Send.i"\
	"..\ace/FIFO_Send_Msg.h"\
	"..\ace/FIFO_Send_Msg.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
DEP_CPP_FIFO_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Send.h"\
	"..\ace/FIFO_Send.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) $(DEP_CPP_FIFO_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
DEP_CPP_FIFO_R=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Recv.h"\
	"..\ace/FIFO_Recv.i"\
	"..\ace/FIFO_Recv_Msg.h"\
	"..\ace/FIFO_Recv_Msg.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) $(DEP_CPP_FIFO_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
DEP_CPP_FIFO_RE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/FIFO_Recv.h"\
	"..\ace/FIFO_Recv.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) $(DEP_CPP_FIFO_RE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
DEP_CPP_FIFO_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/FIFO.h"\
	"..\ace/FIFO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) $(DEP_CPP_FIFO_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
DEP_CPP_EVENT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
DEP_CPP_DYNAM=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
DEP_CPP_DUMP_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Dump.h"\
	"..\ace/Dump_T.cpp"\
	"..\ace/Dump_T.h"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
DEP_CPP_DEV_I=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) $(DEP_CPP_DEV_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
DEP_CPP_DEV_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/DEV_Connector.h"\
	"..\ace/DEV_Connector.i"\
	"..\ace/DEV_IO.h"\
	"..\ace/DEV_IO.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) $(DEP_CPP_DEV_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
DEP_CPP_DEV_A=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) $(DEP_CPP_DEV_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
DEP_CPP_DEV_CP=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/DEV.h"\
	"..\ace/DEV.i"\
	"..\ace/DEV_Addr.h"\
	"..\ace/DEV_Addr.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_SAP.h"\
	"..\ace/IO_SAP.i"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) $(DEP_CPP_DEV_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
DEP_CPP_DATE_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Date_Time.h"\
	"..\ace/Date_Time.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
DEP_CPP_CORBA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/CORBA_Handler.h"\
	"..\ace/CORBA_Handler.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Asynch_IO.cpp
DEP_CPP_ASYNC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Asynch_IO.h"\
	"..\ace/Asynch_IO.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Proactor.h"\
	"..\ace/Proactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel.h"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) $(DEP_CPP_ASYNC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
DEP_CPP_ARGV_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/ARGV.h"\
	"..\ace/ARGV.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) $(DEP_CPP_ARGV_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
DEP_CPP_ADDR_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) $(DEP_CPP_ADDR_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
DEP_CPP_ACTIV=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Activation_Queue.h"\
	"..\ace/Activation_Queue.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Method_Request.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) $(DEP_CPP_ACTIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
DEP_CPP_ACE_C=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/Version.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) $(DEP_CPP_ACE_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Types.cpp
DEP_CPP_SERVICE_T=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) $(DEP_CPP_SERVICE_T) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp
DEP_CPP_WFMO_=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_WFMO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor.cpp
DEP_CPP_SELEC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Priority_Reactor.cpp
DEP_CPP_PRIOR=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Priority_Reactor.h"\
	"..\ace/Priority_Reactor.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Basic_Types.cpp
DEP_CPP_BASIC=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Dynamic.h"\
	"..\ace/Dynamic.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Filecache.h"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Local_Name_Space.h"\
	"..\ace/Local_Name_Space_T.cpp"\
	"..\ace/Local_Name_Space_T.h"\
	"..\ace/Local_Tokens.h"\
	"..\ace/Local_Tokens.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Map_Manager.cpp"\
	"..\ace/Map_Manager.h"\
	"..\ace/Map_Manager.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/Name_Proxy.h"\
	"..\ace/Name_Request_Reply.h"\
	"..\ace/Name_Space.h"\
	"..\ace/Naming_Context.h"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/Obstack.h"\
	"..\ace/Obstack.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Remote_Tokens.h"\
	"..\ace/Remote_Tokens.i"\
	"..\ace/Select_Reactor.h"\
	"..\ace/Select_Reactor.i"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Select_Reactor_T.cpp"\
	"..\ace/Select_Reactor_T.h"\
	"..\ace/Select_Reactor_T.i"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream.cpp"\
	"..\ace/Stream.h"\
	"..\ace/Stream.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Template_Instantiations.cpp"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Hash.h"\
	"..\ace/Timer_Hash_T.cpp"\
	"..\ace/Timer_Hash_T.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_List_T.cpp"\
	"..\ace/Timer_List_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Timer_Wheel_T.cpp"\
	"..\ace/Timer_Wheel_T.h"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Token_Invariants.h"\
	"..\ace/Token_Invariants.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory.cpp
DEP_CPP_SHARED_M=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Shared_Memory.h"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) $(DEP_CPP_SHARED_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp
DEP_CPP_MSG_W=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Msg_WFMO_Reactor.h"\
	"..\ace/Msg_WFMO_Reactor.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) $(DEP_CPP_MSG_W) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Arg_Shifter.cpp
DEP_CPP_ARG_S=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Arg_Shifter.h"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Arg_Shifter.obj" : $(SOURCE) $(DEP_CPP_ARG_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Stats.cpp
DEP_CPP_STATS=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Stats.h"\
	"..\ace/Stats.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Containers.cpp
DEP_CPP_CONTA=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Containers.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor_Base.cpp
DEP_CPP_SELECT=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Addr.h"\
	"..\ace/Addr.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/inc_user_config.h"\
	"..\ace/INET_Addr.h"\
	"..\ace/INET_Addr.i"\
	"..\ace/iosfwd.h"\
	"..\ace/IPC_SAP.h"\
	"..\ace/IPC_SAP.i"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Select_Reactor_Base.h"\
	"..\ace/Select_Reactor_Base.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SOCK.h"\
	"..\ace/SOCK.i"\
	"..\ace/SOCK_Acceptor.h"\
	"..\ace/SOCK_Acceptor.i"\
	"..\ace/SOCK_Connector.h"\
	"..\ace/SOCK_Connector.i"\
	"..\ace/SOCK_IO.h"\
	"..\ace/SOCK_IO.i"\
	"..\ace/SOCK_Stream.h"\
	"..\ace/SOCK_Stream.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Heap.h"\
	"..\ace/Timer_Heap_T.cpp"\
	"..\ace/Timer_Heap_T.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor_Base.obj" : $(SOURCE) $(DEP_CPP_SELECT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NT_Service.cpp
DEP_CPP_NT_SE=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
	"..\ace/NT_Service.h"\
	"..\ace/NT_Service.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/Stream_Modules.cpp"\
	"..\ace/Stream_Modules.h"\
	"..\ace/Stream_Modules.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Task.h"\
	"..\ace/Task.i"\
	"..\ace/Task_T.cpp"\
	"..\ace/Task_T.h"\
	"..\ace/Task_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\NT_Service.obj" : $(SOURCE) $(DEP_CPP_NT_SE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Release"

DEP_CPP_MESSAG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"

DEP_CPP_MESSAG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

DEP_CPP_MESSAG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

DEP_CPP_MESSAG=\
	"..\ace/ACE.h"\
	"..\ace/ACE.i"\
	"..\ace/Atomic_Op.i"\
	"..\ace/Auto_Ptr.cpp"\
	"..\ace/Auto_Ptr.h"\
	"..\ace/Auto_Ptr.i"\
	"..\ace/Basic_Types.h"\
	"..\ace/Basic_Types.i"\
	"..\ace/config-win32-borland.h"\
	"..\ace/config-win32-common.h"\
	"..\ace/config-win32.h"\
	"..\ace/config-WinCE.h"\
	"..\ace/config.h"\
	"..\ace/Containers.h"\
	"..\ace/Containers.i"\
	"..\ace/Containers_T.cpp"\
	"..\ace/Containers_T.h"\
	"..\ace/Containers_T.i"\
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
	"..\ace/Free_List.cpp"\
	"..\ace/Free_List.h"\
	"..\ace/Free_List.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/inc_user_config.h"\
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/Log_Msg.h"\
	"..\ace/Log_Priority.h"\
	"..\ace/Log_Record.h"\
	"..\ace/Log_Record.i"\
	"..\ace/Malloc.h"\
	"..\ace/Malloc.i"\
	"..\ace/Malloc_Base.h"\
	"..\ace/Malloc_T.cpp"\
	"..\ace/Malloc_T.h"\
	"..\ace/Malloc_T.i"\
	"..\ace/Managed_Object.cpp"\
	"..\ace/Managed_Object.h"\
	"..\ace/Managed_Object.i"\
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
	"..\ace/Memory_Pool.h"\
	"..\ace/Memory_Pool.i"\
	"..\ace/Message_Block.h"\
	"..\ace/Message_Block.i"\
	"..\ace/Message_Block_T.cpp"\
	"..\ace/Message_Block_T.h"\
	"..\ace/Message_Block_T.i"\
	"..\ace/Message_Queue.h"\
	"..\ace/Message_Queue.i"\
	"..\ace/Message_Queue_T.cpp"\
	"..\ace/Message_Queue_T.h"\
	"..\ace/Message_Queue_T.i"\
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Service_Config.h"\
	"..\ace/Service_Config.i"\
	"..\ace/Service_Object.h"\
	"..\ace/Service_Object.i"\
	"..\ace/Service_Repository.h"\
	"..\ace/Service_Repository.i"\
	"..\ace/Service_Types.h"\
	"..\ace/Service_Types.i"\
	"..\ace/Shared_Object.h"\
	"..\ace/Shared_Object.i"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/SString.h"\
	"..\ace/SString.i"\
	"..\ace/Strategies.h"\
	"..\ace/Strategies.i"\
	"..\ace/Strategies_T.cpp"\
	"..\ace/Strategies_T.h"\
	"..\ace/Strategies_T.i"\
	"..\ace/streams.h"\
	"..\ace/SV_Semaphore_Complex.h"\
	"..\ace/SV_Semaphore_Complex.i"\
	"..\ace/SV_Semaphore_Simple.h"\
	"..\ace/SV_Semaphore_Simple.i"\
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
	"..\ace/Synch_Options.h"\
	"..\ace/Synch_Options.i"\
	"..\ace/Synch_T.cpp"\
	"..\ace/Synch_T.h"\
	"..\ace/Synch_T.i"\
	"..\ace/Thread.h"\
	"..\ace/Thread.i"\
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	

"$(INTDIR)\Message_Queue.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
