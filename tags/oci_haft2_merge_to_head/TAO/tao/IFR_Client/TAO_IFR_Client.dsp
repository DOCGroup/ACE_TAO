# Microsoft Developer Studio Project File - Name="IFR_Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IFR_Client - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IFR_Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IFR_Client.mak" CFG="IFR_Client - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IFR_Client - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFR_Client - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFR_Client - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFR_Client - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IFR_Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\IFR_Client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IFR_BASIC_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO.lib ace.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_IFR_Client.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFR_Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IFR_Client___Win32_Debug"
# PROP BASE Intermediate_Dir "IFR_Client___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\IFR_Client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IFR_BASIC_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFR_Clientd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFR_Client - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IFR_Client___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "IFR_Client___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Debug\IFR_Client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /D ACE_HAS_MFC=1 /D "_WINDLL" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFR_Clientd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFR_Clientmfcd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFR_Client - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IFR_Client___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "IFR_Client___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Release\IFR_Client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFR_CLIENT_BUILD_DLL" /D ACE_HAS_MFC=1 /D "_WINDLL" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_IFR_Client.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfc.lib acemfc.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_IFR_Clientmfc.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "IFR_Client - Win32 Release"
# Name "IFR_Client - Win32 Debug"
# Name "IFR_Client - Win32 MFC Debug"
# Name "IFR_Client - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IFR_BaseC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.cpp
# ADD CPP /GR
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IFR_BaseC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter_Impl.h
# End Source File
# Begin Source File

SOURCE=.\ifr_client_export.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\IFR_BaseC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\TAO_IFR_Client.rc
# End Source File
# End Group
# End Target
# End Project
