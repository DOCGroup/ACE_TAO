# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=_build_all_ - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to _build_all_ - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "_build_all_ - Win32 Release" && "$(CFG)" != "_build_all_ - Win32 Debug" &&\
 "$(CFG)" != "auto_event - Win32 Release" && "$(CFG)" !=\
 "auto_event - Win32 Debug" && "$(CFG)" != "barrier1 - Win32 Release" &&\
 "$(CFG)" != "barrier1 - Win32 Debug" && "$(CFG)" != "barrier2 - Win32 Release"\
 && "$(CFG)" != "barrier2 - Win32 Debug" && "$(CFG)" != "cancel - Win32 Release"\
 && "$(CFG)" != "cancel - Win32 Debug" && "$(CFG)" != "future1 - Win32 Release"\
 && "$(CFG)" != "future1 - Win32 Debug" && "$(CFG)" != "future2 - Win32 Release"\
 && "$(CFG)" != "future2 - Win32 Debug" && "$(CFG)" !=\
 "manual_event - Win32 Release" && "$(CFG)" != "manual_event - Win32 Debug" &&\
 "$(CFG)" != "process_mutex - Win32 Release" && "$(CFG)" !=\
 "process_mutex - Win32 Debug" && "$(CFG)" !=\
 "process_semaphore - Win32 Release" && "$(CFG)" !=\
 "process_semaphore - Win32 Debug" && "$(CFG)" !=\
 "reader_writer - Win32 Release" && "$(CFG)" != "reader_writer - Win32 Debug" &&\
 "$(CFG)" != "recursive_mutex - Win32 Release" && "$(CFG)" !=\
 "recursive_mutex - Win32 Debug" && "$(CFG)" != "task_one - Win32 Release" &&\
 "$(CFG)" != "task_one - Win32 Debug" && "$(CFG)" != "task_two - Win32 Release"\
 && "$(CFG)" != "task_two - Win32 Debug" && "$(CFG)" !=\
 "task_three - Win32 Release" && "$(CFG)" != "task_three - Win32 Debug" &&\
 "$(CFG)" != "task_four - Win32 Release" && "$(CFG)" !=\
 "task_four - Win32 Debug" && "$(CFG)" != "thread_manager - Win32 Release" &&\
 "$(CFG)" != "thread_manager - Win32 Debug" && "$(CFG)" !=\
 "thread_pool - Win32 Release" && "$(CFG)" != "thread_pool - Win32 Debug" &&\
 "$(CFG)" != "thread_specific - Win32 Release" && "$(CFG)" !=\
 "thread_specific - Win32 Debug" && "$(CFG)" != "token - Win32 Release" &&\
 "$(CFG)" != "token - Win32 Debug" && "$(CFG)" != "tss1 - Win32 Release" &&\
 "$(CFG)" != "tss1 - Win32 Debug" && "$(CFG)" != "tss2 - Win32 Release" &&\
 "$(CFG)" != "tss2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="tss2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "_build_all_ - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "_build_all_ - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "auto_event - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "auto_event - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "barrier1 - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "barrier1 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "barrier2 - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "barrier2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "cancel - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cancel - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "future1 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "future1 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "future2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "future2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "manual_event - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "manual_event - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "process_mutex - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "process_mutex - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "process_semaphore - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "process_semaphore - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "reader_writer - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "reader_writer - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "recursive_mutex - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "recursive_mutex - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_one - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_one - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "task_two - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_two - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "task_three - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_three - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_four - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "task_four - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "thread_manager - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "thread_manager - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "thread_pool - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "thread_pool - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "thread_specific - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "thread_specific - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "token - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "token - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "tss1 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tss1 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "tss2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tss2 - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "tss2 - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\.
INTDIR=.\Release

ALL : "tss2 - Win32 Release" "tss1 - Win32 Release" "token - Win32 Release"\
 "thread_specific - Win32 Release" "thread_pool - Win32 Release"\
 "thread_manager - Win32 Release" "task_four - Win32 Release"\
 "task_three - Win32 Release" "task_two - Win32 Release"\
 "task_one - Win32 Release" "recursive_mutex - Win32 Release"\
 "reader_writer - Win32 Release" "process_semaphore - Win32 Release"\
 "process_mutex - Win32 Release" "manual_event - Win32 Release"\
 "future2 - Win32 Release" "future1 - Win32 Release" "cancel - Win32 Release"\
 "barrier2 - Win32 Release" "barrier1 - Win32 Release"\
 "auto_event - Win32 Release" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/test.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/test.pdb" /machine:I386\
 /out:"$(OUTDIR)/test.exe" 
