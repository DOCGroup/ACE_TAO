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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D\
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MTd /W3 /GX /Z7 /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D\
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "..\STL" /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MTd /W3 /GX /Z7 /Od /I "..\STL" /D "_DEBUG" /D "UNICODE"\
 /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /Fo"$(INTDIR)/"\
 /c 
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "UNICODE" /D\
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D\
 "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
	-@erase "$(OUTDIR)\aceu.dll"
	-@erase "$(OUTDIR)\aceu.exp"
	-@erase "$(OUTDIR)\aceu.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "UNICODE" /D\
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
	-@erase "$(INTDIR)\Method_Object.obj"
	-@erase "$(INTDIR)\Msg_WFMO_Reactor.obj"
	-@erase "$(INTDIR)\Multiplexor.obj"
	-@erase "$(INTDIR)\Name_Proxy.obj"
	-@erase "$(INTDIR)\Name_Request_Reply.obj"
	-@erase "$(INTDIR)\Name_Space.obj"
	-@erase "$(INTDIR)\Naming_Context.obj"
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
	-@erase "$(OUTDIR)\ace.dll"
	-@erase "$(OUTDIR)\ace.exp"
	-@erase "$(OUTDIR)\ace.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=1 /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D\
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
	"$(INTDIR)\Method_Object.obj" \
	"$(INTDIR)\Msg_WFMO_Reactor.obj" \
	"$(INTDIR)\Multiplexor.obj" \
	"$(INTDIR)\Name_Proxy.obj" \
	"$(INTDIR)\Name_Request_Reply.obj" \
	"$(INTDIR)\Name_Space.obj" \
	"$(INTDIR)\Naming_Context.obj" \
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
NODEP_CPP_UPIPE=\
	".\ace\UPIPE_Stream.h"\
	".\ace\UPIPE_Stream.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
NODEP_CPP_UPIPE_=\
	".\ace\UPIPE_Connector.h"\
	".\ace\UPIPE_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
NODEP_CPP_UPIPE_A=\
	".\ace\UPIPE_Acceptor.h"\
	".\ace\UPIPE_Acceptor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
NODEP_CPP_UNIX_=\
	".\ace\UNIX_Addr.h"\
	".\ace\UNIX_Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
NODEP_CPP_TTY_I=\
	".\ace\TTY_IO.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
NODEP_CPP_TRACE=\
	".\ace\ACE.h"\
	".\ace\inc_user_config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Trace.h"\
	".\ace\Trace.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
NODEP_CPP_TOKEN=\
	".\ace\Token_Request_Reply.h"\
	".\ace\Token_Request_Reply.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
NODEP_CPP_TOKEN_=\
	".\ace\Object_Manager.h"\
	".\ace\Token_Manager.h"\
	".\ace\Token_Manager.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
NODEP_CPP_TOKEN_I=\
	".\ace\Object_Manager.h"\
	".\ace\Token_Invariants.h"\
	".\ace\Token_Invariants.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
NODEP_CPP_TOKEN_C=\
	".\ace\Token_Collection.h"\
	".\ace\Token_Collection.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
NODEP_CPP_TOKEN_CP=\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
NODEP_CPP_TLI_S=\
	".\ace\TLI_Stream.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
NODEP_CPP_TLI_C=\
	".\ace\Handle_Set.h"\
	".\ace\TLI_Connector.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
NODEP_CPP_TLI_A=\
	".\ace\TLI_Acceptor.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
NODEP_CPP_TLI_CP=\
	".\ace\TLI.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
NODEP_CPP_TIMER=\
	".\ace\Containers.h"\
	".\ace\Timer_Hash.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
NODEP_CPP_TIME_=\
	".\ace\Time_Request_Reply.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
NODEP_CPP_THREA=\
	".\ace\Dynamic.h"\
	".\ace\Object_Manager.h"\
	".\ace\Singleton.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
NODEP_CPP_THREAD=\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
NODEP_CPP_TASK_=\
	".\ace\Module.h"\
	".\ace\Service_Config.h"\
	".\ace\Task.h"\
	".\ace\Task.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
NODEP_CPP_SYSTE=\
	".\ace\System_Time.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
NODEP_CPP_SYNCH=\
	".\ace\Synch_Options.h"\
	".\ace\Synch_Options.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
NODEP_CPP_SYNCH_=\
	".\ace\Object_Manager.h"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
NODEP_CPP_SVC_C=\
	".\ace\ARGV.h"\
	".\ace\Module.h"\
	".\ace\Stream.h"\
	".\ace\Svc_Conf.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
NODEP_CPP_SVC_CO=\
	".\ace\OS.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Svc_Conf_Tokens.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
NODEP_CPP_SV_SH=\
	".\ace\SV_Shared_Memory.h"\
	".\ace\SV_Shared_Memory.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
