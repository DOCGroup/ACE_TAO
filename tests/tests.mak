# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Pipe_Test - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Pipe_Test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CPP_Test - Win32 Debug" && "$(CFG)" !=\
 "Handle_Set_Test - Win32 Debug" && "$(CFG)" != "Mem_Map_Test - Win32 Debug" &&\
 "$(CFG)" != "Mutex_Test - Win32 Debug" && "$(CFG)" !=\
 "Naming_Test - Win32 Debug" && "$(CFG)" != "Reactor_Timer_Test - Win32 Debug"\
 && "$(CFG)" != "Reactors_Test - Win32 Debug" && "$(CFG)" !=\
 "SString_Test - Win32 Debug" && "$(CFG)" != "Time_Value_Test - Win32 Debug" &&\
 "$(CFG)" != "Timer_Queue_Test - Win32 Debug" && "$(CFG)" !=\
 "UPIPE_SAP_Test - Win32 Debug" && "$(CFG)" !=\
 "Priority_Buffer_Test - Win32 Debug" && "$(CFG)" !=\
 "Time_Service_Test - Win32 Debug" && "$(CFG)" != "SPIPE_Test - Win32 Debug" &&\
 "$(CFG)" != "Buffer_Stream_Test - Win32 Debug" && "$(CFG)" !=\
 "Barrier_Test - Win32 Debug" && "$(CFG)" != "Reader_Writer_Test - Win32 Debug"\
 && "$(CFG)" != "Recursive_Mutex_Test - Win32 Debug" && "$(CFG)" !=\
 "Task_Test - Win32 Debug" && "$(CFG)" != "Thread_Manager_Test - Win32 Debug" &&\
 "$(CFG)" != "TSS_Test - Win32 Debug" && "$(CFG)" !=\
 "Shared_Memory_MM_Test - Win32 Debug" && "$(CFG)" !=\
 "Thread_Pool_Test - Win32 Debug" && "$(CFG)" != "Future_Test - Win32 Debug" &&\
 "$(CFG)" != "Tokens_Test - Win32 Debug" && "$(CFG)" !=\
 "Message_Queue_Test - Win32 Debug" && "$(CFG)" !=\
 "Map_Manager_Test - Win32 Debug" && "$(CFG)" != "Pipe_Test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "tests.mak" CFG="Pipe_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CPP_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Handle_Set_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Mem_Map_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Mutex_Test - Win32 Debug" (based on\
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
!MESSAGE "Shared_Memory_MM_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "Priority_Buffer_Test - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "CPP_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CPP_Test\Debug"
# PROP BASE Intermediate_Dir "CPP_Test\Debug"
# PROP BASE Target_Dir "CPP_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "CPP_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\CPP_Test.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\CPP_Test.exe"
	-@erase ".\Debug\CPP_Test.obj"
	-@erase ".\CPP_Test.ilk"
	-@erase ".\CPP_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/CPP_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CPP_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/CPP_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CPP_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/CPP_Test.obj"

"$(OUTDIR)\CPP_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Handle_Set_Test - Win32 Debug"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Handle_Set_Test.exe"
	-@erase ".\Debug\Handle_Set_Test.obj"
	-@erase ".\Handle_Set_Test.ilk"
	-@erase ".\Handle_Set_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Handle_Set_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Handle_Set_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Handle_Set_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Handle_Set_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Handle_Set_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Mem_Map_Test.exe"
	-@erase ".\Debug\Mem_Map_Test.obj"
	-@erase ".\Mem_Map_Test.ilk"
	-@erase ".\Mem_Map_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Mem_Map_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Mem_Map_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Mem_Map_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Mem_Map_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Mem_Map_Test.obj"

"$(OUTDIR)\Mem_Map_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mutex_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mutex_Test\Debug"
# PROP BASE Intermediate_Dir "Mutex_Test\Debug"
# PROP BASE Target_Dir "Mutex_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Mutex_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Mutex_Test.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Mutex_Test.exe"
	-@erase ".\Debug\Mutex_Test.obj"
	-@erase ".\Mutex_Test.ilk"
	-@erase ".\Mutex_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Mutex_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Mutex_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Mutex_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Mutex_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Mutex_Test.obj"

