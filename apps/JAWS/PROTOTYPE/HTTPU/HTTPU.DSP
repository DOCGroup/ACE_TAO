# Microsoft Developer Studio Project File - Name="HTTPU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HTTPU - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HTTPU.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HTTPU.MAK" CFG="HTTPU - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HTTPU - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HTTPU - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HTTPU - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\..\..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D HTTPU_HAS_DLL=1 /D "HTTPU_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib jaws.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\bin\HTTPU.dll" /libpath:"..\..\..\..\ace" /libpath:"..\JAWS"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "HTTPU - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "..\..\..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D HTTPU_HAS_DLL=1 /D "HTTPU_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib jawsd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\bin\HTTPUd.dll" /pdbtype:sept /libpath:"..\..\..\..\ace" /libpath:"..\JAWS"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "HTTPU - Win32 Release"
# Name "HTTPU - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\http_base.cpp
# End Source File
# Begin Source File

SOURCE=.\http_headers.cpp
# End Source File
# Begin Source File

SOURCE=.\http_request.cpp
# End Source File
# Begin Source File

SOURCE=.\http_response.cpp
# End Source File
# Begin Source File

SOURCE=.\http_status.cpp
# End Source File
# Begin Source File

SOURCE=.\parse_http_request.cpp
# End Source File
# Begin Source File

SOURCE=.\parse_http_response.cpp
# End Source File
# Begin Source File

SOURCE=.\parse_url.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\http_base.h
# End Source File
# Begin Source File

SOURCE=.\http_export.h
# End Source File
# Begin Source File

SOURCE=.\http_headers.h
# End Source File
# Begin Source File

SOURCE=.\http_request.h
# End Source File
# Begin Source File

SOURCE=.\http_response.h
# End Source File
# Begin Source File

SOURCE=.\http_status.h
# End Source File
# Begin Source File

SOURCE=.\parse_http_request.h
# End Source File
# Begin Source File

SOURCE=.\parse_http_response.h
# End Source File
# Begin Source File

SOURCE=.\parse_url.h
# End Source File
# End Group
# End Target
# End Project
