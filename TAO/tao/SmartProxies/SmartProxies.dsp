# Microsoft Developer Studio Project File - Name="SmartProxies" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SmartProxies - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "SmartProxies.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "SmartProxies.mak" CFG="SmartProxies - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "SmartProxies - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SmartProxies - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SmartProxies - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\SmartProxies"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D TAO_SMARTPROXIES_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_SmartProxies.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\lib\TAO_SmartProxies.pdb" /dll  /machine:I386 /out:"..\..\..\lib\TAO_SmartProxies.dll"

!ELSEIF  "$(CFG)" == "SmartProxies - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\SmartProxies"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D TAO_SMARTPROXIES_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_SmartProxies.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\lib\TAO_SmartProxiesd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\TAO_SmartProxiesd.dll"

!ENDIF

# Begin Target

# Name "SmartProxies - Win32 Release"
# Name "SmartProxies - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\Smart_Proxies.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\Smart_Proxies.h"
# End Source File
# Begin Source File

SOURCE=".\smartproxies_export.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\Smart_Proxies.inl"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\SmartProxies.rc"
# End Source File
# End Group
# End Target
# End Project