"$(OUTDIR)\Mutex_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Naming_Test.exe"
	-@erase ".\Debug\Naming_Test.obj"
	-@erase ".\Naming_Test.ilk"
	-@erase ".\Naming_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Naming_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Naming_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Naming_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Naming_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Naming_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Reactor_Timer_Test.exe"
	-@erase ".\Debug\Reactor_Timer_Test.obj"
	-@erase ".\Reactor_Timer_Test.ilk"
	-@erase ".\Reactor_Timer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Reactor_Timer_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactor_Timer_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactor_Timer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactor_Timer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Reactor_Timer_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Reactors_Test.exe"
	-@erase ".\Debug\Reactors_Test.obj"
	-@erase ".\Reactors_Test.ilk"
	-@erase ".\Reactors_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Reactors_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reactors_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reactors_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reactors_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Reactors_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\SString_Test.exe"
	-@erase ".\Debug\SString_Test.obj"
	-@erase ".\SString_Test.ilk"
	-@erase ".\SString_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/SString_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SString_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SString_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SString_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SString_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Time_Value_Test.exe"
	-@erase ".\Debug\Time_Value_Test.obj"
	-@erase ".\Time_Value_Test.ilk"
	-@erase ".\Time_Value_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Time_Value_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Time_Value_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Time_Value_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Time_Value_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Time_Value_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Timer_Queue_Test.exe"
	-@erase ".\Debug\Timer_Queue_Test.obj"
	-@erase ".\Timer_Queue_Test.ilk"
	-@erase ".\Timer_Queue_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Timer_Queue_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Timer_Queue_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Timer_Queue_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Timer_Queue_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Timer_Queue_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\UPIPE_SAP_Test.exe"
	-@erase ".\Debug\UPIPE_SAP_Test.obj"
	-@erase ".\UPIPE_SAP_Test.ilk"
	-@erase ".\UPIPE_SAP_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/UPIPE_SAP_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/UPIPE_SAP_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib odbc32.lib odbccp32.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/UPIPE_SAP_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/UPIPE_SAP_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/UPIPE_SAP_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Priority_Buffer_Test.exe"
	-@erase ".\Debug\Priority_Buffer_Test.obj"
	-@erase ".\Priority_Buffer_Test.ilk"
	-@erase ".\Priority_Buffer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Priority_Buffer_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Priority_Buffer_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Priority_Buffer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Priority_Buffer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Priority_Buffer_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Time_Service_Test.exe"
	-@erase ".\Debug\Time_Service_Test.obj"
	-@erase ".\Time_Service_Test.ilk"
	-@erase ".\Time_Service_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Time_Service_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Time_Service_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Time_Service_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Time_Service_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Time_Service_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\SPIPE_Test.exe"
	-@erase ".\Debug\SPIPE_Test.obj"
	-@erase ".\SPIPE_Test.ilk"
	-@erase ".\SPIPE_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/SPIPE_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SPIPE_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/SPIPE_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SPIPE_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SPIPE_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Buffer_Stream_Test.exe"
	-@erase ".\Debug\Buffer_Stream_Test.obj"
	-@erase ".\Buffer_Stream_Test.ilk"
	-@erase ".\Buffer_Stream_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Buffer_Stream_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Buffer_Stream_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Buffer_Stream_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Buffer_Stream_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Buffer_Stream_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Barrier_Test.exe"
	-@erase ".\Debug\Barrier_Test.obj"
	-@erase ".\Barrier_Test.ilk"
	-@erase ".\Barrier_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Barrier_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Barrier_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Barrier_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Barrier_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Barrier_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Reader_Writer_Test.exe"
	-@erase ".\Debug\Reader_Writer_Test.obj"
	-@erase ".\Reader_Writer_Test.ilk"
	-@erase ".\Reader_Writer_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Reader_Writer_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Reader_Writer_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Reader_Writer_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Reader_Writer_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Reader_Writer_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Recursive_Mutex_Test.exe"
	-@erase ".\Debug\Recursive_Mutex_Test.obj"
	-@erase ".\Recursive_Mutex_Test.ilk"
	-@erase ".\Recursive_Mutex_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Recursive_Mutex_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Recursive_Mutex_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Recursive_Mutex_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Recursive_Mutex_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Recursive_Mutex_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Task_Test.exe"
	-@erase ".\Debug\Task_Test.obj"
	-@erase ".\Task_Test.ilk"
	-@erase ".\Task_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Task_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Task_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Task_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Task_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Task_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Thread_Manager_Test.exe"
	-@erase ".\Debug\Thread_Manager_Test.obj"
	-@erase ".\Thread_Manager_Test.ilk"
	-@erase ".\Thread_Manager_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Thread_Manager_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Manager_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Manager_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Manager_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Thread_Manager_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\TSS_Test.exe"
	-@erase ".\Debug\TSS_Test.obj"
	-@erase ".\TSS_Test.ilk"
	-@erase ".\TSS_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/TSS_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TSS_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/TSS_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/TSS_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/TSS_Test.obj"