LINK32_OBJS= \
	

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "tss2 - Win32 Debug" "tss1 - Win32 Debug" "token - Win32 Debug"\
 "thread_specific - Win32 Debug" "thread_pool - Win32 Debug"\
 "thread_manager - Win32 Debug" "task_four - Win32 Debug"\
 "task_three - Win32 Debug" "task_two - Win32 Debug" "task_one - Win32 Debug"\
 "recursive_mutex - Win32 Debug" "reader_writer - Win32 Debug"\
 "process_semaphore - Win32 Debug" "process_mutex - Win32 Debug"\
 "manual_event - Win32 Debug" "future2 - Win32 Debug" "future1 - Win32 Debug"\
 "cancel - Win32 Debug" "barrier2 - Win32 Debug" "barrier1 - Win32 Debug"\
 "auto_event - Win32 Debug" 

CLEAN : 
	-@erase 

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/test.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/test.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/test.exe" 
LINK32_OBJS= \
	

!ELSEIF  "$(CFG)" == "auto_event - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "auto_event\Release"
# PROP BASE Intermediate_Dir "auto_event\Release"
# PROP BASE Target_Dir "auto_event"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "auto_event"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\auto_event.exe"

CLEAN : 
	-@erase "$(INTDIR)\auto_event.obj"
	-@erase "$(OUTDIR)\auto_event.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/auto_event.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/auto_event.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/auto_event.pdb"\
 /machine:I386 /out:"$(OUTDIR)/auto_event.exe" 
LINK32_OBJS= \
	"$(INTDIR)\auto_event.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\auto_event.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "auto_event - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "auto_event\Debug"
# PROP BASE Intermediate_Dir "auto_event\Debug"
# PROP BASE Target_Dir "auto_event"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "auto_event"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\auto_event.exe"

CLEAN : 
	-@erase "$(INTDIR)\auto_event.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\auto_event.exe"
	-@erase "$(OUTDIR)\auto_event.ilk"
	-@erase "$(OUTDIR)\auto_event.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/auto_event.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/auto_event.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/auto_event.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/auto_event.exe" 
LINK32_OBJS= \
	"$(INTDIR)\auto_event.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\auto_event.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "barrier1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "barrier1\Release"
# PROP BASE Intermediate_Dir "barrier1\Release"
# PROP BASE Target_Dir "barrier1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "barrier1"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\barrier1.exe"

CLEAN : 
	-@erase "$(INTDIR)\barrier1.obj"
	-@erase "$(OUTDIR)\barrier1.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/barrier1.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/barrier1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/barrier1.pdb" /machine:I386\
 /out:"$(OUTDIR)/barrier1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\barrier1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\barrier1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "barrier1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "barrier1\Debug"
# PROP BASE Intermediate_Dir "barrier1\Debug"
# PROP BASE Target_Dir "barrier1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "barrier1"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\barrier1.exe"

CLEAN : 
	-@erase "$(INTDIR)\barrier1.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\barrier1.exe"
	-@erase "$(OUTDIR)\barrier1.ilk"
	-@erase "$(OUTDIR)\barrier1.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/barrier1.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/barrier1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/barrier1.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/barrier1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\barrier1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\barrier1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "barrier2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "barrier2\Release"
# PROP BASE Intermediate_Dir "barrier2\Release"
# PROP BASE Target_Dir "barrier2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "barrier2"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\barrier2.exe"

CLEAN : 
	-@erase "$(INTDIR)\barrier2.obj"
	-@erase "$(OUTDIR)\barrier2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/barrier2.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/barrier2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/barrier2.pdb" /machine:I386\
 /out:"$(OUTDIR)/barrier2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\barrier2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\barrier2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "barrier2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "barrier2\Debug"
# PROP BASE Intermediate_Dir "barrier2\Debug"
# PROP BASE Target_Dir "barrier2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "barrier2"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\barrier2.exe"

CLEAN : 
	-@erase "$(INTDIR)\barrier2.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\barrier2.exe"
	-@erase "$(OUTDIR)\barrier2.ilk"
	-@erase "$(OUTDIR)\barrier2.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/barrier2.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/barrier2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/barrier2.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/barrier2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\barrier2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\barrier2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cancel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cancel\Release"
# PROP BASE Intermediate_Dir "cancel\Release"
# PROP BASE Target_Dir "cancel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "cancel"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\cancel.exe"

CLEAN : 
	-@erase "$(INTDIR)\cancel.obj"
	-@erase "$(OUTDIR)\cancel.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cancel.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cancel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/cancel.pdb" /machine:I386\
 /out:"$(OUTDIR)/cancel.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cancel.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\cancel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cancel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cancel\Debug"
# PROP BASE Intermediate_Dir "cancel\Debug"
# PROP BASE Target_Dir "cancel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "cancel"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\cancel.exe"

CLEAN : 
	-@erase "$(INTDIR)\cancel.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\cancel.exe"
	-@erase "$(OUTDIR)\cancel.ilk"
	-@erase "$(OUTDIR)\cancel.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cancel.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cancel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/cancel.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/cancel.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cancel.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\cancel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "future1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "future1\Release"