NODEP_CPP_SV_SE=\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
NODEP_CPP_SV_SEM=\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
NODEP_CPP_SV_ME=\
	".\ace\SV_Message_Queue.h"\
	".\ace\SV_Message_Queue.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
NODEP_CPP_SV_MES=\
	".\ace\SV_Message.h"\
	".\ace\SV_Message.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
NODEP_CPP_STRAT=\
	".\ace\Reactor.h"\
	".\ace\Strategies.h"\
	".\ace\Strategies.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
NODEP_CPP_SSTRI=\
	".\ace\Malloc.h"\
	".\ace\Service_Config.h"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\streams.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
NODEP_CPP_SPIPE=\
	".\ace\SPIPE_Stream.h"\
	".\ace\SPIPE_Stream.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
NODEP_CPP_SPIPE_=\
	".\ace\SPIPE_Connector.h"\
	".\ace\SPIPE_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
NODEP_CPP_SPIPE_A=\
	".\ace\SPIPE_Addr.h"\
	".\ace\SPIPE_Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
NODEP_CPP_SPIPE_AC=\
	".\ace\SPIPE_Acceptor.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
NODEP_CPP_SPIPE_C=\
	".\ace\SPIPE.h"\
	".\ace\SPIPE.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
NODEP_CPP_SOCK_=\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
NODEP_CPP_SOCK_I=\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
NODEP_CPP_SOCK_D=\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Dgram_Mcast.h"\
	".\ace\SOCK_Dgram_Mcast.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
NODEP_CPP_SOCK_DG=\
	".\ace\SOCK_Dgram_Bcast.h"\
	".\ace\SOCK_Dgram_Bcast.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
NODEP_CPP_SOCK_DGR=\
	".\ace\Handle_Set.h"\
	".\ace\SOCK_Dgram.h"\
	".\ace\SOCK_Dgram.i"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
NODEP_CPP_SOCK_C=\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\SOCK_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
NODEP_CPP_SOCK_CO=\
	".\ace\SOCK_CODgram.h"\
	".\ace\SOCK_CODgram.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
NODEP_CPP_SOCK_A=\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Acceptor.i"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
NODEP_CPP_SOCK_CP=\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
NODEP_CPP_SIGNA=\
	".\ace\Object_Manager.h"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
NODEP_CPP_SHARE=\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
NODEP_CPP_SHARED=\
	".\ace\Shared_Memory_SV.h"\
	".\ace\Shared_Memory_SV.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
NODEP_CPP_SHARED_=\
	".\ace\Shared_Memory_MM.h"\
	".\ace\Shared_Memory_MM.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
NODEP_CPP_SERVI=\
	".\ace\Object_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Repository.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
NODEP_CPP_SERVIC=\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Service_Types.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
NODEP_CPP_SERVICE=\
	".\ace\Get_Opt.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Manager.i"\
	".\ace\Service_Repository.h"\
	".\ace\WFMO_Reactor.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp
NODEP_CPP_SERVICE_=\
	".\ace\ARGV.h"\
	".\ace\Auto_Ptr.h"\
	".\ace\Containers.h"\
	".\ace\Get_Opt.h"\
	".\ace\Malloc.h"\
	".\ace\Proactor.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Types.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sched_Params.cpp
NODEP_CPP_SCHED=\
	".\ace\ACE.h"\
	".\ace\Sched_Params.h"\
	".\ace\Sched_Params.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
NODEP_CPP_REMOT=\
	".\ace\Remote_Tokens.h"\
	".\ace\Remote_Tokens.i"\
	".\ace\Singleton.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
NODEP_CPP_REMOTE=\
	".\ace\Remote_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
NODEP_CPP_REGIS=\
	".\ace\Registry_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
NODEP_CPP_REGIST=\
	".\ace\Registry.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
NODEP_CPP_READ_=\
	".\ace\Read_Buffer.h"\
	".\ace\Read_Buffer.i"\
	".\ace\Service_Config.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
NODEP_CPP_REACT=\
	".\ace\Handle_Set.h"\
	".\ace\Msg_WFMO_Reactor.h"\
	".\ace\Object_Manager.h"\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\Reactor_Impl.h"\
	".\ace\Select_Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\WFMO_Reactor.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
NODEP_CPP_PROFI=\
	".\ace\Profile_Timer.h"\
	".\ace\Profile_Timer.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
NODEP_CPP_PROCE=\
	".\ace\Process.h"\
	".\ace\Process_Manager.h"\
	".\ace\Process_Manager.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
NODEP_CPP_PROCES=\
	".\ace\ARGV.h"\
	".\ace\Process.h"\
	".\ace\Process.i"\
	".\ace\SString.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
NODEP_CPP_PROAC=\
	".\ace\Log_Msg.h"\
	".\ace\Object_Manager.h"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Task_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
