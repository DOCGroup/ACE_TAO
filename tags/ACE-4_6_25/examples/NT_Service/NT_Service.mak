# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=NT_Service - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to NT_Service - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "NT_Service - Win32 Release" && "$(CFG)" !=\
 "NT_Service - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "NT_Service.mak" CFG="NT_Service - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NT_Service - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "NT_Service - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "NT_Service - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NT_Service - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\NT_Service.exe"

CLEAN : 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\ntsvc.obj"
	-@erase "$(OUTDIR)\NT_Service.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D\
 "_CONSOLE" /Fp"$(INTDIR)/NT_Service.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/NT_Service.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ..\..\ace\ace.lib user32.lib advapi32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=..\..\ace\ace.lib user32.lib advapi32.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/NT_Service.pdb"\
 /machine:I386 /out:"$(OUTDIR)/NT_Service.exe" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\ntsvc.obj"

"$(OUTDIR)\NT_Service.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NT_Service - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\NT_Service.exe"

CLEAN : 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\ntsvc.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\NT_Service.exe"
	-@erase "$(OUTDIR)\NT_Service.ilk"
	-@erase "$(OUTDIR)\NT_Service.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/NT_Service.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\..\ace\aced.lib user32.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\..\ace\aced.lib user32.lib advapi32.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/NT_Service.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/NT_Service.exe" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\ntsvc.obj"

"$(OUTDIR)\NT_Service.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "NT_Service - Win32 Release"
# Name "NT_Service - Win32 Debug"

!IF  "$(CFG)" == "NT_Service - Win32 Release"

!ELSEIF  "$(CFG)" == "NT_Service - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ntsvc.cpp
DEP_CPP_NTSVC=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
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
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Containers_T.cpp"\
	"..\..\ace/Containers_T.h"\
	"..\..\ace/Containers_T.i"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Free_List.cpp"\
	"..\..\ace/Free_List.h"\
	"..\..\ace/Free_List.i"\
	"..\..\ace/Handle_Set.h"\
	"..\..\ace/Handle_Set.i"\
	"..\..\ace/Hash_Map_Manager.cpp"\
	"..\..\ace/Hash_Map_Manager.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
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
	"..\..\ace/Message_Block_T.cpp"\
	"..\..\ace/Message_Block_T.h"\
	"..\..\ace/Message_Block_T.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/NT_Service.h"\
	"..\..\ace/NT_Service.i"\
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
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\ntsvc.h"\
	

"$(INTDIR)\ntsvc.obj" : $(SOURCE) $(DEP_CPP_NTSVC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\main.cpp
DEP_CPP_MAIN_=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
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
	"..\..\ace/Containers.h"\
	"..\..\ace/Containers.i"\
	"..\..\ace/Containers_T.cpp"\
	"..\..\ace/Containers_T.h"\
	"..\..\ace/Containers_T.i"\
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
	"..\..\ace/IO_Cntl_Msg.h"\
	"..\..\ace/iosfwd.h"\
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
	"..\..\ace/Message_Block_T.cpp"\
	"..\..\ace/Message_Block_T.h"\
	"..\..\ace/Message_Block_T.i"\
	"..\..\ace/Message_Queue.h"\
	"..\..\ace/Message_Queue.i"\
	"..\..\ace/Message_Queue_T.cpp"\
	"..\..\ace/Message_Queue_T.h"\
	"..\..\ace/Message_Queue_T.i"\
	"..\..\ace/Module.cpp"\
	"..\..\ace/Module.h"\
	"..\..\ace/Module.i"\
	"..\..\ace/NT_Service.h"\
	"..\..\ace/NT_Service.i"\
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
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_Options.h"\
	"..\..\ace/Synch_Options.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Task.h"\
	"..\..\ace/Task.i"\
	"..\..\ace/Task_T.cpp"\
	"..\..\ace/Task_T.h"\
	"..\..\ace/Task_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Thread_Manager.h"\
	"..\..\ace/Thread_Manager.i"\
	"..\..\ace/Timer_Queue.h"\
	"..\..\ace/Timer_Queue_T.cpp"\
	"..\..\ace/Timer_Queue_T.h"\
	"..\..\ace/Timer_Queue_T.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/WFMO_Reactor.h"\
	"..\..\ace/WFMO_Reactor.i"\
	"..\..\ace/ws2tcpip.h"\
	".\ntsvc.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