# PROP BASE Intermediate_Dir "future1\Release"
# PROP BASE Target_Dir "future1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "future1"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\future1.exe"

CLEAN : 
	-@erase "$(INTDIR)\future1.obj"
	-@erase "$(OUTDIR)\future1.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/future1.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/future1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/future1.pdb" /machine:I386\
 /out:"$(OUTDIR)/future1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\future1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\future1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "future1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "future1\Debug"
# PROP BASE Intermediate_Dir "future1\Debug"
# PROP BASE Target_Dir "future1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "future1"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\future1.exe"

CLEAN : 
	-@erase "$(INTDIR)\future1.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\future1.exe"
	-@erase "$(OUTDIR)\future1.ilk"
	-@erase "$(OUTDIR)\future1.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/future1.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/future1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/future1.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/future1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\future1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\future1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "future2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "future2\Release"
# PROP BASE Intermediate_Dir "future2\Release"
# PROP BASE Target_Dir "future2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "future2"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\future2.exe"

CLEAN : 
	-@erase "$(INTDIR)\future2.obj"
	-@erase "$(OUTDIR)\future2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/future2.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/future2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/future2.pdb" /machine:I386\
 /out:"$(OUTDIR)/future2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\future2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\future2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "future2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "future2\Debug"
# PROP BASE Intermediate_Dir "future2\Debug"
# PROP BASE Target_Dir "future2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "future2"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\future2.exe"

CLEAN : 
	-@erase "$(INTDIR)\future2.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\future2.exe"
	-@erase "$(OUTDIR)\future2.ilk"
	-@erase "$(OUTDIR)\future2.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/future2.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/future2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/future2.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/future2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\future2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\future2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "manual_event - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "manual_event\Release"
# PROP BASE Intermediate_Dir "manual_event\Release"
# PROP BASE Target_Dir "manual_event"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "manual_event"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\manual_event.exe"

CLEAN : 
	-@erase "$(INTDIR)\manual_event.obj"
	-@erase "$(OUTDIR)\manual_event.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/manual_event.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/manual_event.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/manual_event.pdb"\
 /machine:I386 /out:"$(OUTDIR)/manual_event.exe" 
LINK32_OBJS= \
	"$(INTDIR)\manual_event.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\manual_event.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "manual_event - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "manual_event\Debug"
# PROP BASE Intermediate_Dir "manual_event\Debug"
# PROP BASE Target_Dir "manual_event"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "manual_event"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\manual_event.exe"

CLEAN : 
	-@erase "$(INTDIR)\manual_event.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\manual_event.exe"
	-@erase "$(OUTDIR)\manual_event.ilk"
	-@erase "$(OUTDIR)\manual_event.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/manual_event.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/manual_event.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/manual_event.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/manual_event.exe" 
LINK32_OBJS= \
	"$(INTDIR)\manual_event.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\manual_event.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "process_mutex - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "process_mutex\Release"
# PROP BASE Intermediate_Dir "process_mutex\Release"
# PROP BASE Target_Dir "process_mutex"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "process_mutex"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\process_mutex.exe"

CLEAN : 
	-@erase "$(INTDIR)\process_mutex.obj"
	-@erase "$(OUTDIR)\process_mutex.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/process_mutex.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/process_mutex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/process_mutex.pdb"\
 /machine:I386 /out:"$(OUTDIR)/process_mutex.exe" 
LINK32_OBJS= \
	"$(INTDIR)\process_mutex.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\process_mutex.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "process_mutex - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "process_mutex\Debug"
# PROP BASE Intermediate_Dir "process_mutex\Debug"
# PROP BASE Target_Dir "process_mutex"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "process_mutex"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\process_mutex.exe"

CLEAN : 
	-@erase "$(INTDIR)\process_mutex.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\process_mutex.exe"
	-@erase "$(OUTDIR)\process_mutex.ilk"
	-@erase "$(OUTDIR)\process_mutex.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/process_mutex.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/process_mutex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/process_mutex.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/process_mutex.exe" 
LINK32_OBJS= \
	"$(INTDIR)\process_mutex.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\process_mutex.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "process_semaphore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "process_semaphore\Release"
# PROP BASE Intermediate_Dir "process_semaphore\Release"
# PROP BASE Target_Dir "process_semaphore"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "process_semaphore"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\process_semaphore.exe"

CLEAN : 
	-@erase "$(INTDIR)\process_semaphore.obj"
	-@erase "$(OUTDIR)\process_semaphore.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/process_semaphore.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/process_semaphore.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/process_semaphore.pdb"\
 /machine:I386 /out:"$(OUTDIR)/process_semaphore.exe" 
LINK32_OBJS= \
	"$(INTDIR)\process_semaphore.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\process_semaphore.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "process_semaphore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "process_semaphore\Debug"
# PROP BASE Intermediate_Dir "process_semaphore\Debug"
# PROP BASE Target_Dir "process_semaphore"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "process_semaphore"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\process_semaphore.exe"

