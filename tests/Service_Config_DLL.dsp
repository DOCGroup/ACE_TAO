# Microsoft Developer Studio Project File - Name="Service_Config_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Service_Config_DLL - Win32 Static Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Service_Config_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Service_Config_DLL.mak" CFG="Service_Config_DLL - Win32 Static Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Service_Config_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Service_Config_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Service_Config_DLL - Win32 Static Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Service_Config_DLL - Win32 Static Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Service_Config_DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Service_Config_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "SERVICE_CONFIG_DLL_BUILD_DLL" /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ACE.lib /nologo /dll /pdb:none /machine:I386 /out:"..\bin\Service_Config_DLL.dll" /libpath:"../ace"

!ELSEIF  "$(CFG)" == "Service_Config_DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Service_Config_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SERVICE_CONFIG_DLL_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ACEd.lib /nologo /dll /debug /machine:I386 /out:"..\bin\Service_Config_DLLd.dll" /libpath:"../ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Service_Config_DLL - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Service_Config_DLL___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "Service_Config_DLL___Win32_Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TEST_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /Fr /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ACEd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\Service_Config_DLLd.dll" /libpath:"../../../ace" /libpath:"../../tao"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ACEsd.lib /nologo /dll /debug /machine:I386 /out:"Static_Debug\Service_Config_DLLsd.lib" /libpath:"../ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Service_Config_DLL - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Service_Config_DLL___Win32_Static_Release"
# PROP BASE Intermediate_Dir "Service_Config_DLL___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TEST_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /Fr /YX
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ACEd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\Service_Config_DLLd.dll" /libpath:"../../../ace" /libpath:"../../tao"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ACEs.lib /nologo /dll /debug /machine:I386 /out:"Service_Config_DLLs.lib" /libpath:"../ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Service_Config_DLL - Win32 Release"
# Name "Service_Config_DLL - Win32 Debug"
# Name "Service_Config_DLL - Win32 Static Debug"
# Name "Service_Config_DLL - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx"
# Begin Source File

SOURCE=.\Service_Config_DLL.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\Service_Config_DLL.h
# End Source File
# Begin Source File

SOURCE=.\Service_Config_DLL_Export.h
# End Source File
# Begin Source File

SOURCE=.\test_config.h
# End Source File
# End Group
# End Target
# End Project
