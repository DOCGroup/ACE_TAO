# Microsoft Developer Studio Project File - Name="HUDisplay_svnt DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HUDisplay_svnt DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "HUDisplay_svnt.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "HUDisplay_svnt.mak" CFG="HUDisplay_svnt DLL - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "HUDisplay_svnt DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HUDisplay_svnt DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HUDisplay_svnt DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\HUDisplay_svnt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /Zi /GR /I "$(TAO_ROOT)\orbsvcs\orbsvcs" /I "$(CIAO_ROOT)" /I "$(CIAO_ROOT)\ciao" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D HUDISPLAY_SVNT_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "$(TAO_ROOT)\orbsvcs\orbsvcs" /i "$(CIAO_ROOT)" /i "$(CIAO_ROOT)\ciao" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\HUDisplay_svnt.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO HUDisplay_stub.lib CIAO_Server.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib TAO_Valuetype.lib CIAO_Client.lib ACE.lib /libpath:"$(TAO_ROOT)\tao\PortableServer" /libpath:"$(TAO_ROOT)\tao\IORInterceptor" /libpath:"$(TAO_ROOT)\tao\ObjRefTemplate" /libpath:"$(TAO_ROOT)\orbsvcs\orbsvcs" /libpath:"$(TAO_ROOT)\tao\IFR_Client" /libpath:"$(TAO_ROOT)\tao\Valuetype" /libpath:"$(CIAO_ROOT)\ciao" /libpath:"$(TAO_ROOT)\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\..\..\bin\HUDisplay_svnt.pdb" /dll  /machine:I386 /out:"..\..\..\..\..\bin\HUDisplay_svnt.dll"

!ELSEIF  "$(CFG)" == "HUDisplay_svnt DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\HUDisplay_svnt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "$(TAO_ROOT)\orbsvcs\orbsvcs" /I "$(CIAO_ROOT)" /I "$(CIAO_ROOT)\ciao" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D HUDISPLAY_SVNT_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "$(TAO_ROOT)\orbsvcs\orbsvcs" /i "$(CIAO_ROOT)" /i "$(CIAO_ROOT)\ciao" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\HUDisplay_svnt.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO HUDisplay_stubd.lib CIAO_Serverd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib TAO_Valuetyped.lib CIAO_Clientd.lib ACEd.lib /libpath:"$(TAO_ROOT)\tao\PortableServer" /libpath:"$(TAO_ROOT)\tao\IORInterceptor" /libpath:"$(TAO_ROOT)\tao\ObjRefTemplate" /libpath:"$(TAO_ROOT)\orbsvcs\orbsvcs" /libpath:"$(TAO_ROOT)\tao\IFR_Client" /libpath:"$(TAO_ROOT)\tao\Valuetype" /libpath:"$(CIAO_ROOT)\ciao" /libpath:"$(TAO_ROOT)\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\..\..\bin\HUDisplay_svntd.pdb" /dll /debug /machine:I386 /out:"..\..\..\..\..\bin\HUDisplay_svntd.dll"

!ENDIF

# Begin Target

# Name "HUDisplay_svnt DLL - Win32 Release"
# Name "HUDisplay_svnt DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\HUDisplayS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\HUDisplayS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\HUDisplayS.i
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

SOURCE=.\HUDisplay.idl

!IF  "$(CFG)" == "HUDisplay_svnt DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\HUDisplay.idl
InputName=HUDisplay
InputDir=.

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -o . -I$(TAO_ROOT) -I$(TAO_ROOT)\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I $(CIAO_ROOT) -I $(TAO_ROOT) -I $(TAO_ROOT)\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

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

!ELSEIF  "$(CFG)" == "HUDisplay_svnt DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\HUDisplay.idl
InputName=HUDisplay
InputDir=.

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -o . -I$(TAO_ROOT) -I$(TAO_ROOT)\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I $(CIAO_ROOT) -I $(TAO_ROOT) -I $(TAO_ROOT)\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

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