CLEAN : 
	-@erase "$(INTDIR)\process_semaphore.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\process_semaphore.exe"
	-@erase "$(OUTDIR)\process_semaphore.ilk"
	-@erase "$(OUTDIR)\process_semaphore.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/process_semaphore.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/process_semaphore.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/process_semaphore.pdb"\
 /debug /machine:I386 /out:"$(OUTDIR)/process_semaphore.exe" 
LINK32_OBJS= \
	"$(INTDIR)\process_semaphore.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\process_semaphore.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "reader_writer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "reader_writer\Release"
# PROP BASE Intermediate_Dir "reader_writer\Release"
# PROP BASE Target_Dir "reader_writer"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "reader_writer"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\reader_writer.exe"

CLEAN : 
	-@erase "$(INTDIR)\reader_writer.obj"
	-@erase "$(OUTDIR)\reader_writer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/reader_writer.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/reader_writer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/reader_writer.pdb"\
 /machine:I386 /out:"$(OUTDIR)/reader_writer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\reader_writer.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\reader_writer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "reader_writer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "reader_writer\Debug"
# PROP BASE Intermediate_Dir "reader_writer\Debug"
# PROP BASE Target_Dir "reader_writer"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "reader_writer"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\reader_writer.exe"

CLEAN : 
	-@erase "$(INTDIR)\reader_writer.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\reader_writer.exe"
	-@erase "$(OUTDIR)\reader_writer.ilk"
	-@erase "$(OUTDIR)\reader_writer.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/reader_writer.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/reader_writer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/reader_writer.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/reader_writer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\reader_writer.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\reader_writer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "recursive_mutex - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "recursive_mutex\Release"
# PROP BASE Intermediate_Dir "recursive_mutex\Release"
# PROP BASE Target_Dir "recursive_mutex"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "recursive_mutex"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\recursive_mutex.exe"

CLEAN : 
	-@erase "$(INTDIR)\recursive_mutex.obj"
	-@erase "$(OUTDIR)\recursive_mutex.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/recursive_mutex.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/recursive_mutex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/recursive_mutex.pdb"\
 /machine:I386 /out:"$(OUTDIR)/recursive_mutex.exe" 
LINK32_OBJS= \
	"$(INTDIR)\recursive_mutex.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\recursive_mutex.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "recursive_mutex - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "recursive_mutex\Debug"
# PROP BASE Intermediate_Dir "recursive_mutex\Debug"
# PROP BASE Target_Dir "recursive_mutex"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "recursive_mutex"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\recursive_mutex.exe"

CLEAN : 
	-@erase "$(INTDIR)\recursive_mutex.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\recursive_mutex.exe"
	-@erase "$(OUTDIR)\recursive_mutex.ilk"
	-@erase "$(OUTDIR)\recursive_mutex.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/recursive_mutex.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/recursive_mutex.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/recursive_mutex.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/recursive_mutex.exe" 
LINK32_OBJS= \
	"$(INTDIR)\recursive_mutex.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\recursive_mutex.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_one - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "task_one\Release"
# PROP BASE Intermediate_Dir "task_one\Release"
# PROP BASE Target_Dir "task_one"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "task_one"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\task_one.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_one.obj"
	-@erase "$(OUTDIR)\task_one.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_one.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_one.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/task_one.pdb" /machine:I386\
 /out:"$(OUTDIR)/task_one.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_one.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_one.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_one - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "task_one\Debug"
# PROP BASE Intermediate_Dir "task_one\Debug"
# PROP BASE Target_Dir "task_one"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "task_one"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\task_one.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_one.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\task_one.exe"
	-@erase "$(OUTDIR)\task_one.ilk"
	-@erase "$(OUTDIR)\task_one.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_one.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_one.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/task_one.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/task_one.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_one.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_one.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_two - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "task_two\Release"
# PROP BASE Intermediate_Dir "task_two\Release"
# PROP BASE Target_Dir "task_two"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "task_two"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\task_two.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_two.obj"
	-@erase "$(OUTDIR)\task_two.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_two.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_two.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/task_two.pdb" /machine:I386\
 /out:"$(OUTDIR)/task_two.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_two.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_two.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_two - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "task_two\Debug"
# PROP BASE Intermediate_Dir "task_two\Debug"
# PROP BASE Target_Dir "task_two"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "task_two"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\task_two.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_two.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\task_two.exe"
	-@erase "$(OUTDIR)\task_two.ilk"
	-@erase "$(OUTDIR)\task_two.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_two.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_two.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/task_two.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/task_two.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_two.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_two.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_three - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "task_three\Release"
# PROP BASE Intermediate_Dir "task_three\Release"
# PROP BASE Target_Dir "task_three"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "task_three"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\task_three.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_three.obj"
	-@erase "$(OUTDIR)\task_three.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_three.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_three.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/task_three.pdb"\
 /machine:I386 /out:"$(OUTDIR)/task_three.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_three.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_three.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_three - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "task_three\Debug"
