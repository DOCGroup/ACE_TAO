# Microsoft Developer Studio Project File - Name="NamespaceSupport_Test EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=NamespaceSupport_Test EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "NamespaceSupport_Test.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "NamespaceSupport_Test.mak" CFG="NamespaceSupport_Test EXE - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "NamespaceSupport_Test EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "NamespaceSupport_Test EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NamespaceSupport_Test EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\NamespaceSupport_Test"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /GR /I "..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXML_Parser.lib ACEXML.lib ACE.lib /libpath:"..\..\ACEXML\common" /libpath:"..\..\ACEXML\parser\parser" /libpath:"..\..\ace" /nologo /version:5.3.1 /subsystem:console /pdb:"Release\NamespaceSupport_Test.pdb"  /machine:I386 /out:"Release\NamespaceSupport_Test.exe"

!ELSEIF  "$(CFG)" == "NamespaceSupport_Test EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\NamespaceSupport_Test"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXML_Parserd.lib ACEXMLd.lib ACEd.lib /libpath:"..\..\ACEXML\common" /libpath:"..\..\ACEXML\parser\parser" /libpath:"..\..\ace" /nologo /version:5.3.1 /subsystem:console /pdb:".\NamespaceSupport_Test.pdb" /debug /machine:I386 /out:".\NamespaceSupport_Test.exe"

!ENDIF

# Begin Target

# Name "NamespaceSupport_Test EXE - Win32 Release"
# Name "NamespaceSupport_Test EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\NamespaceSupport_Test.cpp
# End Source File
# End Group
# End Target
# End Project
