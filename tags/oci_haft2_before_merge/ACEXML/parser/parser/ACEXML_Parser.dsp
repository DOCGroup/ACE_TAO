# Microsoft Developer Studio Project File - Name="ACEXML_Parser DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ACEXML_Parser DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Parser.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Parser.mak" CFG="ACEXML_Parser DLL - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "ACEXML_Parser DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACEXML_Parser DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACEXML_Parser DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\ACEXML_Parser"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /Zi /GR /I "..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D ACEXML_PARSER_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\ACEXML_Parser.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXML.lib ACE.lib /libpath:"..\..\..\ACEXML\common" /libpath:"..\..\..\ace" /nologo /version:5.3.1 /subsystem:windows /pdb:"..\..\..\bin\ACEXML_Parser.pdb" /dll  /machine:I386 /out:"..\..\..\bin\ACEXML_Parser.dll"

!ELSEIF  "$(CFG)" == "ACEXML_Parser DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\ACEXML_Parser"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D ACEXML_PARSER_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\ACEXML_Parser.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXMLd.lib ACEd.lib /libpath:"..\..\..\ACEXML\common" /libpath:"..\..\..\ace" /nologo /version:5.3.1 /subsystem:windows /pdb:"..\..\..\bin\ACEXML_Parserd.pdb" /dll /debug /machine:I386 /out:"..\..\..\bin\ACEXML_Parserd.dll"

!ENDIF

# Begin Target

# Name "ACEXML_Parser DLL - Win32 Release"
# Name "ACEXML_Parser DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Entity_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserContext.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserInternals.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Entity_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\Parser_export.h
# End Source File
# Begin Source File

SOURCE=.\ParserContext.h
# End Source File
# Begin Source File

SOURCE=.\ParserInternals.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Entity_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Parser.i
# End Source File
# Begin Source File

SOURCE=.\ParserContext.inl
# End Source File
# End Group
# End Target
# End Project