# PROP BASE Intermediate_Dir "task_three\Debug"
# PROP BASE Target_Dir "task_three"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "task_three"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\task_three.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_three.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\task_three.exe"
	-@erase "$(OUTDIR)\task_three.ilk"
	-@erase "$(OUTDIR)\task_three.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_three.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_three.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/task_three.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/task_three.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_three.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_three.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_four - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "task_four\Release"
# PROP BASE Intermediate_Dir "task_four\Release"
# PROP BASE Target_Dir "task_four"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "task_four"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\task_four.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_four.obj"
	-@erase "$(OUTDIR)\task_four.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_four.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_four.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/task_four.pdb" /machine:I386\
 /out:"$(OUTDIR)/task_four.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_four.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_four.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "task_four - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "task_four\Debug"
# PROP BASE Intermediate_Dir "task_four\Debug"
# PROP BASE Target_Dir "task_four"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "task_four"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\task_four.exe"

CLEAN : 
	-@erase "$(INTDIR)\task_four.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\task_four.exe"
	-@erase "$(OUTDIR)\task_four.ilk"
	-@erase "$(OUTDIR)\task_four.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/task_four.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/task_four.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/task_four.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/task_four.exe" 
LINK32_OBJS= \
	"$(INTDIR)\task_four.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\task_four.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_manager - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "thread_manager\Release"
# PROP BASE Intermediate_Dir "thread_manager\Release"
# PROP BASE Target_Dir "thread_manager"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "thread_manager"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\thread_manager.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_manager.obj"
	-@erase "$(OUTDIR)\thread_manager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_manager.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_manager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/thread_manager.pdb"\
 /machine:I386 /out:"$(OUTDIR)/thread_manager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_manager.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_manager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_manager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "thread_manager\Debug"
# PROP BASE Intermediate_Dir "thread_manager\Debug"
# PROP BASE Target_Dir "thread_manager"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "thread_manager"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\thread_manager.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_manager.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\thread_manager.exe"
	-@erase "$(OUTDIR)\thread_manager.ilk"
	-@erase "$(OUTDIR)\thread_manager.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_manager.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_manager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/thread_manager.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/thread_manager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_manager.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_manager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_pool - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "thread_pool\Release"
# PROP BASE Intermediate_Dir "thread_pool\Release"
# PROP BASE Target_Dir "thread_pool"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "thread_pool"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\thread_pool.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_pool.obj"
	-@erase "$(OUTDIR)\thread_pool.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_pool.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_pool.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/thread_pool.pdb"\
 /machine:I386 /out:"$(OUTDIR)/thread_pool.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_pool.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_pool.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_pool - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "thread_pool\Debug"
# PROP BASE Intermediate_Dir "thread_pool\Debug"
# PROP BASE Target_Dir "thread_pool"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "thread_pool"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\thread_pool.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_pool.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\thread_pool.exe"
	-@erase "$(OUTDIR)\thread_pool.ilk"
	-@erase "$(OUTDIR)\thread_pool.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_pool.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_pool.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/thread_pool.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/thread_pool.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_pool.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_pool.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_specific - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "thread_specific\Release"
# PROP BASE Intermediate_Dir "thread_specific\Release"
# PROP BASE Target_Dir "thread_specific"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "thread_specific"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\thread_specific.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_specific.obj"
	-@erase "$(OUTDIR)\thread_specific.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_specific.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_specific.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/thread_specific.pdb"\
 /machine:I386 /out:"$(OUTDIR)/thread_specific.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_specific.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_specific.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "thread_specific - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "thread_specific\Debug"
# PROP BASE Intermediate_Dir "thread_specific\Debug"
# PROP BASE Target_Dir "thread_specific"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "thread_specific"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\thread_specific.exe"

CLEAN : 
	-@erase "$(INTDIR)\thread_specific.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\thread_specific.exe"
	-@erase "$(OUTDIR)\thread_specific.ilk"
	-@erase "$(OUTDIR)\thread_specific.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/thread_specific.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/thread_specific.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/thread_specific.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/thread_specific.exe" 
LINK32_OBJS= \
	"$(INTDIR)\thread_specific.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\thread_specific.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "token - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "token\Release"
# PROP BASE Intermediate_Dir "token\Release"
# PROP BASE Target_Dir "token"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "token"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\token.exe"

CLEAN : 
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(OUTDIR)\token.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/token.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/token.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/token.pdb" /machine:I386\
 /out:"$(OUTDIR)/token.exe" 
LINK32_OBJS= \
	"$(INTDIR)\token.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\token.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "token - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "token\Debug"
# PROP BASE Intermediate_Dir "token\Debug"
# PROP BASE Target_Dir "token"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "token"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\token.exe"