NODEP_CPP_PIPE_=\
	".\ace\Pipe.h"\
	".\ace\Pipe.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
NODEP_CPP_PARSE=\
	".\ace\Parse_Node.h"\
	".\ace\Parse_Node.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\ace\Task.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
NODEP_CPP_OS_CP=\
	".\ace\ARGV.h"\
	".\ace\Array.h"\
	".\ace\Containers.h"\
	".\ace\Object_Manager.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Sched_Params.h"\
	".\ace\SString.h"\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Task.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
NODEP_CPP_OBSTA=\
	".\ace\Obstack.h"\
	".\ace\Obstack.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_Manager.cpp
NODEP_CPP_OBJEC=\
	".\ace\Array.h"\
	".\ace\Containers.h"\
	".\ace\Log_Msg.h"\
	".\ace\Malloc.h"\
	".\ace\Naming_Context.h"\
	".\ace\Object_Manager.h"\
	".\ace\Object_Manager.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Signal.h"\
	".\ace\Synch.h"\
	".\ace\Token_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
NODEP_CPP_NAMIN=\
	".\ace\Get_Opt.h"\
	".\ace\Local_Name_Space_T.h"\
	".\ace\Memory_Pool.h"\
	".\ace\Naming_Context.h"\
	".\ace\Registry_Name_Space.h"\
	".\ace\Remote_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
NODEP_CPP_NAME_=\
	".\ace\Name_Space.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
NODEP_CPP_NAME_R=\
	".\ace\Name_Request_Reply.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
NODEP_CPP_NAME_P=\
	".\ace\Name_Proxy.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
NODEP_CPP_MULTI=\
	".\ace\Multiplexor.h"\
	".\ace\Multiplexor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
NODEP_CPP_METHO=\
	".\ace\Method_Object.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
NODEP_CPP_MESSA=\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
NODEP_CPP_MEMOR=\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
NODEP_CPP_MEM_M=\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
NODEP_CPP_MALLO=\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Object_Manager.h"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
NODEP_CPP_LSOCK=\
	".\ace\LSOCK_Stream.h"\
	".\ace\LSOCK_Stream.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
NODEP_CPP_LSOCK_=\
	".\ace\LSOCK_Dgram.h"\
	".\ace\LSOCK_Dgram.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
NODEP_CPP_LSOCK_C=\
	".\ace\LSOCK_Connector.h"\
	".\ace\LSOCK_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
NODEP_CPP_LSOCK_CO=\
	".\ace\LSOCK_CODgram.h"\
	".\ace\LSOCK_CODgram.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
NODEP_CPP_LSOCK_A=\
	".\ace\LSOCK_Acceptor.h"\
	".\ace\LSOCK_Acceptor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
NODEP_CPP_LSOCK_CP=\
	".\ace\LSOCK.h"\
	".\ace\LSOCK.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
NODEP_CPP_LOG_R=\
	".\ace\Log_Record.h"\
	".\ace\LOG_Record.i"\
	".\ace\streams.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
NODEP_CPP_LOG_M=\
	".\ace\ACE.h"\
	".\ace\inc_user_config.h"\
	".\ace\Managed_Object.h"\
	".\ace\Object_Manager.h"\
	".\ace\Signal.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\SPIPE_Connector.h"\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
NODEP_CPP_LOCAL=\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Thread.h"\
	".\ace\Token_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
NODEP_CPP_LOCAL_=\
	".\ace\ACE.h"\
	".\ace\Local_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
NODEP_CPP_IPC_S=\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IOStream.cpp
NODEP_CPP_IOSTR=\
	".\ace\IOStream.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
NODEP_CPP_IO_SA=\
	".\ace\IO_SAP.h"\
	".\ace\IO_SAP.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


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
NODEP_CPP_INET_=\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
NODEP_CPP_HIGH_=\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\Object_Manager.h"\
	".\ace\Stats.h"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
NODEP_CPP_HANDL=\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
NODEP_CPP_GET_O=\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Filecache.cpp
NODEP_CPP_FILEC=\
	".\ace\Filecache.h"\
	".\ace\Object_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
NODEP_CPP_FILE_=\
	".\ace\FILE_IO.h"\
	".\ace\FILE_IO.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
NODEP_CPP_FILE_C=\
	".\ace\FILE_Connector.h"\
	".\ace\FILE_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
NODEP_CPP_FILE_A=\
	".\ace\FILE_Addr.h"\
	".\ace\FILE_Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
NODEP_CPP_FILE_CP=\
	".\ace\FILE.h"\
	".\ace\FILE.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
NODEP_CPP_FIFO_=\
	".\ace\FIFO_Send_Msg.h"\
	".\ace\FIFO_Send_Msg.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
NODEP_CPP_FIFO_S=\
	".\ace\FIFO_Send.h"\
	".\ace\FIFO_Send.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
