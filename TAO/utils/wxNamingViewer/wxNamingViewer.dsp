# Microsoft Developer Studio Project File - Name="wxNamingViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxNamingViewer - Win32 Wx Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxNamingViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxNamingViewer.mak" CFG="wxNamingViewer - Win32 Wx Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxNamingViewer - Win32 Wx Release" (based on "Win32 (x86) Application")
!MESSAGE "wxNamingViewer - Win32 Wx Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxNamingViewer - Win32 Wx Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /I "../../" /I "../../orbsvcs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__WXMSW__" /D "WX_PRECOMP" /YX"pch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "wxNamingViewer - Win32 Wx Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../" /I "../../" /I "../../orbsvcs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WXMSW__" /YX"pch.h" /FD  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 rpcrt4.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aced.lib TAOd.lib TAO_PortableServerd.lib TAO_CosNamingd.lib wxd.lib xpmd.lib pngd.lib zlibd.lib jpegd.lib tiffd.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../../ace" /libpath:"../../tao" /libpath:"../../tao/PortableServer" /libpath:"../../orbsvcs/orbsvcs"

!ENDIF 

# Begin Target

# Name "wxNamingViewer - Win32 Wx Release"
# Name "wxNamingViewer - Win32 Wx Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\wxAddNameServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\wxBindDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\wxBindNewContext.cpp
# End Source File
# Begin Source File

SOURCE=.\wxNamingObject.cpp
# End Source File
# Begin Source File

SOURCE=.\wxNamingTree.cpp
# End Source File
# Begin Source File

SOURCE=.\wxNamingViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\wxNamingViewer.rc
# End Source File
# Begin Source File

SOURCE=.\wxNamingViewerFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\wxSelectNSDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\wxViewIORDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\wxAddNameServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\wxAutoDialog.h
# End Source File
# Begin Source File

SOURCE=.\wxBindDialog.h
# End Source File
# Begin Source File

SOURCE=.\wxBindNewContext.h
# End Source File
# Begin Source File

SOURCE=.\wxNamingObject.h
# End Source File
# Begin Source File

SOURCE=.\wxNamingTree.h
# End Source File
# Begin Source File

SOURCE=.\wxNamingViewer.h
# End Source File
# Begin Source File

SOURCE=.\wxNamingViewerFrame.h
# End Source File
# Begin Source File

SOURCE=.\wxSelectNSDialog.h
# End Source File
# Begin Source File

SOURCE=.\wxViewIORDialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
