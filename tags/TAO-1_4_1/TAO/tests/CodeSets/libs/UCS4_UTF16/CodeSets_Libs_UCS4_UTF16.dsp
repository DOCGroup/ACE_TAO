# Microsoft Developer Studio Project File - Name="CodeSets_Libs_UCS4_UTF16" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CodeSets_Libs_UCS4_UTF16 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CodeSets_libs_UCS4_UTF16.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CodeSets_libs_UCS4_UTF16.mak" CFG="CodeSets_Libs_UCS4_UTF16 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CodeSets_Libs_UCS4_UTF16 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CodeSets_Libs_UCS4_UTF16 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CodeSets_Libs_UCS4_UTF16 - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\bin"
# PROP Intermediate_Dir "Release\CodeSets_Libs_UCS4_UTF16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UCS4_UTF16_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\..\..\lib\UCS4_UTF16.bsc"
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAO.lib ACE.lib /nologo /version:1.304 /subsystem:windows /dll /pdb:"..\..\..\..\..\lib\UCS4_UTF16.pdb" /machine:I386 /out:"..\..\..\..\..\bin\UCS4_UTF16.dll" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CodeSets_Libs_UCS4_UTF16 - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\bin"
# PROP Intermediate_Dir "Debug\CodeSets_Libs_UCS4_UTF16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UCS4_UTF16_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\..\..\lib\UCS4_UTF16.bsc"
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAOd.lib ACEd.lib /nologo /version:1.304 /subsystem:windows /dll /incremental:no /pdb:"..\..\..\..\..\lib\UCS4_UTF16d.pdb" /debug /machine:I386 /out:"..\..\..\..\..\bin\UCS4_UTF16d.dll" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CodeSets_Libs_UCS4_UTF16 - Win32 Release"
# Name "CodeSets_Libs_UCS4_UTF16 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\WUCS4_UTF16.cpp"
# End Source File
# Begin Source File

SOURCE=".\WUCS4_UTF16_Factory.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\WUCS4_UTF16.h"
# End Source File
# Begin Source File

SOURCE=".\WUCS4_UTF16_export.h"
# End Source File
# Begin Source File

SOURCE=".\WUCS4_UTF16_Factory.h"
# End Source File
# End Group
# End Target
# End Project
