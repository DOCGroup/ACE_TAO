# Microsoft Developer Studio Project File - Name="RTSched" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTSched - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTSched.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTSched.mak" CFG="RTSched - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTSched - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTSched - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTSched - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTSched - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTSched - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTSched"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTSched_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_CosNaming.lib TAO_Svc_Utils.lib ace.lib TAO.lib TAO_PortableServer.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTSched.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTSched - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTSched"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTSched_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "TAO_ORBSVCS_HAS_NAMING" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAOd.lib aced.lib TAO_PortableServerd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTSchedd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTSched___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "RTSched___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTSchedMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "TAO_ORBSVCS_HAS_NAMING" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "TAO_ORBSVCS_HAS_NAMING" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTSchedd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 TAOmfcd.lib acemfcd.lib TAO_CosNamingmfcd.lib TAO_Svc_Utilsmfcd.lib TAO_PortableServermfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTSchedmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTSched___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "RTSched___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTSchedMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTSCHED_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_CosNaming.lib TAO_Svc_Utils.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTSched.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_PortableServermfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTSchedmfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "RTSched - Win32 Release"
# Name "RTSched - Win32 Debug"
# Name "RTSched - Win32 MFC Debug"
# Name "RTSched - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils_T.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler_T.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS_T.h
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Sched\sched_export.h
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\RtecScheduler.idl

!IF  "$(CFG)" == "RTSched - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=Sched\sched_export.h $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "RTSched - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=Sched\sched_export.h $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=Sched\sched_export.h $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTSched_Export -Wb,export_include=Sched\sched_export.h $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils_T.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler_T.i
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.i
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.i
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS_T.i
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.i
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils_T.cpp

!IF  "$(CFG)" == "RTSched - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler_T.cpp

!IF  "$(CFG)" == "RTSched - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS_T.cpp

!IF  "$(CFG)" == "RTSched - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTSched - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\RTSched.rc
# End Source File
# End Group
# End Target
# End Project
