# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=test_registry_update - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to test_registry_update -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "test_registry_update - Win32 Release" && "$(CFG)" !=\
 "test_registry_update - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "test_registry_update.mak"\
 CFG="test_registry_update - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test_registry_update - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "test_registry_update - Win32 Debug" (based on\
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
# PROP Target_Last_Scanned "test_registry_update - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "test_registry_update - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\test_registry_update.exe"

CLEAN : 
	-@erase ".\test_registry_update.exe"
	-@erase ".\test_registry_update.obj"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I ".." /I " ..\stl" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /O2 /I ".." /I " ..\stl" /D "WIN32" /D "NDEBUG"\
 /D "_CONSOLE" /Fp"test_registry_update.pch" /YX /c 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test_registry_update.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib c:\users\irfan\registry\ace\debug\registry.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib c:\users\irfan\registry\ace\debug\registry.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/test_registry_update.pdb"\
 /machine:I386 /out:"$(OUTDIR)/test_registry_update.exe" 
LINK32_OBJS= \
	"$(INTDIR)/test_registry_update.obj"

"$(OUTDIR)\test_registry_update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "test_registry_update - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\test_registry_update.exe"

CLEAN : 
	-@erase ".\vc40.pdb"
	-@erase ".\vc40.idb"
	-@erase ".\test_registry_update.exe"
	-@erase ".\test_registry_update.obj"
	-@erase ".\test_registry_update.ilk"
	-@erase ".\test_registry_update.pdb"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I " ..\stl" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I " ..\stl" /D "WIN32" /D\
 "_DEBUG" /D "_CONSOLE" /Fp"test_registry_update.pch" /YX /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test_registry_update.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib c:\users\irfan\registry\ace\debug\registry.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib c:\users\irfan\registry\ace\debug\registry.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/test_registry_update.pdb"\
 /debug /machine:I386 /out:"$(OUTDIR)/test_registry_update.exe" 
LINK32_OBJS= \
	"$(INTDIR)/test_registry_update.obj"

"$(OUTDIR)\test_registry_update.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

.c.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "test_registry_update - Win32 Release"
# Name "test_registry_update - Win32 Debug"

!IF  "$(CFG)" == "test_registry_update - Win32 Release"

!ELSEIF  "$(CFG)" == "test_registry_update - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\test_registry_update.cpp
DEP_CPP_TEST_=\
	".\..\ace/Registry.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\bstring.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\ACE.i"\
	
NODEP_CPP_TEST_=\
	".\..\..\stl\msdev\mutex.h"\
	

"$(INTDIR)\test_registry_update.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
