# Microsoft Developer Studio Project File - Name="Notify_Performance_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Notify_Performance_Test - Win32 PharLap ETS Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Notify_Performance_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Notify_Performance_Test.mak" CFG="Notify_Performance_Test - Win32 PharLap ETS Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Notify_Performance_Test - Win32 PharLap ETS Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Notify_Performance_Test___Win32_PharLap_ETS_Debug"
# PROP BASE Intermediate_Dir "Notify_Performance_Test___Win32_PharLap_ETS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../../" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D ACE_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 acesd.lib /nologo /subsystem:console /debug /machine:I386 /out:"./Notify_Performance_Test_ETS.exe" /pdbtype:sept /libpath:"../../ace" /ETS:tests_pharlap_msvc.lnk
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "Notify_Performance_Test - Win32 PharLap ETS Debug"
# Begin Source File

SOURCE=..\Notify_Performance_Test.cpp
# End Source File
# End Target
# End Project