NODEP_CPP_FIFO_R=\
	".\ace\FIFO_Recv_Msg.h"\
	".\ace\FIFO_Recv_Msg.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
NODEP_CPP_FIFO_RE=\
	".\ace\FIFO_Recv.h"\
	".\ace\FIFO_Recv.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
NODEP_CPP_FIFO_C=\
	".\ace\FIFO.h"\
	".\ace\FIFO.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
NODEP_CPP_EVENT=\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Message_Block.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
NODEP_CPP_DYNAM=\
	".\ace\Dynamic.h"\
	".\ace\Dynamic.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
NODEP_CPP_DUMP_=\
	".\ace\Dump.h"\
	".\ace\Object_Manager.h"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
NODEP_CPP_DEV_I=\
	".\ace\DEV_IO.h"\
	".\ace\DEV_IO.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
NODEP_CPP_DEV_C=\
	".\ace\DEV_Connector.h"\
	".\ace\DEV_Connector.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
NODEP_CPP_DEV_A=\
	".\ace\DEV_Addr.h"\
	".\ace\DEV_Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
NODEP_CPP_DEV_CP=\
	".\ace\DEV.h"\
	".\ace\DEV.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
NODEP_CPP_DATE_=\
	".\ace\Date_Time.h"\
	".\ace\Date_Time.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
NODEP_CPP_CORBA=\
	".\ace\CORBA_Handler.h"\
	".\ace\CORBA_Handler.i"\
	".\ace\Object_Manager.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Asynch_IO.cpp
NODEP_CPP_ASYNC=\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\INET_Addr.h"\
	".\ace\Message_Block.h"\
	".\ace\Proactor.h"\
	".\ace\Service_Config.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
NODEP_CPP_ARGV_=\
	".\ace\ARGV.h"\
	".\ace\ARGV.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
NODEP_CPP_ADDR_=\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
NODEP_CPP_ACTIV=\
	".\ace\Activation_Queue.h"\
	".\ace\Activation_Queue.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
NODEP_CPP_ACE_C=\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Auto_Ptr.h"\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\IPC_SAP.h"\
	".\ace\Process.h"\
	".\ace\Reactor.h"\
	".\ace\SString.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Version.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Types.cpp
NODEP_CPP_SERVICE_T=\
	".\ace\Service_Types.h"\
	".\ace\Service_Types.i"\
	".\ace\Stream.h"\
	".\ace\Stream_Modules.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor.cpp
NODEP_CPP_SELEC=\
	".\ace\Reactor.h"\
	".\ace\Select_Reactor.h"\
	".\ace\Select_Reactor.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	".\ace\Timer_Heap.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp
NODEP_CPP_WFMO_=\
	".\ace\Auto_Ptr.h"\
	".\ace\Handle_Set.h"\
	".\ace\Thread.h"\
	".\ace\Timer_Heap.h"\
	".\ace\WFMO_Reactor.h"\
	".\ace\WFMO_Reactor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Priority_Reactor.cpp
NODEP_CPP_PRIOR=\
	".\ace\Malloc_T.h"\
	".\ace\Priority_Reactor.h"\
	".\ace\Priority_Reactor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Basic_Types.cpp
NODEP_CPP_BASIC=\
	".\ace\Basic_Types.i"\
	".\ace\OS.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory.cpp
NODEP_CPP_SHARED_M=\
	".\ace\Shared_Memory.h"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp
NODEP_CPP_MSG_W=\
	".\ace\Msg_WFMO_Reactor.h"\
	".\ace\Msg_WFMO_Reactor.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Arg_Shifter.cpp
DEP_CPP_ARG_S=\
	".\Arg_Shifter.h"\
	

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
NODEP_CPP_STATS=\
	".\ace\Stats.h"\
	".\ace\Stats.i"\
	

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


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
NODEP_CPP_UPIPE=\
	".\ace\UPIPE_Stream.h"\
	".\ace\UPIPE_Stream.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
NODEP_CPP_UPIPE_=\
	".\ace\UPIPE_Connector.h"\
	".\ace\UPIPE_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
NODEP_CPP_UPIPE_A=\
	".\ace\UPIPE_Acceptor.h"\
	".\ace\UPIPE_Acceptor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
NODEP_CPP_UNIX_=\
	".\ace\UNIX_Addr.h"\
	".\ace\UNIX_Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\UNIX_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TTY_IO.cpp
NODEP_CPP_TTY_I=\
	".\ace\TTY_IO.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TTY_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trace.cpp
NODEP_CPP_TRACE=\
	".\ace\ACE.h"\
	".\ace\inc_user_config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Trace.h"\
	".\ace\Trace.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Trace.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
