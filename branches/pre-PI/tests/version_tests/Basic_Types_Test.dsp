# Microsoft Developer Studio Project File - Name="Basic_Types_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Basic_Types_Test - Win32 static Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Basic_Types_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Basic_Types_Test.mak" CFG="Basic_Types_Test - Win32 static Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Basic_Types_Test - Win32 DLL Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 DLL Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 static Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 DLL Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 DLL Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 static Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Basic_Types_Test - Win32 static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Basic_Types_Test - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DLL Debug"
# PROP BASE Intermediate_Dir "DLL Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL Debug"
# PROP Intermediate_Dir ".\DLL Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DLL Unicode Debug"
# PROP BASE Intermediate_Dir "DLL Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL Unicode Debug"
# PROP Intermediate_Dir ".\DLL Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=1 /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aceud.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "static Debug"
# PROP BASE Intermediate_Dir "static Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\static Debug"
# PROP Intermediate_Dir ".\static Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D ACE_NO_INLINE /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 acesd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 static Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "static Unicode Debug"
# PROP BASE Intermediate_Dir "static Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\static Unicode Debug"
# PROP Intermediate_Dir ".\static Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D ACE_NO_INLINE /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 acesud.lib user32.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DLL Release"
# PROP BASE Intermediate_Dir "DLL Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\DLL Release"
# PROP Intermediate_Dir ".\DLL Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DLL Unicode Release"
# PROP BASE Intermediate_Dir "DLL Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\DLL Unicode Release"
# PROP Intermediate_Dir ".\DLL Unicode Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=1 /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 aceu.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 static Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "static Unicode Release"
# PROP BASE Intermediate_Dir "static Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\static Unicode Release"
# PROP Intermediate_Dir ".\static Unicode Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D ACE_NO_INLINE /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 acesu.lib user32.lib advapi32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Basic_Types_Test - Win32 static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "static Release"
# PROP BASE Intermediate_Dir "static Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\static Release"
# PROP Intermediate_Dir ".\static Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D ACE_NO_INLINE /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 aces.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Basic_Types_Test - Win32 DLL Debug"
# Name "Basic_Types_Test - Win32 DLL Unicode Debug"
# Name "Basic_Types_Test - Win32 static Debug"
# Name "Basic_Types_Test - Win32 static Unicode Debug"
# Name "Basic_Types_Test - Win32 DLL Release"
# Name "Basic_Types_Test - Win32 DLL Unicode Release"
# Name "Basic_Types_Test - Win32 static Unicode Release"
# Name "Basic_Types_Test - Win32 static Release"
# Begin Source File

SOURCE=..\Basic_Types_Test.cpp
# End Source File
# End Target
# End Project
