# Microsoft Developer Studio Project File - Name="TypeCodeFactory" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TypeCodeFactory - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "TypeCodeFactory.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "TypeCodeFactory.mak" CFG="TypeCodeFactory - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "TypeCodeFactory - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TypeCodeFactory - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TypeCodeFactory - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\TypeCodeFactory"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D TAO_TYPECODEFACTORY_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_TypeCodeFactory.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Client.lib TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\lib\TAO_TypeCodeFactory.pdb" /dll  /machine:I386 /out:"..\..\..\lib\TAO_TypeCodeFactory.dll"

!ELSEIF  "$(CFG)" == "TypeCodeFactory - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\TypeCodeFactory"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D TAO_TYPECODEFACTORY_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_TypeCodeFactory.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Clientd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\lib\TAO_TypeCodeFactoryd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\TAO_TypeCodeFactoryd.dll"

!ENDIF

# Begin Target

# Name "TypeCodeFactory - Win32 Release"
# Name "TypeCodeFactory - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\TypeCodeFactory_Adapter_Impl.cpp"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactory_i.cpp"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactory_Loader.cpp"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactoryC.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\TypeCodeFactory_Adapter_Impl.h"
# End Source File
# Begin Source File

SOURCE=".\typecodefactory_export.h"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactory_i.h"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactory_Loader.h"
# End Source File
# Begin Source File

SOURCE=".\TypeCodeFactoryC.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\TypeCodeFactoryC.inl"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\TypeCodeFactory.rc"
# End Source File
# End Group
# End Target
# End Project