CLEAN : 
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\token.exe"
	-@erase "$(OUTDIR)\token.ilk"
	-@erase "$(OUTDIR)\token.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/token.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/token.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/token.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/token.exe" 
LINK32_OBJS= \
	"$(INTDIR)\token.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\token.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tss1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tss1\Release"
# PROP BASE Intermediate_Dir "tss1\Release"
# PROP BASE Target_Dir "tss1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "tss1"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\tss1.exe"

CLEAN : 
	-@erase "$(INTDIR)\tss1.obj"
	-@erase "$(OUTDIR)\tss1.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tss1.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tss1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/tss1.pdb" /machine:I386\
 /out:"$(OUTDIR)/tss1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\tss1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\tss1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tss1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tss1\Debug"
# PROP BASE Intermediate_Dir "tss1\Debug"
# PROP BASE Target_Dir "tss1"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "tss1"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\tss1.exe"

CLEAN : 
	-@erase "$(INTDIR)\tss1.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\tss1.exe"
	-@erase "$(OUTDIR)\tss1.ilk"
	-@erase "$(OUTDIR)\tss1.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tss1.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tss1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/tss1.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/tss1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\tss1.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\tss1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tss2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tss2\Release"
# PROP BASE Intermediate_Dir "tss2\Release"
# PROP BASE Target_Dir "tss2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "tss2"
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\tss2.exe"

CLEAN : 
	-@erase "$(INTDIR)\tss2.obj"
	-@erase "$(OUTDIR)\tss2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tss2.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tss2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/tss2.pdb" /machine:I386\
 /out:"$(OUTDIR)/tss2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\tss2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\tss2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tss2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tss2\Debug"
# PROP BASE Intermediate_Dir "tss2\Debug"
# PROP BASE Target_Dir "tss2"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug/"
# PROP Target_Dir "tss2"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\tss2.exe"

CLEAN : 
	-@erase "$(INTDIR)\tss2.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\tss2.exe"
	-@erase "$(OUTDIR)\tss2.ilk"
	-@erase "$(OUTDIR)\tss2.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tss2.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tss2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/tss2.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/tss2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\tss2.obj" \
	"..\..\..\t127\ace\ace.lib"

"$(OUTDIR)\tss2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "_build_all_ - Win32 Release"
# Name "_build_all_ - Win32 Debug"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

!ENDIF 

################################################################################
# Begin Project Dependency

# Project_Dep_Name "auto_event"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"auto_event - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="auto_event - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"auto_event - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="auto_event - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "barrier1"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"barrier1 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="barrier1 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"barrier1 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="barrier1 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "barrier2"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"barrier2 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="barrier2 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"barrier2 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="barrier2 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "cancel"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"cancel - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="cancel - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"cancel - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="cancel - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "future1"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"future1 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="future1 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"future1 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="future1 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "future2"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"future2 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="future2 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"future2 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="future2 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "manual_event"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"manual_event - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="manual_event - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"manual_event - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="manual_event - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "process_mutex"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"process_mutex - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="process_mutex - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"process_mutex - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="process_mutex - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "process_semaphore"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"process_semaphore - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak"\
 CFG="process_semaphore - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"process_semaphore - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="process_semaphore - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "reader_writer"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"reader_writer - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="reader_writer - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"reader_writer - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="reader_writer - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "recursive_mutex"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"recursive_mutex - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="recursive_mutex - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"recursive_mutex - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="recursive_mutex - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "task_one"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"task_one - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_one - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"task_one - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_one - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "task_two"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"task_two - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_two - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"task_two - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_two - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "task_three"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"task_three - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_three - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"task_three - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_three - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "task_four"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"task_four - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_four - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"task_four - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="task_four - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "thread_manager"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"thread_manager - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_manager - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"thread_manager - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_manager - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "thread_pool"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"thread_pool - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_pool - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"thread_pool - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_pool - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "thread_specific"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"thread_specific - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_specific - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"thread_specific - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="thread_specific - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "token"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"token - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="token - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"token - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="token - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "tss1"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"tss1 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="tss1 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"tss1 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="tss1 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "tss2"

!IF  "$(CFG)" == "_build_all_ - Win32 Release"

"tss2 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="tss2 - Win32 Release" 

!ELSEIF  "$(CFG)" == "_build_all_ - Win32 Debug"

"tss2 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\test.mak" CFG="tss2 - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "auto_event - Win32 Release"
# Name "auto_event - Win32 Debug"

!IF  "$(CFG)" == "auto_event - Win32 Release"

!ELSEIF  "$(CFG)" == "auto_event - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\auto_event.cpp
DEP_CPP_TEST_=\
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
	{$(INCLUDE)}"\ace\Singleton.cpp"\
	{$(INCLUDE)}"\ace\Singleton.h"\
	{$(INCLUDE)}"\ace\Singleton.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\auto_event.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "auto_event - Win32 Release"

!ELSEIF  "$(CFG)" == "auto_event - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "barrier1 - Win32 Release"
# Name "barrier1 - Win32 Debug"