NODEP_CPP_TOKEN=\
	".\ace\Token_Request_Reply.h"\
	".\ace\Token_Request_Reply.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Manager.cpp
NODEP_CPP_TOKEN_=\
	".\ace\Object_Manager.h"\
	".\ace\Token_Manager.h"\
	".\ace\Token_Manager.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Invariants.cpp
NODEP_CPP_TOKEN_I=\
	".\ace\Object_Manager.h"\
	".\ace\Token_Invariants.h"\
	".\ace\Token_Invariants.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Invariants.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token_Collection.cpp
NODEP_CPP_TOKEN_C=\
	".\ace\Token_Collection.h"\
	".\ace\Token_Collection.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token_Collection.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.cpp
NODEP_CPP_TOKEN_CP=\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Stream.cpp
NODEP_CPP_TLI_S=\
	".\ace\TLI_Stream.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Connector.cpp
NODEP_CPP_TLI_C=\
	".\ace\Handle_Set.h"\
	".\ace\TLI_Connector.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
NODEP_CPP_TLI_A=\
	".\ace\TLI_Acceptor.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TLI.cpp
NODEP_CPP_TLI_CP=\
	".\ace\TLI.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\TLI.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue.cpp
NODEP_CPP_TIMER=\
	".\ace\Containers.h"\
	".\ace\Timer_Hash.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Timer_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
NODEP_CPP_TIME_=\
	".\ace\Time_Request_Reply.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Time_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager.cpp
NODEP_CPP_THREA=\
	".\ace\Dynamic.h"\
	".\ace\Object_Manager.h"\
	".\ace\Singleton.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Thread_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
NODEP_CPP_THREAD=\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Task.cpp
NODEP_CPP_TASK_=\
	".\ace\Module.h"\
	".\ace\Service_Config.h"\
	".\ace\Task.h"\
	".\ace\Task.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Task.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\System_Time.cpp
NODEP_CPP_SYSTE=\
	".\ace\System_Time.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\System_Time.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch_Options.cpp
NODEP_CPP_SYNCH=\
	".\ace\Synch_Options.h"\
	".\ace\Synch_Options.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Synch_Options.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Synch.cpp
NODEP_CPP_SYNCH_=\
	".\ace\Object_Manager.h"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Synch.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
NODEP_CPP_SVC_C=\
	".\ace\ARGV.h"\
	".\ace\Module.h"\
	".\ace\Stream.h"\
	".\ace\Svc_Conf.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_y.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
NODEP_CPP_SVC_CO=\
	".\ace\OS.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Svc_Conf_Tokens.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Svc_Conf_l.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
NODEP_CPP_SV_SH=\
	".\ace\SV_Shared_Memory.h"\
	".\ace\SV_Shared_Memory.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
NODEP_CPP_SV_SE=\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Simple.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
NODEP_CPP_SV_SEM=\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Semaphore_Complex.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
NODEP_CPP_SV_ME=\
	".\ace\SV_Message_Queue.h"\
	".\ace\SV_Message_Queue.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Message_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SV_Message.cpp
NODEP_CPP_SV_MES=\
	".\ace\SV_Message.h"\
	".\ace\SV_Message.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SV_Message.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Strategies.cpp
NODEP_CPP_STRAT=\
	".\ace\Reactor.h"\
	".\ace\Strategies.h"\
	".\ace\Strategies.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Strategies.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SString.cpp
NODEP_CPP_SSTRI=\
	".\ace\Malloc.h"\
	".\ace\Service_Config.h"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\streams.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SString.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
NODEP_CPP_SPIPE=\
	".\ace\SPIPE_Stream.h"\
	".\ace\SPIPE_Stream.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
NODEP_CPP_SPIPE_=\
	".\ace\SPIPE_Connector.h"\
	".\ace\SPIPE_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
NODEP_CPP_SPIPE_A=\
	".\ace\SPIPE_Addr.h"\
	".\ace\SPIPE_Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
NODEP_CPP_SPIPE_AC=\
	".\ace\SPIPE_Acceptor.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIPE.cpp
NODEP_CPP_SPIPE_C=\
	".\ace\SPIPE.h"\
	".\ace\SPIPE.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SPIPE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
NODEP_CPP_SOCK_=\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_IO.cpp
NODEP_CPP_SOCK_I=\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
NODEP_CPP_SOCK_D=\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Dgram_Mcast.h"\
	".\ace\SOCK_Dgram_Mcast.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Mcast.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
NODEP_CPP_SOCK_DG=\
	".\ace\SOCK_Dgram_Bcast.h"\
	".\ace\SOCK_Dgram_Bcast.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram_Bcast.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
NODEP_CPP_SOCK_DGR=\
	".\ace\Handle_Set.h"\
	".\ace\SOCK_Dgram.h"\
	".\ace\SOCK_Dgram.i"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
