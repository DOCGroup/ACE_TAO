# Microsoft Developer Studio Project File - Name="RTCORBAEvent" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTCORBAEvent - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTCORBAEvent.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTCORBAEvent.mak" CFG="RTCORBAEvent - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTCORBAEvent - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBAEvent - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBAEvent - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCORBAEvent - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCORBAEvent - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTCORBAEvent"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTCORBAEvent_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_RTPortableServer.lib TAO_RTCORBA.lib TAO_Messaging.lib TAO_Svc_Utils.lib ace.lib TAO.lib TAO_PortableServer.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEvent.dll" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBAEvent - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTCORBAEvent"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTCORBAEvent_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_RTPortableServerd.lib TAO_RTCORBAd.lib TAO_Svc_Utilsd.lib TAOd.lib aced.lib TAO_PortableServerd.lib TAO_Messagingd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEventd.dll" /pdbtype:sept /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBAEvent - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTCORBAEvent___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "RTCORBAEvent___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTCORBAEventMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEventd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_RTPortableServermfcd.lib TAO_RTCORBAmfcd.lib TAOmfcd.lib acemfcd.lib TAO_Svc_Utilsmfcd.lib TAO_PortableServermfcd.lib TAO_Messagingmfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEventmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTCORBAEvent - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTCORBAEvent___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "RTCORBAEvent___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTCORBAEventMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTCORBAEVENT_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEvent.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_RTPortableServermfc.lib TAO_RTCORBAmfc.lib acemfc.lib TAOmfc.lib TAO_Svc_Utilsmfc.lib TAO_PortableServermfc.lib TAO_Messagingmfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTCORBAEventmfc.dll" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "RTCORBAEvent - Win32 Release"
# Name "RTCORBAEvent - Win32 Debug"
# Name "RTCORBAEvent - Win32 MFC Debug"
# Name "RTCORBAEvent - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Factory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Factory.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Dispatching.inl
# End Source File
# Begin Source File

SOURCE=.\Event\EC_RTCORBA_Factory.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# End Group
# End Target
# End Project
