# Microsoft Developer Studio Project File - Name="JAWS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JAWS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JAWS.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JAWS.MAK" CFG="JAWS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JAWS - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JAWS - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JAWS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\.." /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D JAWS_HAS_DLL=1 /D "JAWS_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\bin\JAWS.dll" /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "JAWS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\.." /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D JAWS_HAS_DLL=1 /D "JAWS_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\bin\JAWSd.dll" /pdbtype:sept /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "JAWS - Win32 Release"
# Name "JAWS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Concurrency.cpp
# End Source File
# Begin Source File

SOURCE=.\Data_Block.cpp
# End Source File
# Begin Source File

SOURCE=.\Headers.cpp
# End Source File
# Begin Source File

SOURCE=.\IO.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse_Headers.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\Reaper.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Waiter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Assoc_Array.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency.h
# End Source File
# Begin Source File

SOURCE=.\Data_Block.h
# End Source File
# Begin Source File

SOURCE=.\Export.h
# End Source File
# Begin Source File

SOURCE=.\IO.h
# End Source File
# Begin Source File

SOURCE=.\IO_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IO_Handler.h
# End Source File
# Begin Source File

SOURCE=.\IO_State.h
# End Source File
# Begin Source File

SOURCE=.\JAWS.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\Policy.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\Waiter.h
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Assoc_Array.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Handler.cpp
# End Source File
# End Group
# End Target
# End Project