NODEP_CPP_SOCK_C=\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\SOCK_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
NODEP_CPP_SOCK_CO=\
	".\ace\SOCK_CODgram.h"\
	".\ace\SOCK_CODgram.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
NODEP_CPP_SOCK_A=\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Acceptor.i"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SOCK.cpp
NODEP_CPP_SOCK_CP=\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\SOCK.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Signal.cpp
NODEP_CPP_SIGNA=\
	".\ace\Object_Manager.h"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Signal.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Object.cpp
NODEP_CPP_SHARE=\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
NODEP_CPP_SHARED=\
	".\ace\Shared_Memory_SV.h"\
	".\ace\Shared_Memory_SV.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_SV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
NODEP_CPP_SHARED_=\
	".\ace\Shared_Memory_MM.h"\
	".\ace\Shared_Memory_MM.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory_MM.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Repository.cpp
NODEP_CPP_SERVI=\
	".\ace\Object_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Repository.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Repository.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Object.cpp
NODEP_CPP_SERVIC=\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Service_Types.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Manager.cpp
NODEP_CPP_SERVICE=\
	".\ace\Get_Opt.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Manager.i"\
	".\ace\Service_Repository.h"\
	".\ace\WFMO_Reactor.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Config.cpp
NODEP_CPP_SERVICE_=\
	".\ace\ARGV.h"\
	".\ace\Auto_Ptr.h"\
	".\ace\Containers.h"\
	".\ace\Get_Opt.h"\
	".\ace\Malloc.h"\
	".\ace\Proactor.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\Service_Types.h"\
	".\ace\Svc_Conf.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Config.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sched_Params.cpp
NODEP_CPP_SCHED=\
	".\ace\ACE.h"\
	".\ace\Sched_Params.h"\
	".\ace\Sched_Params.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Sched_Params.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
NODEP_CPP_REMOT=\
	".\ace\Remote_Tokens.h"\
	".\ace\Remote_Tokens.i"\
	".\ace\Singleton.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Remote_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
NODEP_CPP_REMOTE=\
	".\ace\Remote_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Remote_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
NODEP_CPP_REGIS=\
	".\ace\Registry_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Registry_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
NODEP_CPP_REGIST=\
	".\ace\Registry.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Registry.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Read_Buffer.cpp
NODEP_CPP_READ_=\
	".\ace\Read_Buffer.h"\
	".\ace\Read_Buffer.i"\
	".\ace\Service_Config.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Read_Buffer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Reactor.cpp
NODEP_CPP_REACT=\
	".\ace\Handle_Set.h"\
	".\ace\Msg_WFMO_Reactor.h"\
	".\ace\Object_Manager.h"\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\Reactor_Impl.h"\
	".\ace\Select_Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\WFMO_Reactor.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Profile_Timer.cpp
NODEP_CPP_PROFI=\
	".\ace\Profile_Timer.h"\
	".\ace\Profile_Timer.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Profile_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process_Manager.cpp
NODEP_CPP_PROCE=\
	".\ace\Process.h"\
	".\ace\Process_Manager.h"\
	".\ace\Process_Manager.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Process_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.cpp
NODEP_CPP_PROCES=\
	".\ace\ARGV.h"\
	".\ace\Process.h"\
	".\ace\Process.i"\
	".\ace\SString.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Proactor.cpp
NODEP_CPP_PROAC=\
	".\ace\Log_Msg.h"\
	".\ace\Object_Manager.h"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Task_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Proactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pipe.cpp
NODEP_CPP_PIPE_=\
	".\ace\Pipe.h"\
	".\ace\Pipe.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Pipe.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Node.cpp
NODEP_CPP_PARSE=\
	".\ace\Parse_Node.h"\
	".\ace\Parse_Node.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\ace\Task.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Parse_Node.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OS.cpp
NODEP_CPP_OS_CP=\
	".\ace\ARGV.h"\
	".\ace\Array.h"\
	".\ace\Containers.h"\
	".\ace\Object_Manager.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Sched_Params.h"\
	".\ace\SString.h"\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Task.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\OS.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Obstack.cpp
NODEP_CPP_OBSTA=\
	".\ace\Obstack.h"\
	".\ace\Obstack.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Obstack.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_Manager.cpp
NODEP_CPP_OBJEC=\
	".\ace\Array.h"\
	".\ace\Containers.h"\
	".\ace\Log_Msg.h"\
	".\ace\Malloc.h"\
	".\ace\Naming_Context.h"\
	".\ace\Object_Manager.h"\
	".\ace\Object_Manager.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Manager.h"\
	".\ace\Signal.h"\
	".\ace\Synch.h"\
	".\ace\Token_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Object_Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Naming_Context.cpp
