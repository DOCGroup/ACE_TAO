# Microsoft Developer Studio Project File - Name="NamingViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NamingViewer - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NamingViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NamingViewer.mak" CFG="NamingViewer - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NamingViewer - Win32 MFC Release" (based on "Win32 (x86) Application")
!MESSAGE "NamingViewer - Win32 MFC Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NamingViewer - Win32 MFC Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../../../" /I "../../orbsvcs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D ACE_HAS_MFC=1 /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 TAO_PortableServer.lib tao.lib ace.lib TAO_CosNaming.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../tao/PortableServer" /libpath:"../../tao" /libpath:"../../../ace" /libpath:"../../orbsvcs/orbsvcs"

!ELSEIF  "$(CFG)" == "NamingViewer - Win32 MFC Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../" /I "../../../" /I "../../orbsvcs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D ACE_HAS_MFC=1 /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_PortableServerd.lib taod.lib aced.lib TAO_CosNamingd.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/NamingViewerd.exe" /pdbtype:sept /libpath:"../../tao/PortableServer" /libpath:"../../tao" /libpath:"../../../ace" /libpath:"../../orbsvcs/orbsvcs"

!ENDIF 

# Begin Target

# Name "NamingViewer - Win32 MFC Release"
# Name "NamingViewer - Win32 MFC Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddNameServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BindDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BindNewContext.cpp
# End Source File
# Begin Source File

SOURCE=.\NamingObject.cpp
# End Source File
# Begin Source File

SOURCE=.\NamingTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NamingViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\NamingViewer.rc
# End Source File
# Begin Source File

SOURCE=.\NamingViewerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectNSDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ViewIORDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddNameServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\BindDialog.h
# End Source File
# Begin Source File

SOURCE=.\BindNewContext.h
# End Source File
# Begin Source File

SOURCE=.\NamingObject.h
# End Source File
# Begin Source File

SOURCE=.\NamingTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NamingViewer.h
# End Source File
# Begin Source File

SOURCE=.\NamingViewerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectNSDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ViewIORDialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\NamingViewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\NamingViewer.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
