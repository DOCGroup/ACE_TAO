# Microsoft Developer Studio Project File - Name="Bug 1395 Server EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Bug 1395 Server EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Bug_1395_Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bug_1395_Server.mak" CFG="Bug 1395 Server EXE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bug 1395 Server EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Bug 1395 Server EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bug 1395 Server EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\\" /I "..\..\..\..\\" /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "$(ACE_ROOT)" /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_PortableServer.lib TAO.lib ACE.lib /nologo /subsystem:console /machine:I386 /out:"server.exe" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Bug 1395 Server EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\\" /I "..\..\..\..\\" /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "$(ACE_ROOT)" /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_PortableServerd.lib TAOd.lib ACEd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"server.exe" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Bug 1395 Server EXE - Win32 Release"
# Name "Bug 1395 Server EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_i.cpp
# End Source File
# Begin Source File

SOURCE=.\TestC.cpp
# End Source File
# Begin Source File

SOURCE=.\TestS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Test_i.h
# End Source File
# Begin Source File

SOURCE=.\TestC.h
# End Source File
# Begin Source File

SOURCE=.\TestS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\TestC.i
# End Source File
# Begin Source File

SOURCE=.\TestS.i
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\Test.idl

!IF  "$(CFG)" == "Bug 1395 Server EXE - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
OutDir=.
InputPath=.\Test.idl
InputName=Test

BuildCmds= \
	$(ACE_ROOT)\bin\Release\tao_idl -o $(OutDir) -Ge 1 -Sc $(InputPath)

"$(OutDir)\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Bug 1395 Server EXE - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
OutDir=.
InputPath=.\Test.idl
InputName=Test

BuildCmds= \
	$(ACE_ROOT)\bin\tao_idl -o $(OutDir) -Ge 1 -Sc $(InputPath)

"$(OutDir)\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