NODEP_CPP_NAMIN=\
	".\ace\Get_Opt.h"\
	".\ace\Local_Name_Space_T.h"\
	".\ace\Memory_Pool.h"\
	".\ace\Naming_Context.h"\
	".\ace\Registry_Name_Space.h"\
	".\ace\Remote_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Naming_Context.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Space.cpp
NODEP_CPP_NAME_=\
	".\ace\Name_Space.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
NODEP_CPP_NAME_R=\
	".\ace\Name_Request_Reply.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Request_Reply.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Proxy.cpp
NODEP_CPP_NAME_P=\
	".\ace\Name_Proxy.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Name_Proxy.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Multiplexor.cpp
NODEP_CPP_MULTI=\
	".\ace\Multiplexor.h"\
	".\ace\Multiplexor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Multiplexor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Method_Object.cpp
NODEP_CPP_METHO=\
	".\ace\Method_Object.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Method_Object.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message_Block.cpp
NODEP_CPP_MESSA=\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Message_Block.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Memory_Pool.cpp
NODEP_CPP_MEMOR=\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Memory_Pool.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mem_Map.cpp
NODEP_CPP_MEM_M=\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Mem_Map.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.cpp
NODEP_CPP_MALLO=\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Object_Manager.h"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Malloc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
NODEP_CPP_LSOCK=\
	".\ace\LSOCK_Stream.h"\
	".\ace\LSOCK_Stream.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
NODEP_CPP_LSOCK_=\
	".\ace\LSOCK_Dgram.h"\
	".\ace\LSOCK_Dgram.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Dgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
NODEP_CPP_LSOCK_C=\
	".\ace\LSOCK_Connector.h"\
	".\ace\LSOCK_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
NODEP_CPP_LSOCK_CO=\
	".\ace\LSOCK_CODgram.h"\
	".\ace\LSOCK_CODgram.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_CODgram.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
NODEP_CPP_LSOCK_A=\
	".\ace\LSOCK_Acceptor.h"\
	".\ace\LSOCK_Acceptor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK_Acceptor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSOCK.cpp
NODEP_CPP_LSOCK_CP=\
	".\ace\LSOCK.h"\
	".\ace\LSOCK.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\LSOCK.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Record.cpp
NODEP_CPP_LOG_R=\
	".\ace\Log_Record.h"\
	".\ace\LOG_Record.i"\
	".\ace\streams.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Log_Record.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Log_Msg.cpp
NODEP_CPP_LOG_M=\
	".\ace\ACE.h"\
	".\ace\inc_user_config.h"\
	".\ace\Managed_Object.h"\
	".\ace\Object_Manager.h"\
	".\ace\Signal.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\SPIPE_Connector.h"\
	".\ace\streams.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Log_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Tokens.cpp
NODEP_CPP_LOCAL=\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Thread.h"\
	".\ace\Token_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Local_Tokens.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
NODEP_CPP_LOCAL_=\
	".\ace\ACE.h"\
	".\ace\Local_Name_Space.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Local_Name_Space.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IPC_SAP.cpp
NODEP_CPP_IPC_S=\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IPC_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IOStream.cpp
NODEP_CPP_IOSTR=\
	".\ace\IOStream.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IOStream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO_SAP.cpp
NODEP_CPP_IO_SA=\
	".\ace\IO_SAP.h"\
	".\ace\IO_SAP.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\IO_SAP.obj" : $(SOURCE) "$(INTDIR)"


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
NODEP_CPP_INET_=\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\INET_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
NODEP_CPP_HIGH_=\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\Object_Manager.h"\
	".\ace\Stats.h"\
	".\ace\Synch.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\High_Res_Timer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Handle_Set.cpp
NODEP_CPP_HANDL=\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Handle_Set.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Get_Opt.cpp
NODEP_CPP_GET_O=\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Get_Opt.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Filecache.cpp
NODEP_CPP_FILEC=\
	".\ace\Filecache.h"\
	".\ace\Object_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Filecache.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_IO.cpp
NODEP_CPP_FILE_=\
	".\ace\FILE_IO.h"\
	".\ace\FILE_IO.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Connector.cpp
NODEP_CPP_FILE_C=\
	".\ace\FILE_Connector.h"\
	".\ace\FILE_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE_Addr.cpp
NODEP_CPP_FILE_A=\
	".\ace\FILE_Addr.h"\
	".\ace\FILE_Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILE.cpp
NODEP_CPP_FILE_CP=\
	".\ace\FILE.h"\
	".\ace\FILE.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FILE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
NODEP_CPP_FIFO_=\
	".\ace\FIFO_Send_Msg.h"\
	".\ace\FIFO_Send_Msg.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Send.cpp
NODEP_CPP_FIFO_S=\
	".\ace\FIFO_Send.h"\
	".\ace\FIFO_Send.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Send.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
NODEP_CPP_FIFO_R=\
	".\ace\FIFO_Recv_Msg.h"\
	".\ace\FIFO_Recv_Msg.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv_Msg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
