# Microsoft Developer Studio Project File - Name="RTPortableServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTPortableServer - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTPortableServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTPortableServer.mak" CFG="RTPortableServer - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTPortableServer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTPortableServer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTPortableServer - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTPortableServer - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTPortableServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTPortableServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTPortableServer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "TAO_RTPORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_RTCORBA.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_RTPortableServer.dll" /libpath:"..\\" /libpath:"..\..\..\ace" /libpath:"..\PortableServer" /libpath:"..\RTCORBA"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTPortableServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTPortableServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTPortableServer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "TAO_RTPORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib TAO_PortableServerd.lib TAO_RTCORBAd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTPortableServerd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\RTCORBA"

!ELSEIF  "$(CFG)" == "RTPortableServer - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTPortableServer___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "RTPortableServer___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTPortableServerMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D ACE_HAS_MFC=1 /D "TAO_RTPORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTSchedd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTPORTABLESERVERd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_RTCORBAmfcd.lib TAOmfcd.lib acemfcd.lib TAO_PortableServermfcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTPortableServermfcd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "RTPortableServer - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTPortableServer___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "RTPortableServer___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTPortableServerMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D ACE_HAS_MFC=1 /D "TAO_RTPORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTSched.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTPORTABLESERVER.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_RTCORBAmfc.lib TAOmfc.lib acemfc.lib TAO_PortableServermfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_RTPortableServermfc.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\PortableServer"

!ENDIF 

# Begin Target

# Name "RTPortableServer - Win32 Release"
# Name "RTPortableServer - Win32 Debug"
# Name "RTPortableServer - Win32 MFC Debug"
# Name "RTPortableServer - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RT_Acceptor_Filters.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Collocation_Resolver.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Object_Adapter_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_POA.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Servant_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPortableServer.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPortableServerC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RT_Acceptor_Filters.h
# End Source File
# Begin Source File

SOURCE=.\RT_Collocation_Resolver.h
# End Source File
# Begin Source File

SOURCE=.\RT_Object_Adapter_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_POA.h
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_Validator.h
# End Source File
# Begin Source File

SOURCE=.\RT_Servant_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\RTPortableServer.h
# End Source File
# Begin Source File

SOURCE=.\rtportableserver_export.h
# End Source File
# Begin Source File

SOURCE=.\RTPortableServerC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\RT_Acceptor_Filters.i
# End Source File
# Begin Source File

SOURCE=.\RT_Collocation_Resolver.i
# End Source File
# Begin Source File

SOURCE=.\RT_POA.i
# End Source File
# Begin Source File

SOURCE=.\RTPortableServerC.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\TAO_RTPortableServer.rc
# End Source File
# End Group
# End Target
# End Project
