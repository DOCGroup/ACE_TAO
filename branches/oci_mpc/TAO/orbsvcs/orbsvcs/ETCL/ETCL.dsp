# Microsoft Developer Studio Project File - Name="ETCL DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ETCL DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ETCL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ETCL.mak" CFG="ETCL DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ETCL DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ETCL DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ETCL DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL/ETCL/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCLDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\\" /I "..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D TAO_ETCL_HAS_DLL=1 /D "TAO_ETCL_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\..\bin\TAO_ETCL.dll" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"

!ELSEIF  "$(CFG)" == "ETCL DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL/ETCL/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCLDLL_EXPORTS" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /I "..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D TAO_ETCL_HAS_DLL=1 /D "TAO_ETCL_BUILD_DLL" /YX /FD  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\bin\TAO_ETCLd.dll" /pdbtype:sept /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"

!ENDIF 

# Begin Target

# Name "ETCL DLL - Win32 Release"
# Name "ETCL DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ETCL_Constraint.cpp
# End Source File
# Begin Source File

SOURCE=.\ETCL_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\ETCL_l.cpp
# End Source File
# Begin Source File

SOURCE=.\ETCL_y.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ETCL_Constraint.h
# End Source File
# Begin Source File

SOURCE=.\ETCL_Constraint_Visitor.h
# End Source File
# Begin Source File

SOURCE=.\etcl_export.h
# End Source File
# Begin Source File

SOURCE=.\ETCL_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\ETCL_y.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ETCL_Constraint.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\ETCL.rc
# End Source File
# End Group
# End Target
# End Project