NODEP_CPP_FIFO_RE=\
	".\ace\FIFO_Recv.h"\
	".\ace\FIFO_Recv.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO_Recv.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FIFO.cpp
NODEP_CPP_FIFO_C=\
	".\ace\FIFO.h"\
	".\ace\FIFO.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\FIFO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event_Handler.cpp
NODEP_CPP_EVENT=\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Message_Block.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Event_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dynamic.cpp
NODEP_CPP_DYNAM=\
	".\ace\Dynamic.h"\
	".\ace\Dynamic.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Dynamic.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dump.cpp
NODEP_CPP_DUMP_=\
	".\ace\Dump.h"\
	".\ace\Object_Manager.h"\
	".\ace\Synch_T.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Dump.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_IO.cpp
NODEP_CPP_DEV_I=\
	".\ace\DEV_IO.h"\
	".\ace\DEV_IO.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Connector.cpp
NODEP_CPP_DEV_C=\
	".\ace\DEV_Connector.h"\
	".\ace\DEV_Connector.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_Connector.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV_Addr.cpp
NODEP_CPP_DEV_A=\
	".\ace\DEV_Addr.h"\
	".\ace\DEV_Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV_Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEV.cpp
NODEP_CPP_DEV_CP=\
	".\ace\DEV.h"\
	".\ace\DEV.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\DEV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Date_Time.cpp
NODEP_CPP_DATE_=\
	".\ace\Date_Time.h"\
	".\ace\Date_Time.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Date_Time.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
NODEP_CPP_CORBA=\
	".\ace\CORBA_Handler.h"\
	".\ace\CORBA_Handler.i"\
	".\ace\Object_Manager.h"\
	".\ace\Thread_Manager.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\CORBA_Handler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Asynch_IO.cpp
NODEP_CPP_ASYNC=\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\INET_Addr.h"\
	".\ace\Message_Block.h"\
	".\ace\Proactor.h"\
	".\ace\Service_Config.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Asynch_IO.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ARGV.cpp
NODEP_CPP_ARGV_=\
	".\ace\ARGV.h"\
	".\ace\ARGV.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\ARGV.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Addr.cpp
NODEP_CPP_ADDR_=\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Addr.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Activation_Queue.cpp
NODEP_CPP_ACTIV=\
	".\ace\Activation_Queue.h"\
	".\ace\Activation_Queue.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Activation_Queue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ACE.cpp
NODEP_CPP_ACE_C=\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Auto_Ptr.h"\
	".\ace\Handle_Set.h"\
	".\ace\INET_Addr.h"\
	".\ace\IPC_SAP.h"\
	".\ace\Process.h"\
	".\ace\Reactor.h"\
	".\ace\SString.h"\
	".\ace\Thread_Manager.h"\
	".\ace\Version.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\ACE.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Service_Types.cpp
NODEP_CPP_SERVICE_T=\
	".\ace\Service_Types.h"\
	".\ace\Service_Types.i"\
	".\ace\Stream.h"\
	".\ace\Stream_Modules.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Service_Types.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp
NODEP_CPP_WFMO_=\
	".\ace\Auto_Ptr.h"\
	".\ace\Handle_Set.h"\
	".\ace\Thread.h"\
	".\ace\Timer_Heap.h"\
	".\ace\WFMO_Reactor.h"\
	".\ace\WFMO_Reactor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Select_Reactor.cpp
NODEP_CPP_SELEC=\
	".\ace\Reactor.h"\
	".\ace\Select_Reactor.h"\
	".\ace\Select_Reactor.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Connector.h"\
	".\ace\Synch_T.h"\
	".\ace\Thread.h"\
	".\ace\Timer_Heap.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Select_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Priority_Reactor.cpp
NODEP_CPP_PRIOR=\
	".\ace\Malloc_T.h"\
	".\ace\Priority_Reactor.h"\
	".\ace\Priority_Reactor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Priority_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Basic_Types.cpp
NODEP_CPP_BASIC=\
	".\ace\Basic_Types.i"\
	".\ace\OS.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Basic_Types.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory.cpp
NODEP_CPP_SHARED_M=\
	".\ace\Shared_Memory.h"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Shared_Memory.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp
NODEP_CPP_MSG_W=\
	".\ace\Msg_WFMO_Reactor.h"\
	".\ace\Msg_WFMO_Reactor.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Msg_WFMO_Reactor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Arg_Shifter.cpp
DEP_CPP_ARG_S=\
	".\Arg_Shifter.h"\
	

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
NODEP_CPP_STATS=\
	".\ace\Stats.h"\
	".\ace\Stats.i"\
	

!IF  "$(CFG)" == "ACE static library - Win32 Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"


"$(INTDIR)\Stats.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
