# Microsoft Developer Studio Project File - Name="IOGRTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=IOGRTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IOGRTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IOGRTest.mak" CFG="IOGRTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IOGRTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "IOGRTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IOGRTest - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\IOGRTest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_FT_ServerORB.lib TAO_FT_ClientORB.lib TAO_FTORB_Utils.lib TAO_IORManip.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /nologo /subsystem:console /machine:I386 /libpath:"../../../orbsvcs" /libpath:"../../../../tao/IORManipulation" /libpath:"../../../../tao/PortableServer" /libpath:"../../../../tao/Valuetype" /libpath:"../../../../tao/IORInterceptor" /libpath:"../../../../tao/ObjRefTemplate" /libpath:"../../../../tao" /libpath:"../../../../../ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "IOGRTest - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\IOGRTest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_FT_ServerORBd.lib TAO_FT_ClientORBd.lib TAO_FTORB_Utilsd.lib TAO_IORManipd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /libpath:"../../../orbsvcs" /libpath:"../../../../tao/IORManipulation" /libpath:"../../../../tao/PortableServer" /libpath:"../../../../tao/Valuetype" /libpath:"../../../../tao/IORInterceptor" /libpath:"../../../../tao/ObjRefTemplate" /libpath:"../../../../tao" /libpath:"../../../../../ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "IOGRTest - Win32 Release"
# Name "IOGRTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\IOGRTest.cpp"
# End Source File
# End Group
# End Target
# End Project
