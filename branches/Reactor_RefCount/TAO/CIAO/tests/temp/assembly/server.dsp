# Microsoft Developer Studio Project File - Name="server EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=server EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "server.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "server.mak" CFG="server EXE - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "server EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "server EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "server EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /GR /I "..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\..\CIAO" /I "..\..\..\..\CIAO\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\..\CIAO" /i "..\..\..\..\CIAO\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO CIAO_Server.lib TAO_PortableServer.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib CIAO_Client.lib ACE.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\CIAO\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:console  /machine:I386 /out:"server.exe"

!ELSEIF  "$(CFG)" == "server EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\..\CIAO" /I "..\..\..\..\CIAO\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\..\CIAO" /i "..\..\..\..\CIAO\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO CIAO_Serverd.lib TAO_PortableServerd.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib CIAO_Clientd.lib ACEd.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\CIAO\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:console /debug /machine:I386 /out:"server.exe"

!ENDIF

# Begin Target

# Name "server EXE - Win32 Release"
# Name "server EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\controlC.cpp
# End Source File
# Begin Source File

SOURCE=.\controlS.cpp
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\controlC.h
# End Source File
# Begin Source File

SOURCE=.\controlS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\controlC.i
# End Source File
# Begin Source File

SOURCE=.\controlS.i
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

SOURCE=.\control.idl

!IF  "$(CFG)" == "server EXE - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\control.idl
InputName=control
InputDir=.
OutDir=.

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -o $(OutDir) -I..\..\..\.. -I..\..\..\ciao -I..\..\..\.. -Ge 1 -Sc $(InputPath)

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

!ELSEIF  "$(CFG)" == "server EXE - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\control.idl
InputName=control
InputDir=.
OutDir=.

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -o $(OutDir) -I..\..\..\.. -I..\..\..\ciao -I..\..\..\.. -Ge 1 -Sc $(InputPath)

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
