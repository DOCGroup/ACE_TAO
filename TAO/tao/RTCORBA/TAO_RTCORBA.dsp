# Microsoft Developer Studio Project File - Name="RTCORBA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTCORBA - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTCORBA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTCORBA.mak" CFG="RTCORBA - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTCORBA - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBA - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBA - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBA - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCORBA - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTCORBA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTCORBA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "TAO_RTCORBA_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBA.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBA - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTCORBA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTCORBA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "TAO_RTCORBA_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBA - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTCORBA___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "RTCORBA___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTCORBAMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D ACE_HAS_MFC=1 /D "TAO_RTCORBA_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTSchedd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAmfcd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBA - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTCORBA___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "RTCORBA___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTCORBAMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D ACE_HAS_MFC=1 /D "TAO_RTCORBA_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTSched.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTCORBA.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfc.lib acemfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAmfc.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "RTCORBA - Win32 Release"
# Name "RTCORBA - Win32 Debug"
# Name "RTCORBA - Win32 MFC Debug"
# Name "RTCORBA - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Current.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Selector_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORB_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Stub.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Stub_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBA.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.h
# End Source File
# Begin Source File

SOURCE=.\RT_Current.h
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Selector_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Utils.h
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.h
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB_Loader.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\RT_Stub.h
# End Source File
# Begin Source File

SOURCE=.\RT_Stub_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBA.h
# End Source File
# Begin Source File

SOURCE=.\rtcorba_export.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.inl
# End Source File
# Begin Source File

SOURCE=.\RT_Current.i
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.i
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.i
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.i
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.i
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\TAO_RTCORBA.rc
# End Source File
# End Group
# End Target
# End Project
