# Microsoft Developer Studio Project File - Name="Event_Service" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Event_Service - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Event_Service.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Event_Service.mak" CFG="Event_Service - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Event_Service - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Event_Service - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Event_Service - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Event_Service - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 orbsvcs.lib TAO.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "Event_Service - Win32 Release"
# Name "Event_Service - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCU.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBA_Utils_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispatching_Modules.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\GPlot_File.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_ESTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\Memory_Pools.cpp
# End Source File
# Begin Source File

SOURCE=.\ReactorTask.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Task_Manager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCU.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Utils_T.h
# End Source File
# Begin Source File

SOURCE=.\Debug_Macros.h
# End Source File
# Begin Source File

SOURCE=.\Dispatching_Modules.h
# End Source File
# Begin Source File

SOURCE=.\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Fast_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\GPlot_File.h
# End Source File
# Begin Source File

SOURCE=.\Local_ESTypes.h
# End Source File
# Begin Source File

SOURCE=.\Memory_Pools.h
# End Source File
# Begin Source File

SOURCE=.\ReactorTask.h
# End Source File
# Begin Source File

SOURCE=.\RT_Task.h
# End Source File
# Begin Source File

SOURCE=.\Task_Manager.h
# End Source File
# End Group
# End Target
# End Project
