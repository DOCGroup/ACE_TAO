# Microsoft Developer Studio Project File - Name="RTCosScheduling" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RTCosScheduling - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTCosScheduling_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTCosScheduling_Static.mak" CFG="RTCosScheduling - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTCosScheduling - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "RTCosScheduling - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCosScheduling - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTCosScheduling___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "RTCosScheduling___Win32_Static_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /D /D /D /c
# SUBTRACT BASE CPP /Fr /YX
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /D /D /D /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.." /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\TAO_RTCosSchedulingsd.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\TAO_RTCosSchedulingsd.lib"

!ELSEIF  "$(CFG)" == "RTCosScheduling - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTCosScheduling___Win32_Static_Release"
# PROP BASE Intermediate_Dir "RTCosScheduling___Win32_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /D /D /D /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "$(TAO_ROOT)\orbsvcs" /I "$(TAO_ROOT)" /I "$(TAO_ROOT)\tao" /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /D /D /D /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.." /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(TAO_ROOT)\orbsvcs" /i "$(TAO_ROOT)" /i "$(TAO_ROOT)\tao" /i "..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_RTCosScheduling.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\TAO_RTCosSchedulings.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\TAO_RTCosSchedulings.lib"

!ENDIF 

# Begin Target

# Name "RTCosScheduling - Win32 Static Debug"
# Name "RTCosScheduling - Win32 Static Release"
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

!IF  "$(CFG)" == "RTCosScheduling - Win32 Static Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\RTCosScheduling.idl"
InputName=RTCosScheduling

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=RTCosScheduling\RTCosScheduling_export.h -Gv -I$(TAO_ROOT) -I$(TAO_ROOT)\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

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

!ELSEIF  "$(CFG)" == "RTCosScheduling - Win32 Static Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\RTCosScheduling.idl"
InputName=RTCosScheduling

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\Release\tao_idl -o . -Gd -Wb,export_macro=TAO_RTCosScheduling_Export -Wb,export_include=RTCosScheduling\RTCosScheduling_export.h -Gv -I$(TAO_ROOT) -I$(TAO_ROOT)\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

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
