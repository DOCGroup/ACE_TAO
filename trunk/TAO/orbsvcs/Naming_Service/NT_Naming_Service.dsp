# Microsoft Developer Studio Project File - Name="NT_Naming_Service" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=NT_Naming_Service - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NT_Naming_Service.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NT_Naming_Service.mak" CFG="NT_Naming_Service - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NT_Naming_Service - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "NT_Naming_Service - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NT_Naming_Service - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 user32.lib advapi32.lib TAO_CosNaming.lib TAO_IORTable.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "NT_Naming_Service - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_CosNamingd.lib user32.lib advapi32.lib TAO_IORTabled.lib TAO_PortableServerd.lib TAOd.lib aced.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "NT_Naming_Service - Win32 Release"
# Name "NT_Naming_Service - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Naming_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\NT_Naming_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\NT_Naming_Service.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Naming_Service.h
# End Source File
# Begin Source File

SOURCE=.\NT_Naming_Service.h
# End Source File
# End Group
# End Target
# End Project
