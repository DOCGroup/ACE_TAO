# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=netsvcs - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to netsvcs - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "netsvcs - Win32 Release" && "$(CFG)" !=\
 "netsvcs - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "netsvcs4.mak" CFG="netsvcs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netsvcs - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "netsvcs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "netsvcs - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "netsvcs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "netsvcs\Release"
# PROP BASE Intermediate_Dir "netsvcs\Release"
# PROP BASE Target_Dir "netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "netsvcs"
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\netsvcs.dll"

CLEAN : 
	-@erase "$(INTDIR)\Client_Logging_Handler.obj"
	-@erase "$(INTDIR)\Logging_Strategy.obj"
	-@erase "$(INTDIR)\Name_Handler.obj"
	-@erase "$(INTDIR)\Server_Logging_Handler.obj"
	-@erase "$(INTDIR)\Token_Handler.obj"
	-@erase "$(INTDIR)\TS_Clerk_Handler.obj"
	-@erase "$(INTDIR)\TS_Server_Handler.obj"
	-@erase "$(OUTDIR)\netsvcs.dll"
	-@erase "$(OUTDIR)\netsvcs.exp"
	-@erase "$(OUTDIR)\netsvcs.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/netsvcs.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/netsvcs.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\..\ace\ace.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=..\..\ace\ace.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/netsvcs.pdb" /machine:I386 /out:"$(OUTDIR)/netsvcs.dll"\
 /implib:"$(OUTDIR)/netsvcs.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Client_Logging_Handler.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\Name_Handler.obj" \
	"$(INTDIR)\Server_Logging_Handler.obj" \
	"$(INTDIR)\Token_Handler.obj" \
	"$(INTDIR)\TS_Clerk_Handler.obj" \
	"$(INTDIR)\TS_Server_Handler.obj"

"$(OUTDIR)\netsvcs.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "netsvcs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "netsvcs\Debug"
# PROP BASE Intermediate_Dir "netsvcs\Debug"
# PROP BASE Target_Dir "netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "debug"
# PROP Target_Dir "netsvcs"
OUTDIR=.
INTDIR=.\debug

ALL : "$(OUTDIR)\netsvcsd.dll"

CLEAN : 
	-@erase "$(INTDIR)\Client_Logging_Handler.obj"
	-@erase "$(INTDIR)\Logging_Strategy.obj"
	-@erase "$(INTDIR)\Name_Handler.obj"
	-@erase "$(INTDIR)\Server_Logging_Handler.obj"
	-@erase "$(INTDIR)\Token_Handler.obj"
	-@erase "$(INTDIR)\TS_Clerk_Handler.obj"
	-@erase "$(INTDIR)\TS_Server_Handler.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\netsvcsd.dll"
	-@erase "$(OUTDIR)\netsvcsd.exp"
	-@erase "$(OUTDIR)\netsvcsd.ilk"
	-@erase "$(OUTDIR)\netsvcsd.lib"
	-@erase "$(OUTDIR)\netsvcsd.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/netsvcs.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/netsvcs.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ..\..\ace\aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"netsvcsd.dll"
LINK32_FLAGS=..\..\ace\aced.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/netsvcsd.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/netsvcsd.dll" /implib:"$(OUTDIR)/netsvcsd.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Client_Logging_Handler.obj" \
	"$(INTDIR)\Logging_Strategy.obj" \
	"$(INTDIR)\Name_Handler.obj" \
	"$(INTDIR)\Server_Logging_Handler.obj" \
	"$(INTDIR)\Token_Handler.obj" \
	"$(INTDIR)\TS_Clerk_Handler.obj" \
	"$(INTDIR)\TS_Server_Handler.obj"

"$(OUTDIR)\netsvcsd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "netsvcs - Win32 Release"
# Name "netsvcs - Win32 Debug"

!IF  "$(CFG)" == "netsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "netsvcs - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Token_Handler.cpp
DEP_CPP_TOKEN=\
	"..\..\ace/Acceptor.cpp"\
	"..\..\ace/Acceptor.h"\
	"..\..\ace/Acceptor.i"\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Local_Tokens.h"\
	"..\..\ace/Local_Tokens.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Map_Manager.cpp"\
	"..\..\ace/Map_Manager.h"\
	"..\..\ace/Map_Manager.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Acceptor.h"\
	"..\..\ace/SOCK_Acceptor.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Token_Collection.h"\
	"..\..\ace/Token_Collection.i"\
	"..\..\ace/Token_Request_Reply.h"\
	"..\..\ace/Token_Request_Reply.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\Token_Handler.h"\
	

"$(INTDIR)\Token_Handler.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TS_Clerk_Handler.cpp
DEP_CPP_TS_CL=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Connector.cpp"\
	"..\..\ace/Connector.h"\
	"..\..\ace/Connector.i"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Map_Manager.cpp"\
	"..\..\ace/Map_Manager.h"\
	"..\..\ace/Map_Manager.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Connector.h"\
	"..\..\ace/SOCK_Connector.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Request_Reply.h"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\TS_Clerk_Handler.h"\
	