!IF  "$(CFG)" == "barrier1 - Win32 Release"

!ELSEIF  "$(CFG)" == "barrier1 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\barrier1.cpp
DEP_CPP_TEST_B=\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\barrier1.obj" : $(SOURCE) $(DEP_CPP_TEST_B) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "barrier1 - Win32 Release"

!ELSEIF  "$(CFG)" == "barrier1 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "barrier2 - Win32 Release"
# Name "barrier2 - Win32 Debug"

!IF  "$(CFG)" == "barrier2 - Win32 Release"

!ELSEIF  "$(CFG)" == "barrier2 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\barrier2.cpp
DEP_CPP_TEST_BA=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\barrier2.obj" : $(SOURCE) $(DEP_CPP_TEST_BA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "barrier2 - Win32 Release"

!ELSEIF  "$(CFG)" == "barrier2 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "cancel - Win32 Release"
# Name "cancel - Win32 Debug"

!IF  "$(CFG)" == "cancel - Win32 Release"

!ELSEIF  "$(CFG)" == "cancel - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\cancel.cpp
DEP_CPP_TEST_C=\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\cancel.obj" : $(SOURCE) $(DEP_CPP_TEST_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "cancel - Win32 Release"

!ELSEIF  "$(CFG)" == "cancel - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "future1 - Win32 Release"
# Name "future1 - Win32 Debug"

!IF  "$(CFG)" == "future1 - Win32 Release"

!ELSEIF  "$(CFG)" == "future1 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\future1.cpp
DEP_CPP_TEST_F=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Activation_Queue.h"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Future.cpp"\
	{$(INCLUDE)}"\ace\Future.h"\
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
	{$(INCLUDE)}"\ace\Method_Object.h"\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\future1.obj" : $(SOURCE) $(DEP_CPP_TEST_F) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "future1 - Win32 Release"

!ELSEIF  "$(CFG)" == "future1 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "future2 - Win32 Release"
# Name "future2 - Win32 Debug"

!IF  "$(CFG)" == "future2 - Win32 Release"

!ELSEIF  "$(CFG)" == "future2 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\future2.cpp
DEP_CPP_TEST_FU=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Activation_Queue.h"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Future.cpp"\
	{$(INCLUDE)}"\ace\Future.h"\
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
	{$(INCLUDE)}"\ace\Method_Object.h"\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\future2.obj" : $(SOURCE) $(DEP_CPP_TEST_FU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "future2 - Win32 Release"

!ELSEIF  "$(CFG)" == "future2 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "manual_event - Win32 Release"
# Name "manual_event - Win32 Debug"

!IF  "$(CFG)" == "manual_event - Win32 Release"

!ELSEIF  "$(CFG)" == "manual_event - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\manual_event.cpp
DEP_CPP_TEST_M=\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\manual_event.obj" : $(SOURCE) $(DEP_CPP_TEST_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "manual_event - Win32 Release"

!ELSEIF  "$(CFG)" == "manual_event - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "process_mutex - Win32 Release"
# Name "process_mutex - Win32 Debug"

!IF  "$(CFG)" == "process_mutex - Win32 Release"

!ELSEIF  "$(CFG)" == "process_mutex - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\process_mutex.cpp
DEP_CPP_TEST_P=\
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
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\process_mutex.obj" : $(SOURCE) $(DEP_CPP_TEST_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "process_mutex - Win32 Release"

!ELSEIF  "$(CFG)" == "process_mutex - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "process_semaphore - Win32 Release"
# Name "process_semaphore - Win32 Debug"

!IF  "$(CFG)" == "process_semaphore - Win32 Release"

!ELSEIF  "$(CFG)" == "process_semaphore - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\process_semaphore.cpp
DEP_CPP_TEST_PR=\
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
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\process_semaphore.obj" : $(SOURCE) $(DEP_CPP_TEST_PR)\
 "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "process_semaphore - Win32 Release"

!ELSEIF  "$(CFG)" == "process_semaphore - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "reader_writer - Win32 Release"
# Name "reader_writer - Win32 Debug"

!IF  "$(CFG)" == "reader_writer - Win32 Release"

!ELSEIF  "$(CFG)" == "reader_writer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\reader_writer.cpp
DEP_CPP_TEST_R=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
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
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\reader_writer.obj" : $(SOURCE) $(DEP_CPP_TEST_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "reader_writer - Win32 Release"

!ELSEIF  "$(CFG)" == "reader_writer - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "recursive_mutex - Win32 Release"
# Name "recursive_mutex - Win32 Debug"

!IF  "$(CFG)" == "recursive_mutex - Win32 Release"

!ELSEIF  "$(CFG)" == "recursive_mutex - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\recursive_mutex.cpp
DEP_CPP_TEST_RE=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\recursive_mutex.obj" : $(SOURCE) $(DEP_CPP_TEST_RE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "recursive_mutex - Win32 Release"

