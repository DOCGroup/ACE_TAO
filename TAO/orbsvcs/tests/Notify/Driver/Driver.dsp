# Microsoft Developer Studio Project File - Name="Driver EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Driver EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Driver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Driver.mak" CFG="Driver EXE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Driver EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Driver EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Driver EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_NotifyTests.lib TAO.lib ACE.lib /nologo /subsystem:console /machine:I386 /out:"Release/Notify_Tests_Driver.exe" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /version:1.3.1

!ELSEIF  "$(CFG)" == "Driver EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_NotifyTestsd.lib TAOd.lib ACEd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"Notify_Tests_Driver.exe" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /version:1.3.1

!ENDIF 

# Begin Target

# Name "Driver EXE - Win32 Release"
# Name "Driver EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# End Target
# End Project
