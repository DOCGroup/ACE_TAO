# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=MT_Reactor_Timer_Test - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MT_Reactor_Timer_Test -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Handle_Set_Test - Win32 Debug" && "$(CFG)" !=\
 "Mem_Map_Test - Win32 Debug" && "$(CFG)" != "Naming_Test - Win32 Debug" &&\
 "$(CFG)" != "Reactor_Timer_Test - Win32 Debug" && "$(CFG)" !=\
 "Reactors_Test - Win32 Debug" && "$(CFG)" != "SString_Test - Win32 Debug" &&\
 "$(CFG)" != "Time_Value_Test - Win32 Debug" && "$(CFG)" !=\
 "Timer_Queue_Test - Win32 Debug" && "$(CFG)" != "UPIPE_SAP_Test - Win32 Debug"\
 && "$(CFG)" != "Priority_Buffer_Test - Win32 Debug" && "$(CFG)" !=\
 "Time_Service_Test - Win32 Debug" && "$(CFG)" != "SPIPE_Test - Win32 Debug" &&\
 "$(CFG)" != "Buffer_Stream_Test - Win32 Debug" && "$(CFG)" !=\
 "Barrier_Test - Win32 Debug" && "$(CFG)" != "Reader_Writer_Test - Win32 Debug"\
 && "$(CFG)" != "Recursive_Mutex_Test - Win32 Debug" && "$(CFG)" !=\
 "Task_Test - Win32 Debug" && "$(CFG)" != "Thread_Manager_Test - Win32 Debug" &&\
 "$(CFG)" != "TSS_Test - Win32 Debug" && "$(CFG)" !=\
 "Thread_Pool_Test - Win32 Debug" && "$(CFG)" != "Future_Test - Win32 Debug" &&\
 "$(CFG)" != "Tokens_Test - Win32 Debug" && "$(CFG)" !=\
 "Message_Queue_Test - Win32 Debug" && "$(CFG)" !=\
 "Map_Manager_Test - Win32 Debug" && "$(CFG)" != "Pipe_Test - Win32 Debug" &&\
 "$(CFG)" != "Process_Mutex_Test - Win32 Debug" && "$(CFG)" !=\
 "SV_Shared_Memory_Test - Win32 Debug" && "$(CFG)" !=\
 "MM_Shared_Memory_Test - Win32 Debug" && "$(CFG)" !=\
 "Service_Config_Test - Win32 Debug" && "$(CFG)" != "SOCK_Test - Win32 Debug" &&\
 "$(CFG)" != "Message_Block_Test - Win32 Debug" && "$(CFG)" !=\
 "Reactor_Notify_Test - Win32 Debug" && "$(CFG)" !=\
 "IOStream_Test - Win32 Debug" && "$(CFG)" !=\
 "Process_Strategy_Test - Win32 Debug" && "$(CFG)" != "Conn_Test - Win32 Debug"\
 && "$(CFG)" != "Simple_Message_Block_Test - Win32 Debug" && "$(CFG)" !=\
 "Reactor_Exceptions_Test - Win32 Debug" && "$(CFG)" !=\
 "Priority_Task_Test - Win32 Debug" && "$(CFG)" !=\
 "Sigset_Ops_Test - Win32 Debug" && "$(CFG)" !=\
 "Enum_Interfaces_Test - Win32 Debug" && "$(CFG)" !=\
 "Hash_Map_Manager_Test - Win32 Debug" && "$(CFG)" !=\
 "Semaphore_Test - Win32 Debug" && "$(CFG)" != "Atomic_Op_Test - Win32 Debug" &&\
 "$(CFG)" != "MT_SOCK_Test - Win32 Debug" && "$(CFG)" !=\
 "Priority_Reactor_Test - Win32 Debug" && "$(CFG)" !=\
 "Reactor_Performance_Test - Win32 Debug" && "$(CFG)" !=\
 "Notify_Performance_Test - Win32 Debug" && "$(CFG)" !=\
 "Thread_Mutex_Test - Win32 Debug" && "$(CFG)" !=\
 "SOCK_Connector_Test - Win32 Debug" && "$(CFG)" !=\
 "Basic_Types_Test - Win32 Debug" && "$(CFG)" != "Env_Value_Test - Win32 Debug"\
 && "$(CFG)" != "OrdMultiSet_Test - Win32 Debug" && "$(CFG)" !=\
 "Timeprobe_Test - Win32 Debug" && "$(CFG)" !=\
 "Message_Queue_Notifications_Test - Win32 Debug" && "$(CFG)" !=\
 "DLList_Test - Win32 Debug" && "$(CFG)" != "High_Res_Timer_Test - Win32 Debug"\
 && "$(CFG)" != "Dynamic_Priority_Test - Win32 Debug" && "$(CFG)" !=\
 "Thread_Pool_Reactor_Test - Win32 Debug" && "$(CFG)" !=\
 "Upgradable_RW_Test - Win32 Debug" && "$(CFG)" !=\
 "MT_Reactor_Timer_Test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "tests.mak" CFG="MT_Reactor_Timer_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Handle_Set_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Mem_Map_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Naming_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactor_Timer_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactors_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SString_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Time_Value_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Timer_Queue_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "UPIPE_SAP_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Buffer_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Time_Service_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SPIPE_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Buffer_Stream_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Barrier_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reader_Writer_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Recursive_Mutex_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Task_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Thread_Manager_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "TSS_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Thread_Pool_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Future_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Tokens_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Map_Manager_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Pipe_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Process_Mutex_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SV_Shared_Memory_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MM_Shared_Memory_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Service_Config_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SOCK_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Message_Block_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactor_Notify_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "IOStream_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Process_Strategy_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Conn_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Simple_Message_Block_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactor_Exceptions_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Task_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Sigset_Ops_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Enum_Interfaces_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Hash_Map_Manager_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Semaphore_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Atomic_Op_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MT_SOCK_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactor_Performance_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Notify_Performance_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Thread_Mutex_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SOCK_Connector_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Env_Value_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "OrdMultiSet_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Timeprobe_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "DLList_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "High_Res_Timer_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Dynamic_Priority_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Thread_Pool_Reactor_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Upgradable_RW_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "MT_Reactor_Timer_Test - Win32 Debug" (based on\
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
# PROP Target_Last_Scanned "MT_Reactor_Timer_Test - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Handle_Set_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Handle_Set_Test\Debug"
# PROP BASE Intermediate_Dir "Handle_Set_Test\Debug"
# PROP BASE Target_Dir "Handle_Set_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Handle_Set_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Handle_Set_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Handle_Set_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Handle_Set_Test.exe"
	-@erase "$(OUTDIR)\Handle_Set_Test.ilk"
	-@erase "$(OUTDIR)\Handle_Set_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Handle_Set_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Handle_Set_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Handle_Set_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Handle_Set_Test.obj"

"$(OUTDIR)\Handle_Set_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mem_Map_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mem_Map_Test\Debug"
# PROP BASE Intermediate_Dir "Mem_Map_Test\Debug"
# PROP BASE Target_Dir "Mem_Map_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Mem_Map_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Mem_Map_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Mem_Map_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Mem_Map_Test.exe"
	-@erase "$(OUTDIR)\Mem_Map_Test.ilk"
	-@erase "$(OUTDIR)\Mem_Map_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Mem_Map_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Mem_Map_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Mem_Map_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Mem_Map_Test.obj"

"$(OUTDIR)\Mem_Map_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Naming_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Naming_Test\Debug"
# PROP BASE Intermediate_Dir "Naming_Test\Debug"
# PROP BASE Target_Dir "Naming_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Naming_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Naming_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Naming_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Naming_Test.exe"
	-@erase "$(OUTDIR)\Naming_Test.ilk"
	-@erase "$(OUTDIR)\Naming_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Naming_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Naming_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Naming_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Naming_Test.obj"

"$(OUTDIR)\Naming_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactor_Timer_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reactor_Timer_Test\Debug"
# PROP BASE Intermediate_Dir "Reactor_Timer_Test\Debug"
# PROP BASE Target_Dir "Reactor_Timer_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reactor_Timer_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reactor_Timer_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reactor_Timer_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reactor_Timer_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Timer_Test.ilk"
	-@erase "$(OUTDIR)\Reactor_Timer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactor_Timer_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactor_Timer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactor_Timer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Timer_Test.obj"

"$(OUTDIR)\Reactor_Timer_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactors_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reactors_Test\Debug"
# PROP BASE Intermediate_Dir "Reactors_Test\Debug"
# PROP BASE Target_Dir "Reactors_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reactors_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reactors_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reactors_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reactors_Test.exe"
	-@erase "$(OUTDIR)\Reactors_Test.ilk"
	-@erase "$(OUTDIR)\Reactors_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactors_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactors_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactors_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reactors_Test.obj"

"$(OUTDIR)\Reactors_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SString_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SString_Test\Debug"
# PROP BASE Intermediate_Dir "SString_Test\Debug"
# PROP BASE Target_Dir "SString_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "SString_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\SString_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\SString_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SString_Test.exe"
	-@erase "$(OUTDIR)\SString_Test.ilk"
	-@erase "$(OUTDIR)\SString_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SString_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SString_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SString_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SString_Test.obj"

"$(OUTDIR)\SString_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Time_Value_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Time_Value_Test\Debug"
# PROP BASE Intermediate_Dir "Time_Value_Test\Debug"
# PROP BASE Target_Dir "Time_Value_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Time_Value_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Time_Value_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Time_Value_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Time_Value_Test.exe"
	-@erase "$(OUTDIR)\Time_Value_Test.ilk"
	-@erase "$(OUTDIR)\Time_Value_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Time_Value_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Time_Value_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Time_Value_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Time_Value_Test.obj"

"$(OUTDIR)\Time_Value_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Timer_Queue_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Timer_Queue_Test\Debug"
# PROP BASE Intermediate_Dir "Timer_Queue_Test\Debug"
# PROP BASE Target_Dir "Timer_Queue_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Timer_Queue_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Timer_Queue_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Timer_Queue_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Timer_Queue_Test.exe"
	-@erase "$(OUTDIR)\Timer_Queue_Test.ilk"
	-@erase "$(OUTDIR)\Timer_Queue_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Timer_Queue_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Timer_Queue_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Timer_Queue_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Timer_Queue_Test.obj"

"$(OUTDIR)\Timer_Queue_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UPIPE_SAP_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UPIPE_SAP_Test\Debug"
# PROP BASE Intermediate_Dir "UPIPE_SAP_Test\Debug"
# PROP BASE Target_Dir "UPIPE_SAP_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "UPIPE_SAP_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\UPIPE_SAP_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\UPIPE_SAP_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\UPIPE_SAP_Test.exe"
	-@erase "$(OUTDIR)\UPIPE_SAP_Test.ilk"
	-@erase "$(OUTDIR)\UPIPE_SAP_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/UPIPE_SAP_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/UPIPE_SAP_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/UPIPE_SAP_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\UPIPE_SAP_Test.obj"

"$(OUTDIR)\UPIPE_SAP_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Priority_Buffer_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority_Buffer_Test\Debug"
# PROP BASE Intermediate_Dir "Priority_Buffer_Test\Debug"
# PROP BASE Target_Dir "Priority_Buffer_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Priority_Buffer_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Priority_Buffer_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Priority_Buffer_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Priority_Buffer_Test.exe"
	-@erase "$(OUTDIR)\Priority_Buffer_Test.ilk"
	-@erase "$(OUTDIR)\Priority_Buffer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Priority_Buffer_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Priority_Buffer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Priority_Buffer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Priority_Buffer_Test.obj"

"$(OUTDIR)\Priority_Buffer_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Time_Service_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Time_Service_Test\Debug"
# PROP BASE Intermediate_Dir "Time_Service_Test\Debug"
# PROP BASE Target_Dir "Time_Service_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Time_Service_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Time_Service_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Time_Service_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Time_Service_Test.exe"
	-@erase "$(OUTDIR)\Time_Service_Test.ilk"
	-@erase "$(OUTDIR)\Time_Service_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Time_Service_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Time_Service_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Time_Service_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Time_Service_Test.obj"

"$(OUTDIR)\Time_Service_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SPIPE_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SPIPE_Test\SPIPE_Te"
# PROP BASE Intermediate_Dir "SPIPE_Test\SPIPE_Te"
# PROP BASE Target_Dir "SPIPE_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "SPIPE_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\SPIPE_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\SPIPE_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SPIPE_Test.exe"
	-@erase "$(OUTDIR)\SPIPE_Test.ilk"
	-@erase "$(OUTDIR)\SPIPE_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SPIPE_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SPIPE_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SPIPE_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SPIPE_Test.obj"

"$(OUTDIR)\SPIPE_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Buffer_Stream_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Buffer_Stream_Test\Debug"
# PROP BASE Intermediate_Dir "Buffer_Stream_Test\Debug"
# PROP BASE Target_Dir "Buffer_Stream_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Buffer_Stream_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Buffer_Stream_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Buffer_Stream_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Buffer_Stream_Test.exe"
	-@erase "$(OUTDIR)\Buffer_Stream_Test.ilk"
	-@erase "$(OUTDIR)\Buffer_Stream_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Buffer_Stream_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Buffer_Stream_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Buffer_Stream_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Buffer_Stream_Test.obj"

"$(OUTDIR)\Buffer_Stream_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Barrier_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Barrier_Test\Debug"
# PROP BASE Intermediate_Dir "Barrier_Test\Debug"
# PROP BASE Target_Dir "Barrier_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Barrier_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Barrier_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Barrier_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Barrier_Test.exe"
	-@erase "$(OUTDIR)\Barrier_Test.ilk"
	-@erase "$(OUTDIR)\Barrier_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Barrier_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Barrier_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Barrier_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Barrier_Test.obj"

"$(OUTDIR)\Barrier_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reader_Writer_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reader_Writer_Test\Debug"
# PROP BASE Intermediate_Dir "Reader_Writer_Test\Debug"
# PROP BASE Target_Dir "Reader_Writer_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reader_Writer_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reader_Writer_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reader_Writer_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reader_Writer_Test.exe"
	-@erase "$(OUTDIR)\Reader_Writer_Test.ilk"
	-@erase "$(OUTDIR)\Reader_Writer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reader_Writer_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reader_Writer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reader_Writer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reader_Writer_Test.obj"

"$(OUTDIR)\Reader_Writer_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Recursive_Mutex_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Recursive_Mutex_Test\Debug"
# PROP BASE Intermediate_Dir "Recursive_Mutex_Test\Debug"
# PROP BASE Target_Dir "Recursive_Mutex_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Recursive_Mutex_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Recursive_Mutex_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Recursive_Mutex_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Recursive_Mutex_Test.exe"
	-@erase "$(OUTDIR)\Recursive_Mutex_Test.ilk"
	-@erase "$(OUTDIR)\Recursive_Mutex_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Recursive_Mutex_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Recursive_Mutex_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Recursive_Mutex_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Recursive_Mutex_Test.obj"

"$(OUTDIR)\Recursive_Mutex_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Task_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Task_Test\Debug"
# PROP BASE Intermediate_Dir "Task_Test\Debug"
# PROP BASE Target_Dir "Task_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Task_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Task_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Task_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Task_Test.exe"
	-@erase "$(OUTDIR)\Task_Test.ilk"
	-@erase "$(OUTDIR)\Task_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Task_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Task_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Task_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Task_Test.obj"

"$(OUTDIR)\Task_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Thread_Manager_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Thread_Manager_Test\Debug"
# PROP BASE Intermediate_Dir "Thread_Manager_Test\Debug"
# PROP BASE Target_Dir "Thread_Manager_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Thread_Manager_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Thread_Manager_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Thread_Manager_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Thread_Manager_Test.exe"
	-@erase "$(OUTDIR)\Thread_Manager_Test.ilk"
	-@erase "$(OUTDIR)\Thread_Manager_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Manager_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Manager_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Manager_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Thread_Manager_Test.obj"

"$(OUTDIR)\Thread_Manager_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TSS_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TSS_Test\Debug"
# PROP BASE Intermediate_Dir "TSS_Test\Debug"
# PROP BASE Target_Dir "TSS_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "TSS_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\TSS_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\TSS_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\TSS_Test.exe"
	-@erase "$(OUTDIR)\TSS_Test.ilk"
	-@erase "$(OUTDIR)\TSS_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TSS_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/TSS_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/TSS_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TSS_Test.obj"

"$(OUTDIR)\TSS_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Thread_Pool_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Thread_Pool_Test\Debug"
# PROP BASE Intermediate_Dir "Thread_Pool_Test\Debug"
# PROP BASE Target_Dir "Thread_Pool_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Thread_Pool_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Thread_Pool_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Thread_Pool_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Thread_Pool_Test.exe"
	-@erase "$(OUTDIR)\Thread_Pool_Test.ilk"
	-@erase "$(OUTDIR)\Thread_Pool_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Pool_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Pool_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Pool_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Thread_Pool_Test.obj"

"$(OUTDIR)\Thread_Pool_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Future_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Future_Test\Debug"
# PROP BASE Intermediate_Dir "Future_Test\Debug"
# PROP BASE Target_Dir "Future_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Future_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Future_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Future_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Future_Test.exe"
	-@erase "$(OUTDIR)\Future_Test.ilk"
	-@erase "$(OUTDIR)\Future_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Future_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Future_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Future_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Future_Test.obj"

"$(OUTDIR)\Future_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tokens_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tokens_Test\Debug"
# PROP BASE Intermediate_Dir "Tokens_Test\Debug"
# PROP BASE Target_Dir "Tokens_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Tokens_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Tokens_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Tokens_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Tokens_Test.exe"
	-@erase "$(OUTDIR)\Tokens_Test.ilk"
	-@erase "$(OUTDIR)\Tokens_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Tokens_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Tokens_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Tokens_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Tokens_Test.obj"

"$(OUTDIR)\Tokens_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Message_Queue_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_Queue_Test\Debug"
# PROP BASE Intermediate_Dir "Message_Queue_Test\Debug"
# PROP BASE Target_Dir "Message_Queue_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Message_Queue_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Message_Queue_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Message_Queue_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Message_Queue_Test.exe"
	-@erase "$(OUTDIR)\Message_Queue_Test.ilk"
	-@erase "$(OUTDIR)\Message_Queue_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Message_Queue_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Message_Queue_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Message_Queue_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Message_Queue_Test.obj"

"$(OUTDIR)\Message_Queue_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Map_Manager_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Map_Manager_Test\Debug"
# PROP BASE Intermediate_Dir "Map_Manager_Test\Debug"
# PROP BASE Target_Dir "Map_Manager_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Map_Manager_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Map_Manager_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Map_Manager_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Map_Manager_Test.exe"
	-@erase "$(OUTDIR)\Map_Manager_Test.ilk"
	-@erase "$(OUTDIR)\Map_Manager_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Map_Manager_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Map_Manager_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Map_Manager_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Map_Manager_Test.obj"

"$(OUTDIR)\Map_Manager_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pipe_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Pipe_Test\Debug"
# PROP BASE Intermediate_Dir "Pipe_Test\Debug"
# PROP BASE Target_Dir "Pipe_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Pipe_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Pipe_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Pipe_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Pipe_Test.exe"
	-@erase "$(OUTDIR)\Pipe_Test.ilk"
	-@erase "$(OUTDIR)\Pipe_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pipe_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Pipe_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Pipe_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Pipe_Test.obj"

"$(OUTDIR)\Pipe_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Process_Mutex_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Process_Mutex_Test\Debug"
# PROP BASE Intermediate_Dir "Process_Mutex_Test\Debug"
# PROP BASE Target_Dir "Process_Mutex_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Process_Mutex_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Process_Mutex_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Process_Mutex_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Process_Mutex_Test.exe"
	-@erase "$(OUTDIR)\Process_Mutex_Test.ilk"
	-@erase "$(OUTDIR)\Process_Mutex_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Process_Mutex_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Process_Mutex_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Process_Mutex_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Process_Mutex_Test.obj"

"$(OUTDIR)\Process_Mutex_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SV_Shared_Memory_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SV_Shared_Memory_Test\Debug"
# PROP BASE Intermediate_Dir "SV_Shared_Memory_Test\Debug"
# PROP BASE Target_Dir "SV_Shared_Memory_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "SV_Shared_Memory_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\SV_Shared_Memory_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\SV_Shared_Memory_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SV_Shared_Memory_Test.exe"
	-@erase "$(OUTDIR)\SV_Shared_Memory_Test.ilk"
	-@erase "$(OUTDIR)\SV_Shared_Memory_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SV_Shared_Memory_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SV_Shared_Memory_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SV_Shared_Memory_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SV_Shared_Memory_Test.obj"

"$(OUTDIR)\SV_Shared_Memory_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MM_Shared_Memory_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MM_Shared_Memory_Test\Debug"
# PROP BASE Intermediate_Dir "MM_Shared_Memory_Test\Debug"
# PROP BASE Target_Dir "MM_Shared_Memory_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "MM_Shared_Memory_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\MM_Shared_Memory_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\MM_Shared_Memory_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MM_Shared_Memory_Test.exe"
	-@erase "$(OUTDIR)\MM_Shared_Memory_Test.ilk"
	-@erase "$(OUTDIR)\MM_Shared_Memory_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MM_Shared_Memory_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/MM_Shared_Memory_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MM_Shared_Memory_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MM_Shared_Memory_Test.obj"

"$(OUTDIR)\MM_Shared_Memory_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Service_Config_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Service_Config_Test\Debug"
# PROP BASE Intermediate_Dir "Service_Config_Test\Debug"
# PROP BASE Target_Dir "Service_Config_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Service_Config_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Service_Config_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Service_Config_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Service_Config_Test.exe"
	-@erase "$(OUTDIR)\Service_Config_Test.ilk"
	-@erase "$(OUTDIR)\Service_Config_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Service_Config_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Service_Config_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Service_Config_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Service_Config_Test.obj"

"$(OUTDIR)\Service_Config_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SOCK_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SOCK_Test\Debug"
# PROP BASE Intermediate_Dir "SOCK_Test\Debug"
# PROP BASE Target_Dir "SOCK_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "SOCK_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\SOCK_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\SOCK_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SOCK_Test.exe"
	-@erase "$(OUTDIR)\SOCK_Test.ilk"
	-@erase "$(OUTDIR)\SOCK_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SOCK_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SOCK_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SOCK_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SOCK_Test.obj"

"$(OUTDIR)\SOCK_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Message_Block_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_Block_Test\Debug"
# PROP BASE Intermediate_Dir "Message_Block_Test\Debug"
# PROP BASE Target_Dir "Message_Block_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Message_Block_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Message_Block_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Message_Block_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Message_Block_Test.exe"
	-@erase "$(OUTDIR)\Message_Block_Test.ilk"
	-@erase "$(OUTDIR)\Message_Block_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Message_Block_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Message_Block_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Message_Block_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Message_Block_Test.obj"

"$(OUTDIR)\Message_Block_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactor_Notify_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reactor_Notify_Test\Debug"
# PROP BASE Intermediate_Dir "Reactor_Notify_Test\Debug"
# PROP BASE Target_Dir "Reactor_Notify_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reactor_Notify_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reactor_Notify_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reactor_Notify_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reactor_Notify_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Notify_Test.ilk"
	-@erase "$(OUTDIR)\Reactor_Notify_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactor_Notify_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactor_Notify_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactor_Notify_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Notify_Test.obj"

"$(OUTDIR)\Reactor_Notify_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IOStream_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IOStream_Test\Debug"
# PROP BASE Intermediate_Dir "IOStream_Test\Debug"
# PROP BASE Target_Dir "IOStream_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "IOStream_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\IOStream_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\IOStream_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\IOStream_Test.exe"
	-@erase "$(OUTDIR)\IOStream_Test.ilk"
	-@erase "$(OUTDIR)\IOStream_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IOStream_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/IOStream_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/IOStream_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\IOStream_Test.obj"

"$(OUTDIR)\IOStream_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Process_Strategy_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Process_Strategy_Test\Debug"
# PROP BASE Intermediate_Dir "Process_Strategy_Test\Debug"
# PROP BASE Target_Dir "Process_Strategy_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Process_Strategy_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Process_Strategy_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Process_Strategy_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Process_Strategy_Test.exe"
	-@erase "$(OUTDIR)\Process_Strategy_Test.ilk"
	-@erase "$(OUTDIR)\Process_Strategy_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Process_Strategy_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Process_Strategy_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Process_Strategy_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Process_Strategy_Test.obj"

"$(OUTDIR)\Process_Strategy_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Conn_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Conn_Test\Debug"
# PROP BASE Intermediate_Dir "Conn_Test\Debug"
# PROP BASE Target_Dir "Conn_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Conn_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Conn_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Conn_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Conn_Test.exe"
	-@erase "$(OUTDIR)\Conn_Test.ilk"
	-@erase "$(OUTDIR)\Conn_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Conn_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Conn_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Conn_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Conn_Test.obj"

"$(OUTDIR)\Conn_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Simple_Message_Block_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Simple_Message_Block_Test\Debug"
# PROP BASE Intermediate_Dir "Simple_Message_Block_Test\Debug"
# PROP BASE Target_Dir "Simple_Message_Block_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Simple_Message_Block_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Simple_Message_Block_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Simple_Message_Block_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Simple_Message_Block_Test.exe"
	-@erase "$(OUTDIR)\Simple_Message_Block_Test.ilk"
	-@erase "$(OUTDIR)\Simple_Message_Block_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Simple_Message_Block_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Simple_Message_Block_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Simple_Message_Block_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Simple_Message_Block_Test.obj"

"$(OUTDIR)\Simple_Message_Block_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactor_Exceptions_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reactor_Exceptions_Test\Debug"
# PROP BASE Intermediate_Dir "Reactor_Exceptions_Test\Debug"
# PROP BASE Target_Dir "Reactor_Exceptions_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reactor_Exceptions_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reactor_Exceptions_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reactor_Exceptions_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reactor_Exceptions_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Exceptions_Test.ilk"
	-@erase "$(OUTDIR)\Reactor_Exceptions_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactor_Exceptions_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactor_Exceptions_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactor_Exceptions_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Exceptions_Test.obj"

"$(OUTDIR)\Reactor_Exceptions_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Priority_Task_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority_Task_Test\Debug"
# PROP BASE Intermediate_Dir "Priority_Task_Test\Debug"
# PROP BASE Target_Dir "Priority_Task_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Priority_Task_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Priority_Task_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Priority_Task_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Priority_Task_Test.exe"
	-@erase "$(OUTDIR)\Priority_Task_Test.ilk"
	-@erase "$(OUTDIR)\Priority_Task_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Priority_Task_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Priority_Task_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Priority_Task_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Priority_Task_Test.obj"

"$(OUTDIR)\Priority_Task_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Sigset_Ops_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Sigset_Ops_Test\Debug"
# PROP BASE Intermediate_Dir "Sigset_Ops_Test\Debug"
# PROP BASE Target_Dir "Sigset_Ops_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Sigset_Ops_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Sigset_Ops_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Sigset_Ops_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Sigset_Ops_Test.exe"
	-@erase "$(OUTDIR)\Sigset_Ops_Test.ilk"
	-@erase "$(OUTDIR)\Sigset_Ops_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Sigset_Ops_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Sigset_Ops_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Sigset_Ops_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Sigset_Ops_Test.obj"

"$(OUTDIR)\Sigset_Ops_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Enum_Interfaces_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Enum_Interfaces_Test\Debug"
# PROP BASE Intermediate_Dir "Enum_Interfaces_Test\Debug"
# PROP BASE Target_Dir "Enum_Interfaces_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Enum_Interfaces_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Enum_Interfaces_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Enum_Interfaces_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Enum_Interfaces_Test.exe"
	-@erase "$(OUTDIR)\Enum_Interfaces_Test.ilk"
	-@erase "$(OUTDIR)\Enum_Interfaces_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Enum_Interfaces_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Enum_Interfaces_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Enum_Interfaces_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Enum_Interfaces_Test.obj"

"$(OUTDIR)\Enum_Interfaces_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Hash_Map_Manager_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Hash_Map_Manager_Test\Debug"
# PROP BASE Intermediate_Dir "Hash_Map_Manager_Test\Debug"
# PROP BASE Target_Dir "Hash_Map_Manager_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Hash_Map_Manager_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Hash_Map_Manager_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Hash_Map_Manager_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Hash_Map_Manager_Test.exe"
	-@erase "$(OUTDIR)\Hash_Map_Manager_Test.ilk"
	-@erase "$(OUTDIR)\Hash_Map_Manager_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Hash_Map_Manager_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Hash_Map_Manager_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Hash_Map_Manager_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Hash_Map_Manager_Test.obj"

"$(OUTDIR)\Hash_Map_Manager_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Semaphore_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Semaphore_Test\Debug"
# PROP BASE Intermediate_Dir "Semaphore_Test\Debug"
# PROP BASE Target_Dir "Semaphore_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Semaphore_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Semaphore_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Semaphore_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Semaphore_Test.exe"
	-@erase "$(OUTDIR)\Semaphore_Test.ilk"
	-@erase "$(OUTDIR)\Semaphore_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Semaphore_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Semaphore_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Semaphore_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Semaphore_Test.obj"

"$(OUTDIR)\Semaphore_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Atomic_Op_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Atomic_Op_Test\Debug"
# PROP BASE Intermediate_Dir "Atomic_Op_Test\Debug"
# PROP BASE Target_Dir "Atomic_Op_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Atomic_Op_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Atomic_Op_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Atomic_Op_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Atomic_Op_Test.exe"
	-@erase "$(OUTDIR)\Atomic_Op_Test.ilk"
	-@erase "$(OUTDIR)\Atomic_Op_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Atomic_Op_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Atomic_Op_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Atomic_Op_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Atomic_Op_Test.obj"

"$(OUTDIR)\Atomic_Op_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MT_SOCK_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MT_SOCK_Test\Debug"
# PROP BASE Intermediate_Dir "MT_SOCK_Test\Debug"
# PROP BASE Target_Dir "MT_SOCK_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "MT_SOCK_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\MT_SOCK_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\MT_SOCK_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MT_SOCK_Test.exe"
	-@erase "$(OUTDIR)\MT_SOCK_Test.ilk"
	-@erase "$(OUTDIR)\MT_SOCK_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MT_SOCK_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/MT_SOCK_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MT_SOCK_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MT_SOCK_Test.obj"

"$(OUTDIR)\MT_SOCK_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority_Reactor_Test\Debug"
# PROP BASE Intermediate_Dir "Priority_Reactor_Test\Debug"
# PROP BASE Target_Dir "Priority_Reactor_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Priority_Reactor_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Priority_Reactor_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Priority_Reactor_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Priority_Reactor_Test.exe"
	-@erase "$(OUTDIR)\Priority_Reactor_Test.ilk"
	-@erase "$(OUTDIR)\Priority_Reactor_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Priority_Reactor_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Priority_Reactor_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Priority_Reactor_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Priority_Reactor_Test.obj"

"$(OUTDIR)\Priority_Reactor_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactor_Performance_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Reactor_Performance_Test\Debug"
# PROP BASE Intermediate_Dir "Reactor_Performance_Test\Debug"
# PROP BASE Target_Dir "Reactor_Performance_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Reactor_Performance_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Reactor_Performance_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Reactor_Performance_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.ilk"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactor_Performance_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactor_Performance_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactor_Performance_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Performance_Test.obj"

"$(OUTDIR)\Reactor_Performance_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Notify_Performance_Test\Debug"
# PROP BASE Intermediate_Dir "Notify_Performance_Test\Debug"
# PROP BASE Target_Dir "Notify_Performance_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Notify_Performance_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Notify_Performance_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Notify_Performance_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Notify_Performance_Test.exe"
	-@erase "$(OUTDIR)\Notify_Performance_Test.ilk"
	-@erase "$(OUTDIR)\Notify_Performance_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Notify_Performance_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Notify_Performance_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Notify_Performance_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Notify_Performance_Test.obj"

"$(OUTDIR)\Notify_Performance_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Thread_Mutex_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Thread_Mutex_Test\Debug"
# PROP BASE Intermediate_Dir "Thread_Mutex_Test\Debug"
# PROP BASE Target_Dir "Thread_Mutex_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Thread_Mutex_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Thread_Mutex_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Thread_Mutex_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Thread_Mutex_Test.exe"
	-@erase "$(OUTDIR)\Thread_Mutex_Test.ilk"
	-@erase "$(OUTDIR)\Thread_Mutex_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Mutex_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Mutex_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Mutex_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Thread_Mutex_Test.obj"

"$(OUTDIR)\Thread_Mutex_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SOCK_Connector_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SOCK_Connector_Test\Debug"
# PROP BASE Intermediate_Dir "SOCK_Connector_Test\Debug"
# PROP BASE Target_Dir "SOCK_Connector_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "SOCK_Connector_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\SOCK_Connector_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\SOCK_Connector_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.exe"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.ilk"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SOCK_Connector_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SOCK_Connector_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SOCK_Connector_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SOCK_Connector_Test.obj"

"$(OUTDIR)\SOCK_Connector_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Basic_Types_Test\Debug"
# PROP BASE Intermediate_Dir "Basic_Types_Test\Debug"
# PROP BASE Target_Dir "Basic_Types_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Basic_Types_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Basic_Types_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Basic_Types_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Basic_Types_Test.exe"
	-@erase "$(OUTDIR)\Basic_Types_Test.ilk"
	-@erase "$(OUTDIR)\Basic_Types_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Basic_Types_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Basic_Types_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Basic_Types_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Basic_Types_Test.obj"

"$(OUTDIR)\Basic_Types_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Env_Value_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Env_Value_Test\Debug"
# PROP BASE Intermediate_Dir "Env_Value_Test\Debug"
# PROP BASE Target_Dir "Env_Value_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Env_Value_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Env_Value_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Env_Value_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Env_Value_Test.exe"
	-@erase "$(OUTDIR)\Env_Value_Test.ilk"
	-@erase "$(OUTDIR)\Env_Value_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Env_Value_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Env_Value_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Env_Value_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Env_Value_Test.obj"

"$(OUTDIR)\Env_Value_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "OrdMultiSet_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OrdMultiSet_Test\Debug"
# PROP BASE Intermediate_Dir "OrdMultiSet_Test\Debug"
# PROP BASE Target_Dir "OrdMultiSet_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "OrdMultiSet_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\OrdMultiSet_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\OrdMultiSet_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\OrdMultiSet_Test.exe"
	-@erase "$(OUTDIR)\OrdMultiSet_Test.ilk"
	-@erase "$(OUTDIR)\OrdMultiSet_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/OrdMultiSet_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/OrdMultiSet_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/OrdMultiSet_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\OrdMultiSet_Test.obj"

"$(OUTDIR)\OrdMultiSet_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Timeprobe_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Timeprobe_Test\Debug"
# PROP BASE Intermediate_Dir "Timeprobe_Test\Debug"
# PROP BASE Target_Dir "Timeprobe_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Timeprobe_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Timeprobe_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Timeprobe_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Timeprobe_Test.exe"
	-@erase "$(OUTDIR)\Timeprobe_Test.ilk"
	-@erase "$(OUTDIR)\Timeprobe_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Timeprobe_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Timeprobe_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Timeprobe_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Timeprobe_Test.obj"

"$(OUTDIR)\Timeprobe_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_Queue_Notifications_Test\Debug"
# PROP BASE Intermediate_Dir "Message_Queue_Notifications_Test\Debug"
# PROP BASE Target_Dir "Message_Queue_Notifications_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Message_Queue_Notifications_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Message_Queue_Notifications_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Message_Queue_Notifications_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Message_Queue_Notifications_Test.exe"
	-@erase "$(OUTDIR)\Message_Queue_Notifications_Test.ilk"
	-@erase "$(OUTDIR)\Message_Queue_Notifications_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Message_Queue_Notifications_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Message_Queue_Notifications_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Message_Queue_Notifications_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Message_Queue_Notifications_Test.obj"

"$(OUTDIR)\Message_Queue_Notifications_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DLList_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DLList_Test\Debug"
# PROP BASE Intermediate_Dir "DLList_Test\Debug"
# PROP BASE Target_Dir "DLList_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "DLList_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\DLList_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\DLList_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\DLList_Test.exe"
	-@erase "$(OUTDIR)\DLList_Test.ilk"
	-@erase "$(OUTDIR)\DLList_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/DLList_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/DLList_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/DLList_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLList_Test.obj"

"$(OUTDIR)\DLList_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "High_Res_Timer_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "High_Res_Timer_Test\Debug"
# PROP BASE Intermediate_Dir "High_Res_Timer_Test\Debug"
# PROP BASE Target_Dir "High_Res_Timer_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "High_Res_Timer_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\High_Res_Timer_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\High_Res_Timer_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\High_Res_Timer_Test.exe"
	-@erase "$(OUTDIR)\High_Res_Timer_Test.ilk"
	-@erase "$(OUTDIR)\High_Res_Timer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/High_Res_Timer_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/High_Res_Timer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/High_Res_Timer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\High_Res_Timer_Test.obj"

"$(OUTDIR)\High_Res_Timer_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Dynamic_Priority_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Dynamic_Priority_Test\Debug"
# PROP BASE Intermediate_Dir "Dynamic_Priority_Test\Debug"
# PROP BASE Target_Dir "Dynamic_Priority_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Dynamic_Priority_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Dynamic_Priority_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Dynamic_Priority_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Dynamic_Priority_Test.exe"
	-@erase "$(OUTDIR)\Dynamic_Priority_Test.ilk"
	-@erase "$(OUTDIR)\Dynamic_Priority_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Dynamic_Priority_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Dynamic_Priority_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Dynamic_Priority_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Dynamic_Priority_Test.obj"

"$(OUTDIR)\Dynamic_Priority_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Thread_Pool_Reactor_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Thread_Pool_Reactor_Test\Debug"
# PROP BASE Intermediate_Dir "Thread_Pool_Reactor_Test\Debug"
# PROP BASE Target_Dir "Thread_Pool_Reactor_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Thread_Pool_Reactor_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Thread_Pool_Reactor_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Thread_Pool_Reactor_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Thread_Pool_Reactor_Test.exe"
	-@erase "$(OUTDIR)\Thread_Pool_Reactor_Test.ilk"
	-@erase "$(OUTDIR)\Thread_Pool_Reactor_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Pool_Reactor_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Pool_Reactor_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Pool_Reactor_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Thread_Pool_Reactor_Test.obj"

"$(OUTDIR)\Thread_Pool_Reactor_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Upgradable_RW_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Upgradable_RW_Test\Debug"
# PROP BASE Intermediate_Dir "Upgradable_RW_Test\Debug"
# PROP BASE Target_Dir "Upgradable_RW_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Upgradable_RW_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Upgradable_RW_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\Upgradable_RW_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Upgradable_RW_Test.exe"
	-@erase "$(OUTDIR)\Upgradable_RW_Test.ilk"
	-@erase "$(OUTDIR)\Upgradable_RW_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Upgradable_RW_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Upgradable_RW_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Upgradable_RW_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Upgradable_RW_Test.obj"

"$(OUTDIR)\Upgradable_RW_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MT_Reactor_Timer_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MT_Reactor_Timer_Test\Debug"
# PROP BASE Intermediate_Dir "MT_Reactor_Timer_Test\Debug"
# PROP BASE Target_Dir "MT_Reactor_Timer_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "MT_Reactor_Timer_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\MT_Reactor_Timer_Test.exe"

CLEAN : 
	-@erase "$(INTDIR)\MT_Reactor_Timer_Test.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MT_Reactor_Timer_Test.exe"
	-@erase "$(OUTDIR)\MT_Reactor_Timer_Test.ilk"
	-@erase "$(OUTDIR)\MT_Reactor_Timer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MT_Reactor_Timer_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/MT_Reactor_Timer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MT_Reactor_Timer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MT_Reactor_Timer_Test.obj"

"$(OUTDIR)\MT_Reactor_Timer_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

CPP_OBJS=.\Debug/
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

################################################################################
# Begin Target

# Name "Handle_Set_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Handle_Set_Test.cpp
DEP_CPP_HANDL=\
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Handle_Set_Test.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Mem_Map_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Mem_Map_Test.cpp
DEP_CPP_MEM_M=\
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
	"..\ace/Mem_Map.h"\
	"..\ace/Mem_Map.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Mem_Map_Test.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Naming_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Naming_Test.cpp
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
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Naming_Test.obj" : $(SOURCE) $(DEP_CPP_NAMIN) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reactor_Timer_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reactor_Timer_Test.cpp
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
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Reactor_Timer_Test.obj" : $(SOURCE) $(DEP_CPP_REACT) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reactors_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reactors_Test.cpp
DEP_CPP_REACTO=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Reactors_Test.obj" : $(SOURCE) $(DEP_CPP_REACTO) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "SString_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\SString_Test.cpp
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\SString_Test.obj" : $(SOURCE) $(DEP_CPP_SSTRI) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Time_Value_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Time_Value_Test.cpp
DEP_CPP_TIME_=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Time_Value_Test.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Timer_Queue_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Timer_Queue_Test.cpp
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Hash.h"\
	"..\ace/Timer_Hash_T.cpp"\
	"..\ace/Timer_Hash_T.h"\
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
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Timer_Queue_Test.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "UPIPE_SAP_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\UPIPE_SAP_Test.cpp
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/UPIPE_Connector.h"\
	"..\ace/UPIPE_Connector.i"\
	"..\ace/UPIPE_Stream.h"\
	"..\ace/UPIPE_Stream.i"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\UPIPE_SAP_Test.obj" : $(SOURCE) $(DEP_CPP_UPIPE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Priority_Buffer_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Priority_Buffer_Test.cpp
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Priority_Buffer_Test.obj" : $(SOURCE) $(DEP_CPP_PRIOR) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Time_Service_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Time_Service_Test.cpp
DEP_CPP_TIME_S=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Time_Service_Test.obj" : $(SOURCE) $(DEP_CPP_TIME_S) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "SPIPE_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\SPIPE_Test.cpp
DEP_CPP_SPIPE=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
	"..\ace/SPIPE.h"\
	"..\ace/SPIPE.i"\
	"..\ace/SPIPE_Acceptor.h"\
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
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\SPIPE_Test.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Buffer_Stream_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Buffer_Stream_Test.cpp
DEP_CPP_BUFFE=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Buffer_Stream_Test.obj" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Barrier_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Barrier_Test.cpp
DEP_CPP_BARRI=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Barrier_Test.obj" : $(SOURCE) $(DEP_CPP_BARRI) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reader_Writer_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reader_Writer_Test.cpp
DEP_CPP_READE=\
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
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Reader_Writer_Test.obj" : $(SOURCE) $(DEP_CPP_READE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Recursive_Mutex_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Recursive_Mutex_Test.cpp
DEP_CPP_RECUR=\
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
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Recursive_Mutex_Test.obj" : $(SOURCE) $(DEP_CPP_RECUR) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Task_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Task_Test.cpp
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Task_Test.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Thread_Manager_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Thread_Manager_Test.cpp
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
	"..\tests/test_config.h"\
	".\Thread_Manager_Test.h"\
	

"$(INTDIR)\Thread_Manager_Test.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "TSS_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\TSS_Test.cpp
DEP_CPP_TSS_T=\
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
	"..\tests/test_config.h"\
	".\TSS_Test_Errno.h"\
	

"$(INTDIR)\TSS_Test.obj" : $(SOURCE) $(DEP_CPP_TSS_T) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Thread_Pool_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Thread_Pool_Test.cpp
DEP_CPP_THREAD=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Thread_Pool_Test.obj" : $(SOURCE) $(DEP_CPP_THREAD) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Future_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Future_Test.cpp
DEP_CPP_FUTUR=\
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
	"..\ace/Future.cpp"\
	"..\ace/Future.h"\
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
	"..\ace/Method_Object.h"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Future_Test.obj" : $(SOURCE) $(DEP_CPP_FUTUR) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Tokens_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Tokens_Test.cpp
DEP_CPP_TOKEN=\
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
	"..\ace/Process.h"\
	"..\ace/Process.i"\
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
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token_Collection.h"\
	"..\ace/Token_Collection.i"\
	"..\ace/Token_Invariants.h"\
	"..\ace/Token_Invariants.i"\
	"..\ace/Token_Request_Reply.h"\
	"..\ace/Token_Request_Reply.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Tokens_Test.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Message_Queue_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Message_Queue_Test.cpp
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Message_Queue_Test.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Map_Manager_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Map_Manager_Test.cpp
DEP_CPP_MAP_M=\
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Svc_Conf_Tokens.h"\
	"..\ace/Synch.h"\
	"..\ace/Synch.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Map_Manager_Test.obj" : $(SOURCE) $(DEP_CPP_MAP_M) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Pipe_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Pipe_Test.cpp
DEP_CPP_PIPE_=\
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
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Process.h"\
	"..\ace/Process.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Pipe_Test.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Process_Mutex_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Process_Mutex_Test.cpp
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
	"..\ace/Process.h"\
	"..\ace/Process.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Process_Mutex_Test.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "SV_Shared_Memory_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\SV_Shared_Memory_Test.cpp
DEP_CPP_SV_SH=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\SV_Shared_Memory_Test.obj" : $(SOURCE) $(DEP_CPP_SV_SH) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "MM_Shared_Memory_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\MM_Shared_Memory_Test.cpp
DEP_CPP_MM_SH=\
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
	"..\ace/Shared_Memory.h"\
	"..\ace/Shared_Memory_MM.h"\
	"..\ace/Shared_Memory_MM.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\MM_Shared_Memory_Test.obj" : $(SOURCE) $(DEP_CPP_MM_SH) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Service_Config_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Service_Config_Test.cpp
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Service_Config_Test.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "SOCK_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\SOCK_Test.cpp
DEP_CPP_SOCK_=\
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
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\SOCK_Test.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Message_Block_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Message_Block_Test.cpp
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Message_Block_Test.obj" : $(SOURCE) $(DEP_CPP_MESSAG) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reactor_Notify_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reactor_Notify_Test.cpp
DEP_CPP_REACTOR=\
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
	"..\ace/Module.cpp"\
	"..\ace/Module.h"\
	"..\ace/Module.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Reactor_Notify_Test.obj" : $(SOURCE) $(DEP_CPP_REACTOR) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "IOStream_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\IOStream_Test.cpp
DEP_CPP_IOSTR=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/IO_Cntl_Msg.h"\
	"..\ace/iosfwd.h"\
	"..\ace/IOStream.h"\
	"..\ace/IOStream_T.cpp"\
	"..\ace/IOStream_T.h"\
	"..\ace/IOStream_T.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\IOStream_Test.obj" : $(SOURCE) $(DEP_CPP_IOSTR) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Process_Strategy_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Process_Strategy_Test.cpp
DEP_CPP_PROCES=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\tests/test_config.h"\
	".\Process_Strategy_Test.h"\
	

"$(INTDIR)\Process_Strategy_Test.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Conn_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Conn_Test.cpp
DEP_CPP_CONN_=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/Connector.cpp"\
	"..\ace/Connector.h"\
	"..\ace/Connector.i"\
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
	"..\ace/LOCK_SOCK_Acceptor.cpp"\
	"..\ace/LOCK_SOCK_Acceptor.h"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\tests/test_config.h"\
	".\Conn_Test.h"\
	

"$(INTDIR)\Conn_Test.obj" : $(SOURCE) $(DEP_CPP_CONN_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Simple_Message_Block_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Simple_Message_Block_Test.cpp
DEP_CPP_SIMPL=\
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
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Simple_Message_Block_Test.obj" : $(SOURCE) $(DEP_CPP_SIMPL)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reactor_Exceptions_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reactor_Exceptions_Test.cpp
DEP_CPP_REACTOR_=\
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
	"..\ace/Reactor.h"\
	"..\ace/Reactor.i"\
	"..\ace/Reactor_Impl.h"\
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Reactor_Exceptions_Test.obj" : $(SOURCE) $(DEP_CPP_REACTOR_)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Priority_Task_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Priority_Task_Test.cpp
DEP_CPP_PRIORI=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Priority_Task_Test.obj" : $(SOURCE) $(DEP_CPP_PRIORI) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Sigset_Ops_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Sigset_Ops_Test.cpp
DEP_CPP_SIGSE=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Sigset_Ops_Test.obj" : $(SOURCE) $(DEP_CPP_SIGSE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Enum_Interfaces_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Enum_Interfaces_Test.cpp
DEP_CPP_ENUM_=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Enum_Interfaces_Test.obj" : $(SOURCE) $(DEP_CPP_ENUM_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Hash_Map_Manager_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Hash_Map_Manager_Test.cpp
DEP_CPP_HASH_=\
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
	"..\tests/test_config.h"\
	".\Hash_Map_Manager_Test.h"\
	

"$(INTDIR)\Hash_Map_Manager_Test.obj" : $(SOURCE) $(DEP_CPP_HASH_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Semaphore_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Semaphore_Test.cpp
DEP_CPP_SEMAP=\
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
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Semaphore_Test.obj" : $(SOURCE) $(DEP_CPP_SEMAP) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Atomic_Op_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Atomic_Op_Test.cpp
DEP_CPP_ATOMI=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Atomic_Op_Test.obj" : $(SOURCE) $(DEP_CPP_ATOMI) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "MT_SOCK_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\MT_SOCK_Test.cpp
DEP_CPP_MT_SO=\
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
	"..\ace/Signal.h"\
	"..\ace/Signal.i"\
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Thread_Manager.h"\
	"..\ace/Thread_Manager.i"\
	"..\ace/Time_Value.h"\
	"..\ace/Trace.h"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\MT_SOCK_Test.obj" : $(SOURCE) $(DEP_CPP_MT_SO) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Priority_Reactor_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Priority_Reactor_Test.cpp
DEP_CPP_PRIORIT=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/Connector.cpp"\
	"..\ace/Connector.h"\
	"..\ace/Connector.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	".\Priority_Reactor_Test.h"\
	

"$(INTDIR)\Priority_Reactor_Test.obj" : $(SOURCE) $(DEP_CPP_PRIORIT)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Reactor_Performance_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Reactor_Performance_Test.cpp
DEP_CPP_REACTOR_P=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/Connector.cpp"\
	"..\ace/Connector.h"\
	"..\ace/Connector.i"\
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
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Token.h"\
	"..\ace/Token.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	".\Reactor_Performance_Test.h"\
	

"$(INTDIR)\Reactor_Performance_Test.obj" : $(SOURCE) $(DEP_CPP_REACTOR_P)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Notify_Performance_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Notify_Performance_Test.cpp
DEP_CPP_NOTIF=\
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
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
	"..\ace/Handle_Set.h"\
	"..\ace/Handle_Set.i"\
	"..\ace/Hash_Map_Manager.cpp"\
	"..\ace/Hash_Map_Manager.h"\
	"..\ace/High_Res_Timer.h"\
	"..\ace/High_Res_Timer.i"\
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
	"..\ace/Object_Manager.h"\
	"..\ace/Object_Manager.i"\
	"..\ace/OS.h"\
	"..\ace/OS.i"\
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	

"$(INTDIR)\Notify_Performance_Test.obj" : $(SOURCE) $(DEP_CPP_NOTIF)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Thread_Mutex_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Thread_Mutex_Test.cpp
DEP_CPP_THREAD_=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Thread_Mutex_Test.obj" : $(SOURCE) $(DEP_CPP_THREAD_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "SOCK_Connector_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\SOCK_Connector_Test.cpp
DEP_CPP_SOCK_C=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\SOCK_Connector_Test.obj" : $(SOURCE) $(DEP_CPP_SOCK_C) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Basic_Types_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Basic_Types_Test.cpp
DEP_CPP_BASIC=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Basic_Types_Test.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Env_Value_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Env_Value_Test.cpp
DEP_CPP_ENV_V=\
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
	"..\ace/Env_Value_T.cpp"\
	"..\ace/Env_Value_T.h"\
	"..\ace/Env_Value_T.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Env_Value_Test.obj" : $(SOURCE) $(DEP_CPP_ENV_V) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "OrdMultiSet_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\OrdMultiSet_Test.cpp
DEP_CPP_ORDMU=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\OrdMultiSet_Test.obj" : $(SOURCE) $(DEP_CPP_ORDMU) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Timeprobe_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Timeprobe_Test.cpp
DEP_CPP_TIMEP=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Timeprobe_Test.obj" : $(SOURCE) $(DEP_CPP_TIMEP) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Message_Queue_Notifications_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Message_Queue_Notifications_Test.cpp
DEP_CPP_MESSAGE=\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Message_Queue_Notifications_Test.obj" : $(SOURCE) $(DEP_CPP_MESSAGE)\
 "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "DLList_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\DLList_Test.cpp
DEP_CPP_DLLIS=\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\DLList_Test.obj" : $(SOURCE) $(DEP_CPP_DLLIS) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "High_Res_Timer_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\High_Res_Timer_Test.cpp
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
	"..\ace/Event_Handler.h"\
	"..\ace/Event_Handler.i"\
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
	"..\ace/Sched_Params.h"\
	"..\ace/Sched_Params.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\High_Res_Timer_Test.obj" : $(SOURCE) $(DEP_CPP_HIGH_) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Dynamic_Priority_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Dynamic_Priority_Test.cpp

!IF  "$(CFG)" == "Dynamic_Priority_Test - Win32 Debug"

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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	

"$(INTDIR)\Dynamic_Priority_Test.obj" : $(SOURCE) $(DEP_CPP_DYNAM) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "Thread_Pool_Reactor_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Thread_Pool_Reactor_Test.cpp

!IF  "$(CFG)" == "Thread_Pool_Reactor_Test - Win32 Debug"

DEP_CPP_THREAD_P=\
	"..\ace/Acceptor.cpp"\
	"..\ace/Acceptor.h"\
	"..\ace/Acceptor.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Svc_Handler.cpp"\
	"..\ace/Svc_Handler.h"\
	"..\ace/Svc_Handler.i"\
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
	"..\tests/test_config.h"\
	"..\tests/Thread_Pool_Reactor_Test.h"\
	

"$(INTDIR)\Thread_Pool_Reactor_Test.obj" : $(SOURCE) $(DEP_CPP_THREAD_P)\
 "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "Upgradable_RW_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Upgradable_RW_Test.cpp

!IF  "$(CFG)" == "Upgradable_RW_Test - Win32 Debug"

DEP_CPP_UPGRA=\
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
	"..\ace/Get_Opt.h"\
	"..\ace/Get_Opt.i"\
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
	"..\ace/Profile_Timer.h"\
	"..\ace/Profile_Timer.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\ace/Time_Value.h"\
	"..\ace/Timer_Queue.h"\
	"..\ace/Timer_Queue_T.cpp"\
	"..\ace/Timer_Queue_T.h"\
	"..\ace/Timer_Queue_T.i"\
	"..\ace/Trace.h"\
	"..\ace/WFMO_Reactor.h"\
	"..\ace/WFMO_Reactor.i"\
	"..\ace/ws2tcpip.h"\
	"..\tests/test_config.h"\
	".\Upgradable_RW_Test.h"\
	

"$(INTDIR)\Upgradable_RW_Test.obj" : $(SOURCE) $(DEP_CPP_UPGRA) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "MT_Reactor_Timer_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\MT_Reactor_Timer_Test.cpp
DEP_CPP_MT_RE=\
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
	"..\ace/Pipe.h"\
	"..\ace/Pipe.i"\
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
	"..\ace/Singleton.cpp"\
	"..\ace/Singleton.h"\
	"..\ace/Singleton.i"\
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
	"..\tests/test_config.h"\
	".\MT_Reactor_Timer_Test.h"\
	

"$(INTDIR)\MT_Reactor_Timer_Test.obj" : $(SOURCE) $(DEP_CPP_MT_RE) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