"$(OUTDIR)\TSS_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Shared_Memory_MM_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Shared_Memory_MM_Test\Debug"
# PROP BASE Intermediate_Dir "Shared_Memory_MM_Test\Debug"
# PROP BASE Target_Dir "Shared_Memory_MM_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Shared_Memory_MM_Test"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Shared_Memory_MM_Test.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Shared_Memory_MM_Test.exe"
	-@erase ".\Debug\Shared_Memory_MM_Test.obj"
	-@erase ".\Shared_Memory_MM_Test.ilk"
	-@erase ".\Shared_Memory_MM_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Shared_Memory_MM_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Shared_Memory_MM_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib wsock32.lib ace.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Shared_Memory_MM_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Shared_Memory_MM_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Shared_Memory_MM_Test.obj"

"$(OUTDIR)\Shared_Memory_MM_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Thread_Pool_Test.exe"
	-@erase ".\Debug\Thread_Pool_Test.obj"
	-@erase ".\Thread_Pool_Test.ilk"
	-@erase ".\Thread_Pool_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Thread_Pool_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Thread_Pool_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ace.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib ace.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Thread_Pool_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Thread_Pool_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Thread_Pool_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Future_Test.exe"
	-@erase ".\Debug\Future_Test.obj"
	-@erase ".\Future_Test.ilk"
	-@erase ".\Future_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Future_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Future_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ace.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib ace.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Future_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Future_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Future_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Tokens_Test.exe"
	-@erase ".\Debug\Tokens_Test.obj"
	-@erase ".\Tokens_Test.ilk"
	-@erase ".\Tokens_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Tokens_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Tokens_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ace.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ace.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/Tokens_Test.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/Tokens_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Tokens_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Message_Queue_Test.exe"
	-@erase ".\Debug\Message_Queue_Test.obj"
	-@erase ".\Message_Queue_Test.ilk"
	-@erase ".\Message_Queue_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Message_Queue_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Message_Queue_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Message_Queue_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Message_Queue_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Message_Queue_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Map_Manager_Test.exe"
	-@erase ".\Debug\Map_Manager_Test.obj"
	-@erase ".\Map_Manager_Test.ilk"
	-@erase ".\Map_Manager_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Map_Manager_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Map_Manager_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Map_Manager_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Map_Manager_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Map_Manager_Test.obj"

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
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Pipe_Test.exe"
	-@erase ".\Debug\Pipe_Test.obj"
	-@erase ".\Pipe_Test.ilk"
	-@erase ".\Pipe_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Pipe_Test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pipe_Test.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Pipe_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Pipe_Test.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Pipe_Test.obj"

"$(OUTDIR)\Pipe_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

CPP_OBJS=.\Debug/
CPP_SBRS=

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

# Name "CPP_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\CPP_Test.cpp
DEP_CPP_CPP_T=\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\SOCK_Connector.h"\
	{$(INCLUDE)}"\ace\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\SOCK_Connector.i"\
	{$(INCLUDE)}"\ace\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	
NODEP_CPP_CPP_T=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\CPP_Test.obj" : $(SOURCE) $(DEP_CPP_CPP_T) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Handle_Set_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Handle_Set_Test.cpp
DEP_CPP_HANDL=\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	
NODEP_CPP_HANDL=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	
NODEP_CPP_MEM_M=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\Mem_Map_Test.obj" : $(SOURCE) $(DEP_CPP_MEM_M) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Mutex_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Mutex_Test.cpp
DEP_CPP_MUTEX=\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	
NODEP_CPP_MUTEX=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\Mutex_Test.obj" : $(SOURCE) $(DEP_CPP_MUTEX) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Naming_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Naming_Test.cpp
DEP_CPP_NAMIN=\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\Naming_Context.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Name_Proxy.h"\
	{$(INCLUDE)}"\ace\Name_Space.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Connector.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Name_Request_Reply.h"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\SOCK_Connector.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	