"$(INTDIR)\TS_Clerk_Handler.obj" : $(SOURCE) $(DEP_CPP_TS_CL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TS_Server_Handler.cpp
DEP_CPP_TS_SE=\
	"..\..\ace/Acceptor.cpp"\
	"..\..\ace/Acceptor.h"\
	"..\..\ace/Acceptor.i"\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Acceptor.h"\
	"..\..\ace/SOCK_Acceptor.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Request_Reply.h"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\TS_Server_Handler.h"\
	

"$(INTDIR)\TS_Server_Handler.obj" : $(SOURCE) $(DEP_CPP_TS_SE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Server_Logging_Handler.cpp
DEP_CPP_SERVE=\
	"..\..\ace/Acceptor.cpp"\
	"..\..\ace/Acceptor.h"\
	"..\..\ace/Acceptor.i"\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Acceptor.h"\
	"..\..\ace/SOCK_Acceptor.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/TLI.h"\
	"..\..\ace/TLI.i"\
	"..\..\ace/TLI_Acceptor.h"\
	"..\..\ace/TLI_Acceptor.i"\
	"..\..\ace/TLI_Stream.h"\
	"..\..\ace/TLI_Stream.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\Base_Optimizer.cpp"\
	".\Base_Optimizer.h"\
	".\Log_Message_Receiver.cpp"\
	".\Log_Message_Receiver.h"\
	".\Server_Logging_Handler.h"\
	".\Server_Logging_Handler_T.cpp"\
	".\Server_Logging_Handler_T.h"\
	

"$(INTDIR)\Server_Logging_Handler.obj" : $(SOURCE) $(DEP_CPP_SERVE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Name_Handler.cpp
DEP_CPP_NAME_=\
	"..\..\ace/Acceptor.cpp"\
	"..\..\ace/Acceptor.h"\
	"..\..\ace/Acceptor.i"\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Name_Proxy.h"\
	"..\..\ace/Name_Request_Reply.h"\
	"..\..\ace/Name_Space.h"\
	"..\..\ace/Naming_Context.h"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Acceptor.h"\
	"..\..\ace/SOCK_Acceptor.i"\
	"..\..\ace/SOCK_Connector.h"\
	"..\..\ace/SOCK_Connector.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\Name_Handler.h"\
	

"$(INTDIR)\Name_Handler.obj" : $(SOURCE) $(DEP_CPP_NAME_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Client_Logging_Handler.cpp
DEP_CPP_CLIEN=\
	"..\..\ace/Acceptor.cpp"\
	"..\..\ace/Acceptor.h"\
	"..\..\ace/Acceptor.i"\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Addr.h"\
	"..\..\ace/Addr.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Containers.cpp"\
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Dynamic.h"\
	"..\..\ace/Dynamic.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/INET_Addr.h"\
	"..\..\ace/INET_Addr.i"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/IPC_SAP.h"\
	"..\..\ace/IPC_SAP.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc.h"\
	"..\..\ace/Malloc.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Malloc_T.cpp"\
	"..\..\ace/Malloc_T.h"\
	"..\..\ace/Malloc_T.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Mem_Map.h"\
	"..\..\ace/Mem_Map.i"\
	"..\..\ace/Memory_Pool.h"\
	"..\..\ace/Memory_Pool.i"\
	"..\..\ace/Message_Block.h"\
	"..\..\ace/Message_Block.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Reactor.h"\
	"..\..\ace/Reactor.i"\
	"..\..\ace/Reactor_Impl.h"\
	"..\..\ace/Service_Config.h"\
	"..\..\ace/Service_Config.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Service_Repository.h"\
	"..\..\ace/Service_Repository.i"\
	"..\..\ace/Service_Types.h"\
	"..\..\ace/Service_Types.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/Signal.h"\
	"..\..\ace/Signal.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SOCK.h"\
	"..\..\ace/SOCK.i"\
	"..\..\ace/SOCK_Acceptor.h"\
	"..\..\ace/SOCK_Acceptor.i"\
	"..\..\ace/SOCK_Connector.h"\
	"..\..\ace/SOCK_Connector.i"\
	"..\..\ace/SOCK_IO.h"\
	"..\..\ace/SOCK_IO.i"\
	"..\..\ace/SOCK_Stream.h"\
	"..\..\ace/SOCK_Stream.i"\
	"..\..\ace/SPIPE.h"\
	"..\..\ace/SPIPE.i"\
	"..\..\ace/SPIPE_Acceptor.h"\
	"..\..\ace/SPIPE_Addr.h"\
	"..\..\ace/SPIPE_Addr.i"\
	"..\..\ace/SPIPE_Stream.h"\
	"..\..\ace/SPIPE_Stream.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/Strategies.h"\
	"..\..\ace/Strategies.i"\
	"..\..\ace/Strategies_T.cpp"\
	"..\..\ace/Strategies_T.h"\
	"..\..\ace/Strategies_T.i"\
	"..\..\ace/Stream_Modules.cpp"\
	"..\..\ace/Stream_Modules.h"\
	"..\..\ace/Stream_Modules.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Svc_Conf_Tokens.h"\
	"..\..\ace/Svc_Handler.cpp"\
	"..\..\ace/Svc_Handler.h"\
	"..\..\ace/Svc_Handler.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Time_Value.h"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\Client_Logging_Handler.h"\
	

"$(INTDIR)\Client_Logging_Handler.obj" : $(SOURCE) $(DEP_CPP_CLIEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Logging_Strategy.cpp
DEP_CPP_LOGGI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-borland.h"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config-WinCE.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Get_Opt.h"\
	"..\..\ace/Get_Opt.i"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Malloc_Base.h"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Service_Object.h"\
	"..\..\ace/Service_Object.i"\
	"..\..\ace/Shared_Object.h"\
	"..\..\ace/Shared_Object.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/ws2tcpip.h"\
	".\Logging_Strategy.h"\
	

"$(INTDIR)\Logging_Strategy.obj" : $(SOURCE) $(DEP_CPP_LOGGI) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
