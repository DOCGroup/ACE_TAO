# Microsoft Developer Studio Project File - Name="TAO_RT_NotifyTests_Filter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_RT_NotifyTests_Filter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter.mak" CFG="TAO_RT_NotifyTests_Filter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_RT_NotifyTests_Filter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_RT_NotifyTests_Filter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_RT_NotifyTests_Filter - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "Release\TAO_RT_NotifyTests_Filter"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RT_Test_Filter_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_RTCORBA.lib TAO_CosNotification.lib TAO_DynamicAny.lib TAO_ETCL.lib TAO_CosEvent.lib TAO_Messaging.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO_IORTable.lib TAO.lib ACE.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\Messaging" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TAO_RT_NotifyTests_Filter - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "Debug\TAO_RT_NotifyTests_Filter"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RT_Test_Filter_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_RTCORBAd.lib TAO_CosNotificationd.lib TAO_DynamicAnyd.lib TAO_ETCLd.lib TAO_CosEventd.lib TAO_Messagingd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAO_IORTabled.lib TAOd.lib ACEd.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\..\lib\TAO_RT_NotifyTests_Filterd.dll" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\Messaging" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TAO_RT_NotifyTests_Filter - Win32 Release"
# Name "TAO_RT_NotifyTests_Filter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\RT_Test_Filter.cpp"
# End Source File
# Begin Source File

SOURCE=".\RT_Test_FilterFactory.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\RT_Test_Filter.h"
# End Source File
# Begin Source File

SOURCE=".\RT_Test_FilterFactory.h"
# End Source File
# End Group
# End Target
# End Project