!ELSEIF  "$(CFG)" == "recursive_mutex - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "task_one - Win32 Release"
# Name "task_one - Win32 Debug"

!IF  "$(CFG)" == "task_one - Win32 Release"

!ELSEIF  "$(CFG)" == "task_one - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\task_one.cpp
DEP_CPP_TEST_T=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\task_one.obj" : $(SOURCE) $(DEP_CPP_TEST_T) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "task_one - Win32 Release"

!ELSEIF  "$(CFG)" == "task_one - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "task_two - Win32 Release"
# Name "task_two - Win32 Debug"

!IF  "$(CFG)" == "task_two - Win32 Release"

!ELSEIF  "$(CFG)" == "task_two - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\task_two.cpp
DEP_CPP_TEST_TA=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\task_two.obj" : $(SOURCE) $(DEP_CPP_TEST_TA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "task_two - Win32 Release"

!ELSEIF  "$(CFG)" == "task_two - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "task_three - Win32 Release"
# Name "task_three - Win32 Debug"

!IF  "$(CFG)" == "task_three - Win32 Release"

!ELSEIF  "$(CFG)" == "task_three - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\task_three.cpp
DEP_CPP_TEST_TAS=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\task_three.obj" : $(SOURCE) $(DEP_CPP_TEST_TAS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "task_three - Win32 Release"

!ELSEIF  "$(CFG)" == "task_three - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "task_four - Win32 Release"
# Name "task_four - Win32 Debug"

!IF  "$(CFG)" == "task_four - Win32 Release"

!ELSEIF  "$(CFG)" == "task_four - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\task_four.cpp
DEP_CPP_TEST_TASK=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\task_four.obj" : $(SOURCE) $(DEP_CPP_TEST_TASK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "task_four - Win32 Release"

!ELSEIF  "$(CFG)" == "task_four - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "thread_manager - Win32 Release"
# Name "thread_manager - Win32 Debug"

!IF  "$(CFG)" == "thread_manager - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_manager - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\thread_manager.cpp
DEP_CPP_TEST_TH=\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\thread_manager.obj" : $(SOURCE) $(DEP_CPP_TEST_TH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "thread_manager - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_manager - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "thread_pool - Win32 Release"
# Name "thread_pool - Win32 Debug"

!IF  "$(CFG)" == "thread_pool - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_pool - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\thread_pool.cpp
DEP_CPP_TEST_THR=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\thread_pool.obj" : $(SOURCE) $(DEP_CPP_TEST_THR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "thread_pool - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_pool - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "thread_specific - Win32 Release"
# Name "thread_specific - Win32 Debug"

!IF  "$(CFG)" == "thread_specific - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_specific - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\thread_specific.cpp
DEP_CPP_TEST_THRE=\
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
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\thread_specific.obj" : $(SOURCE) $(DEP_CPP_TEST_THRE)\
 "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "thread_specific - Win32 Release"

!ELSEIF  "$(CFG)" == "thread_specific - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "token - Win32 Release"
# Name "token - Win32 Debug"

!IF  "$(CFG)" == "token - Win32 Release"

!ELSEIF  "$(CFG)" == "token - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\token.cpp
DEP_CPP_TEST_TO=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\token.obj" : $(SOURCE) $(DEP_CPP_TEST_TO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "token - Win32 Release"

!ELSEIF  "$(CFG)" == "token - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "tss1 - Win32 Release"
# Name "tss1 - Win32 Debug"

!IF  "$(CFG)" == "tss1 - Win32 Release"

!ELSEIF  "$(CFG)" == "tss1 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tss1.cpp
DEP_CPP_TEST_TS=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\tss1.obj" : $(SOURCE) $(DEP_CPP_TEST_TS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "tss1 - Win32 Release"

!ELSEIF  "$(CFG)" == "tss1 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "tss2 - Win32 Release"
# Name "tss2 - Win32 Debug"

!IF  "$(CFG)" == "tss2 - Win32 Release"

!ELSEIF  "$(CFG)" == "tss2 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tss2.cpp
DEP_CPP_TEST_TSS=\
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
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Strategies.h"\
	{$(INCLUDE)}"\ace\Strategies_T.cpp"\
	{$(INCLUDE)}"\ace\Strategies_T.h"\
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
	{$(INCLUDE)}"\ace\Task.h"\
	{$(INCLUDE)}"\ace\Task.i"\
	{$(INCLUDE)}"\ace\Task_T.cpp"\
	{$(INCLUDE)}"\ace\Task_T.h"\
	{$(INCLUDE)}"\ace\Task_T.i"\
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
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\tss2.obj" : $(SOURCE) $(DEP_CPP_TEST_TSS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\T12x\t127\ace\ace.lib

!IF  "$(CFG)" == "tss2 - Win32 Release"

!ELSEIF  "$(CFG)" == "tss2 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
