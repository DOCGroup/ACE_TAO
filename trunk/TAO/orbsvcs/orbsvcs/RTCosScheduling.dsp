# Microsoft Developer Studio Project File - Name="RTCosScheduling" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTCosScheduling - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "RTCosScheduling.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "RTCosScheduling.mak" CFG="RTCosScheduling - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "RTCosScheduling - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTCosScheduling - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCosScheduling - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\RTCosScheduling"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D TAO_RTCOSSCHEDULING_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTPortableServer.lib TAO_RTCORBA.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.4 /subsystem:windows /pdb:"..\..\..\lib\TAO_RTCosScheduling.pdb" /dll  /machine:I386 /out:"..\..\..\lib\TAO_RTCosScheduling.dll"

!ELSEIF  "$(CFG)" == "RTCosScheduling - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\RTCosScheduling"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D TAO_RTCOSSCHEDULING_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTPortableServerd.lib TAO_RTCORBAd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.4 /subsystem:windows /pdb:"..\..\..\lib\TAO_RTCosSchedulingd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\TAO_RTCosSchedulingd.dll"

!ENDIF

# Begin Target

# Name "RTCosScheduling - Win32 Release"
# Name "RTCosScheduling - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ClientScheduler_i.cpp"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_PCP_Manager.cpp"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ServerScheduler_i.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ClientScheduler_i.h"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_PCP_Manager.h"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ServerScheduler_i.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ClientScheduler_i.i"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_PCP_Manager.i"
# End Source File
# Begin Source File

SOURCE=".\RTCosScheduling\RTCosScheduling_ServerScheduler_i.i"
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=".\RTCosScheduling.idl"

!IF  "$(CFG)" == "RTCosScheduling - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\RTCosScheduling.idl
InputName=RTCosScheduling
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\Release\tao_idl -o . -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=RTCosScheduling\RTCosScheduling_export.h -Gv -I$(TAO_ROOT) -I$(TAO_ROOT)\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

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

!ELSEIF  "$(CFG)" == "RTCosScheduling - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\RTCosScheduling.idl
InputName=RTCosScheduling
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=RTCosScheduling\RTCosScheduling_export.h -Gv -I$(TAO_ROOT) -I$(TAO_ROOT)\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

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
