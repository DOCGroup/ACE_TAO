# Microsoft Developer Studio Project File - Name="TAO_RT_NotifyTests_Filter DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_RT_NotifyTests_Filter DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter.mak" CFG="TAO_RT_NotifyTests_Filter DLL - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "TAO_RT_NotifyTests_Filter DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_RT_NotifyTests_Filter DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_RT_NotifyTests_Filter DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /Zi /Ob2 /GR /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_RT_Test_Filter_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_NotifyTests_Filter.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTCORBA.lib TAO_CosNotification.lib TAO_ETCL.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_DynamicAny.lib TAO_PortableServer.lib TAO.lib ACE.lib /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:windows /dll  /machine:I386 /out:"..\..\..\..\..\bin\TAO_RT_NotifyTests_Filter.dll"

!ELSEIF  "$(CFG)" == "TAO_RT_NotifyTests_Filter DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_RT_Test_Filter_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_NotifyTests_Filter.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTCORBAd.lib TAO_CosNotificationd.lib TAO_ETCLd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_DynamicAnyd.lib TAO_PortableServerd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:windows /dll /debug /pdb:TAO_RT_NotifyTests_Filterd.pdb /machine:I386 /out:"..\..\..\..\..\bin\TAO_RT_NotifyTests_Filterd.dll"

!ENDIF

# Begin Target

# Name "TAO_RT_NotifyTests_Filter DLL - Win32 Release"
# Name "TAO_RT_NotifyTests_Filter DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\RT_Test_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Test_FilterFactory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Command_Factory_T.h
# End Source File
# Begin Source File

SOURCE=.\RT_Test_Filter.h
# End Source File
# Begin Source File

SOURCE=.\RT_Test_FilterFactory.h
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
