# Microsoft Developer Studio Project File - Name="TAO_RT_NotifyTests DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_RT_NotifyTests DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RT_NotifyTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RT_NotifyTests.mak" CFG="TAO_RT_NotifyTests DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_RT_NotifyTests DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_RT_NotifyTests DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_RT_NotifyTests DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_RT_NOTIFY_TEST_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_NotifyTests.lib TAO_RTCORBA.lib TAO_RTPortableServer.lib TAO_CosNotification.lib TAO_ETCL.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_DynamicAny.lib TAO_PortableServer.lib TAO.lib ACE.lib /nologo /version:1.3 /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\bin\TAO_RT_NotifyTests.dll" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\tao\RTPortableServer" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\lib"

!ELSEIF  "$(CFG)" == "TAO_RT_NotifyTests DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_RT_NOTIFY_TEST_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_NotifyTestsd.lib TAO_RTCORBAd.lib TAO_RTPortableServerd.lib TAO_CosNotificationd.lib TAO_ETCLd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_DynamicAnyd.lib TAO_PortableServerd.lib TAOd.lib ACEd.lib /nologo /version:1.3 /subsystem:windows /dll /incremental:no /pdb:"TAO_RT_NotifyTestsd.pdb" /debug /machine:I386 /out:"..\..\..\..\..\bin\TAO_RT_NotifyTestsd.dll" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\tao\RTPortableServer" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TAO_RT_NotifyTests DLL - Win32 Release"
# Name "TAO_RT_NotifyTests DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\RT_Application_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Factories_Define.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_POA_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Priority_Mapping.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Command_Factory_T.h
# End Source File
# Begin Source File

SOURCE=.\RT_Application_Command.h
# End Source File
# Begin Source File

SOURCE=.\RT_POA_Command.h
# End Source File
# Begin Source File

SOURCE=.\RT_Priority_Mapping.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Activation_ManagerC.i
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerS.i
# End Source File
# Begin Source File

SOURCE=.\Task_Stats.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Command_Factory_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# End Target
# End Project
