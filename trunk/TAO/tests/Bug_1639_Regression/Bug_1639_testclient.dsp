# Microsoft Developer Studio Project File - Name="Bug_1639_testclient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Bug_1639_testclient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1639_testclient.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1639_testclient.mak" CFG="Bug_1639_testclient - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Bug_1639_testclient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Bug_1639_testclient - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bug_1639_testclient - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Bug_1639_testclient"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\include" /I "." /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\include" /i "." /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_PortableServer.lib TAO_DynamicAny.lib TAO.lib ACE.lib /libpath:"..\..\..\ace" /libpath:"." /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\DynamicAny" /libpath:"..\..\tao" /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:console /pdb:"Release\client.pdb"  /machine:I386 /out:"Release\client.exe"

!ELSEIF  "$(CFG)" == "Bug_1639_testclient - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Bug_1639_testclient"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\include" /I "." /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\include" /i "." /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_PortableServerd.lib TAO_DynamicAnyd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\ace" /libpath:"." /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\DynamicAny" /libpath:"..\..\tao" /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:console /pdb:".\client.pdb" /debug /machine:I386 /out:".\client.exe"

!ENDIF

# Begin Target

# Name "Bug_1639_testclient - Win32 Release"
# Name "Bug_1639_testclient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\struct_client.cpp"
# End Source File
# Begin Source File

SOURCE=".\structC.cpp"
# End Source File
# Begin Source File

SOURCE=".\structS.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\structC.h"
# End Source File
# Begin Source File

SOURCE=".\structS.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\structC.i"
# End Source File
# Begin Source File

SOURCE=".\structS.i"
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=".\struct.idl"

!IF  "$(CFG)" == "Bug_1639_testclient - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\struct.idl
InputName=struct
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == "Bug_1639_testclient - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\struct.idl
InputName=struct
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF

# End Source File
# End Group
# End Target
# End Project