NODEP_CPP_NAMIN=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	
NODEP_CPP_REACT=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Task.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	
NODEP_CPP_REACTO=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\SString.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	
NODEP_CPP_SSTRI=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	
NODEP_CPP_TIME_=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	
NODEP_CPP_TIMER=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Stream.h"\
	{$(INCLUDE)}"\ace\UPIPE_Acceptor.h"\
	{$(INCLUDE)}"\ace\UPIPE_Connector.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Stream.i"\
	{$(INCLUDE)}"\ace\Stream.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\SPIPE_Acceptor.h"\
	{$(INCLUDE)}"\ace\UPIPE_Acceptor.i"\
	{$(INCLUDE)}"\ace\SPIPE.h"\
	{$(INCLUDE)}"\ace\UPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\UPIPE_Stream.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.i"\
	{$(INCLUDE)}"\ace\UPIPE_Connector.i"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	
NODEP_CPP_UPIPE=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	
NODEP_CPP_PRIOR=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Process.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Process.i"\
	
NODEP_CPP_TIME_S=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\SPIPE_Connector.h"\
	{$(INCLUDE)}"\ace\SPIPE_Acceptor.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\SPIPE_Addr.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.h"\
	{$(INCLUDE)}"\ace\SPIPE_Connector.i"\
	{$(INCLUDE)}"\ace\SPIPE.h"\
	{$(INCLUDE)}"\ace\SPIPE_Stream.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SPIPE.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	
NODEP_CPP_SPIPE=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Stream.h"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Task.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Stream.i"\
	{$(INCLUDE)}"\ace\Stream.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	
NODEP_CPP_BUFFE=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	
NODEP_CPP_BARRI=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	
NODEP_CPP_READE=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	
NODEP_CPP_RECUR=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Task.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	
NODEP_CPP_TASK_=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	
NODEP_CPP_THREA=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	
NODEP_CPP_TSS_T=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\TSS_Test.obj" : $(SOURCE) $(DEP_CPP_TSS_T) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Shared_Memory_MM_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Shared_Memory_MM_Test.cpp
DEP_CPP_SHARE=\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Shared_Memory_MM.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Shared_Memory.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Shared_Memory_MM.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	
NODEP_CPP_SHARE=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\Shared_Memory_MM_Test.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Thread_Pool_Test - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\Thread_Pool_Test.cpp
DEP_CPP_THREAD=\
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	
NODEP_CPP_THREAD=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Future.h"\
	{$(INCLUDE)}"\ace\Method_Object.h"\
	{$(INCLUDE)}"\ace\Activation_Queue.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Module.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Module.i"\
	{$(INCLUDE)}"\ace\Module.cpp"\
	{$(INCLUDE)}"\ace\Stream_Modules.h"\
	{$(INCLUDE)}"\ace\Stream_Modules.i"\
	{$(INCLUDE)}"\ace\Stream_Modules.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\Future.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	
NODEP_CPP_FUTUR=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\Process.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\Token_Collection.h"\
	{$(INCLUDE)}"\ace\Remote_Tokens.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Token_Invariants.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\Token_Collection.i"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Connector.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Token_Request_Reply.h"\
	{$(INCLUDE)}"\ace\Remote_Tokens.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\SOCK_Connector.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Token_Request_Reply.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\Token_Invariants.i"\
	
NODEP_CPP_TOKEN=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Message_Queue.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\IO_Cntl_Msg.h"\
	{$(INCLUDE)}"\ace\Message_Queue.i"\
	{$(INCLUDE)}"\ace\Message_Queue.cpp"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	
NODEP_CPP_MESSA=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Synch.i"\
	
NODEP_CPP_MAP_M=\
	".\..\ace\ace\ws2tcpip.h"\
	

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
	{$(INCLUDE)}"\ace\Pipe.h"\
	".\test_config.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	
NODEP_CPP_PIPE_=\
	".\..\ace\ace\ws2tcpip.h"\
	

"$(INTDIR)\Pipe_Test.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
