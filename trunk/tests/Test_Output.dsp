# Microsoft Developer Studio Project File - Name="Test_Output DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Test_Output DLL - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Test_Output.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Test_Output.mak" CFG="Test_Output DLL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test_Output DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Test_Output DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Test_Output DLL - Win32 Static Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Test_Output DLL - Win32 Static Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Test_Output DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DLL_Test DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "FRAMEWORK_COMPONENT_DLL_BUILD_DLL" /D "NDEBUG" /D "DLL_Test DLL_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TEST_OUTPUT_BUILD_DLL" /FD /c
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
# ADD LINK32 ace.lib /nologo /dll /machine:I386 /libpath:"..\ace"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=delete old dlls from tests dir
PostBuild_Cmds=del Test_Output*.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Test_Output DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DLL_Test DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "FRAMEWORK_COMPONENT_DLL_BUILD_DLL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TEST_OUTPUT_BUILD_DLL" /FR /FD /c
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
# ADD LINK32 aced.lib /nologo /dll /debug /machine:I386 /out:"..\bin\Test_Outputd.dll" /libpath:"..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Test_Output DLL - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Static_Debug"
# PROP BASE Intermediate_Dir "Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DLL_Test DLL_EXPORTS" /D "ACE_BUILD_SVC_DLL" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "FRAMEWORK_COMPONENT_DLL_BUILD_DLL" /D "_DEBUG" /D "DLL_Test DLL_EXPORTS" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TEST_OUTPUT_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib /nologo /dll /debug /machine:I386 /out:"DLL_Testd.dll" /pdbtype:sept /libpath:"../ace"
# ADD LINK32 acesd.lib advapi32.lib /nologo /dll /debug /machine:I386 /out:"..\bin\Test_Outputsd.dll" /pdbtype:sept /libpath:"..\ace"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=delete old dlls from tests dir
PostBuild_Cmds=del Test_Output*.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Test_Output DLL - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Static_Release"
# PROP BASE Intermediate_Dir "Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DLL_Test DLL_EXPORTS" /D "ACE_BUILD_SVC_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "TEST_OUTPUT_BUILD_DLL" /D "NDEBUG" /D "DLL_Test DLL_EXPORTS" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib /nologo /dll /machine:I386 /out:"DLL_Test.dll" /libpath:"../ace"
# ADD LINK32 aces.lib advapi32.lib /nologo /dll /machine:I386 /out:"..\bin\Test_Outputs.dll" /libpath:"..\ace"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=delete old dlls from tests dir
PostBuild_Cmds=del Test_Output*.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Test_Output DLL - Win32 Release"
# Name "Test_Output DLL - Win32 Debug"
# Name "Test_Output DLL - Win32 Static Debug"
# Name "Test_Output DLL - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Test_Output.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\test_config.h
# End Source File
# Begin Source File

SOURCE=.\Test_Output_Export.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
