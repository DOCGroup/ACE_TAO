# Microsoft Developer Studio Project File - Name="Export_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Export_Test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Export_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Export_Test.mak" CFG="Export_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Export_Test - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Export_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Export_Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Export_T"
# PROP BASE Intermediate_Dir "Export_T"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /I "..\..\..\..\\" /I "..\..\\" /D "NDEBUG" /D TAO_ORBSVCS_HAS_DLL=1 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib orbsvcs.lib TradingLib.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\orbsvcs"

!ELSEIF  "$(CFG)" == "Export_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Export_0"
# PROP BASE Intermediate_Dir "Export_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /I "..\..\..\..\\" /D "_DEBUG" /D TAO_ORBSVCS_HAS_DLL=1 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib orbsvcsd.lib TradingLibd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\orbsvcs"

!ENDIF 

# Begin Target

# Name "Export_Test - Win32 Release"
# Name "Export_Test - Win32 Debug"
# Begin Source File

SOURCE=.\export_test.cpp
# End Source File
# End Target
# End Project
