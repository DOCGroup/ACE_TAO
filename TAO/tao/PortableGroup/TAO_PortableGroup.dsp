# Microsoft Developer Studio Project File - Name="PortableGroup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PortableGroup - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableGroup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableGroup.mak" CFG="PortableGroup - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PortableGroup - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableGroup"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableGroup_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 ace.lib TAO.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroup.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableGroup"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableGroup_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /I "../../orbsvcs/orbsvcs" /D "_DEBUG" /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib TAO_CosNamingd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PortableGroup___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "PortableGroup___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableGroupMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTSchedd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupmfcd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PortableGroup___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "PortableGroup___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableGroupMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTSched.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroup.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfc.lib acemfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupmfc.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "PortableGroup - Win32 Release"
# Name "PortableGroup - Win32 Debug"
# Name "PortableGroup - Win32 MFC Debug"
# Name "PortableGroup - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\miop.cpp
# End Source File
# Begin Source File

SOURCE=.\miopC.cpp
# End Source File
# Begin Source File

SOURCE=.\POA_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\Portable_Group_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Acceptor_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Request_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Transport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\miop.h
# End Source File
# Begin Source File

SOURCE=.\miopC.h
# End Source File
# Begin Source File

SOURCE=.\PGA.h
# End Source File
# Begin Source File

SOURCE=.\PGA_Loader.h
# End Source File
# Begin Source File

SOURCE=.\POA_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\Portable_Group_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Acceptor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\portablegroup_export.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Loader.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Request_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Profile.h
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Transport.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\miopC.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.i
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Profile.i
# End Source File
# Begin Source File

SOURCE=.\UIPMC_Transport.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\mgm.pidl
# End Source File
# Begin Source File

SOURCE=.\miop.pidl
# End Source File
# Begin Source File

SOURCE=.\PortableGroup.pidl
# End Source File
# End Target
# End Project
