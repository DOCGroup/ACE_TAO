# Microsoft Developer Studio Project File - Name="Kokyu_DSRT_Schedulers" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Kokyu_DSRT_Schedulers - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Kokyu_DSRT_Schedulers.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Kokyu_DSRT_Schedulers.mak" CFG="Kokyu_DSRT_Schedulers - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Kokyu_DSRT_Schedulers - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Kokyu_DSRT_Schedulers - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\Kokyu_DSRT_Schedulers"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\.." /I "..\..\.." /I "..\..\tao\RTScheduling" /I "..\..\tao\RTCORBA" /I "..\..\..\Kokyu" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "KOKYU_DSRT_SCHEDULERS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\.." /i "..\..\.." /i "..\..\tao\RTScheduling" /i "..\..\tao\RTCORBA" /i "..\..\..\Kokyu" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_RTScheduler.lib Kokyu.lib TAO_RTCORBA.lib ACE.lib TAO.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386 /libpath:"..\..\examples\Kokyu_dsrt_schedulers" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\RTScheduling" /libpath:"..\..\..\Kokyu"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Kokyu_DSRT_Schedulers"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\.." /I "..\..\.." /I "..\..\tao\RTScheduling" /I "..\..\tao\RTCORBA" /I "..\..\..\Kokyu" /I "..\..\tao" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "KOKYU_DSRT_SCHEDULERS_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\.." /i "..\..\.." /i "..\..\tao\RTScheduling" /i "..\..\tao\RTCORBA" /i "..\..\..\Kokyu" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_RTSchedulerd.lib Kokyud.lib TAO_RTCORBAd.lib ACEd.lib TAOd.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:".\Kokyu_DSRT_Schedulersd.dll" /libpath:"..\..\examples\Kokyu_dsrt_schedulers" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\RTScheduling" /libpath:"..\..\..\Kokyu"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Kokyu_DSRT_Schedulers - Win32 Release"
# Name "Kokyu_DSRT_Schedulers - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\FP_Scheduler.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FP_SchedulingC.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Kokyu_qosC.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MIF_Scheduler.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MIF_SchedulingC.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MUF_Scheduler.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MUF_SchedulingC.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Task_Stats.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\utils.cpp"

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# ADD CPP /I "..\..\tao"

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\FP_Scheduler.h"
# End Source File
# Begin Source File

SOURCE=".\FP_SchedulingC.h"
# End Source File
# Begin Source File

SOURCE=".\Kokyu_qosC.h"
# End Source File
# Begin Source File

SOURCE=".\MIF_Scheduler.h"
# End Source File
# Begin Source File

SOURCE=".\MIF_SchedulingC.h"
# End Source File
# Begin Source File

SOURCE=".\MUF_Scheduler.h"
# End Source File
# Begin Source File

SOURCE=".\MUF_SchedulingC.h"
# End Source File
# Begin Source File

SOURCE=".\Task_Stats.h"
# End Source File
# Begin Source File

SOURCE=".\utils.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\FP_SchedulingC.i"
# End Source File
# Begin Source File

SOURCE=".\Kokyu_qosC.i"
# End Source File
# Begin Source File

SOURCE=".\MIF_SchedulingC.i"
# End Source File
# Begin Source File

SOURCE=".\MUF_SchedulingC.i"
# End Source File
# Begin Source File

SOURCE=".\Task_Stats.inl"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# Begin Group "IDLFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FP_Scheduling.idl

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# PROP Intermediate_Dir "Release\"
# PROP Ignore_Default_Tool 1
USERDEP__FP_SC="..\..\..\bin\Release\tao_idl -Ge 1 $(InputName).idl"	
# Begin Custom Build - Invoking IDL compiler
InputPath=.\FP_Scheduling.idl
InputName=FP_Scheduling

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I..\..\ -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

# PROP Intermediate_Dir "Debug\"
# PROP Ignore_Default_Tool 1
USERDEP__FP_SC="..\..\..\bin\tao_idl -Ge 1 $(InputName).idl"	
# Begin Custom Build - Invoking IDLCompiler
InputPath=.\FP_Scheduling.idl
InputName=FP_Scheduling

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1  -I..\..\  -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Kokyu_qos.idl

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__KOKYU="..\..\..\bin\Release\tao_idl"	
# Begin Custom Build
InputPath=.\Kokyu_qos.idl
InputName=Kokyu_qos

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I..\..\ $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__KOKYU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking IDL compiler
InputPath=.\Kokyu_qos.idl
InputName=Kokyu_qos

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I..\..\ $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MIF_Scheduling.idl

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\MIF_Scheduling.idl
InputName=MIF_Scheduling

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I..\..\ -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

# PROP Intermediate_Dir "Debug\"
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL compiler
InputPath=.\MIF_Scheduling.idl
InputName=MIF_Scheduling

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I..\..\ -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MUF_Scheduling.idl

!IF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Release"

# PROP Intermediate_Dir "Release\"
# PROP Ignore_Default_Tool 1
USERDEP__MUF_S="..\..\..\bin\Release\tao_idl -Ge 1 $(InputName).idl"	
# Begin Custom Build - Invoking IDL compiler
InputPath=.\MUF_Scheduling.idl
InputName=MUF_Scheduling

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I..\..\  -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Kokyu_DSRT_Schedulers - Win32 Debug"

# PROP Intermediate_Dir "Debug\"
# PROP Ignore_Default_Tool 1
USERDEP__MUF_S="..\..\..\bin\tao_idl -Ge 1 $(InputName).idl"	
# Begin Custom Build - Invoking IDLCompiler
InputPath=.\MUF_Scheduling.idl
InputName=MUF_Scheduling

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I..\..\  -Wb,export_macro=Kokyu_DSRT_Schedulers_Export -Wb,export_include=Kokyu_dsrt_schedulers_export.h $(InputName).idl

"(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
