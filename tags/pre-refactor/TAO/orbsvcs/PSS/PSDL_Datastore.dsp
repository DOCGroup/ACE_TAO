# Microsoft Developer Studio Project File - Name="PSDL_Datastore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PSDL_Datastore - Win32 mfc Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PSDL_Datastore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PSDL_Datastore.mak" CFG="PSDL_Datastore - Win32 mfc Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PSDL_Datastore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PSDL_Datastore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PSDL_Datastore - Win32 mfc Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PSDL_Datastore - Win32 mfc Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PSDL_Datastore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PSDL_Datastore"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PSDL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_PSDL_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO_Svc_Utils.lib TAO_IORTable.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastore.dll" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\orbsvcs"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PSDL_Datastore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PSDL_Datastore"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PSDL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_PSDL_BUILD_DLL" /D "TAO_ORBSVCS_HAS_PSDL" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastored.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "PSDL_Datastore - Win32 mfc Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PSDL_Datastore___Win32_mfc_Release"
# PROP BASE Intermediate_Dir "PSDL_Datastore___Win32_mfc_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PSDL_DatastoreMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_PSDL_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_PSDL_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastore.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acemfc.lib TAOmfc.lib TAO_Svc_Utilsmfc.lib TAO_PortableServermfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastoremfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PSDL_Datastore - Win32 mfc Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PSDL_Datastore___Win32_mfc_Debug"
# PROP BASE Intermediate_Dir "PSDL_Datastore___Win32_mfc_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PSDL_DatastoreMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_PSDL_BUILD_DLL" /D "TAO_ORBSVCS_HAS_PSDL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_PSDL_BUILD_DLL" /D "TAO_ORBSVCS_HAS_PSDL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastored.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 TAOmfcd.lib acemfcd.lib TAO_Svc_Utilsmfcd.lib TAO_PortableServermfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PSDL_Datastoremfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "PSDL_Datastore - Win32 Release"
# Name "PSDL_Datastore - Win32 Debug"
# Name "PSDL_Datastore - Win32 mfc Release"
# Name "PSDL_Datastore - Win32 mfc Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PSDL_Code_Gen.cpp
# End Source File
# Begin Source File

SOURCE=.\PSDL_Datastore.cpp
# End Source File
# Begin Source File

SOURCE=.\PSDL_OctetSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\PSDL_String.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PSDL_Code_Gen.h
# End Source File
# Begin Source File

SOURCE=.\PSDL_Datastore.h
# End Source File
# Begin Source File

SOURCE=.\PSDL_OctetSeq.h
# End Source File
# Begin Source File

SOURCE=.\PSDL_String.h
# End Source File
# End Group
# End Target
# End Project
