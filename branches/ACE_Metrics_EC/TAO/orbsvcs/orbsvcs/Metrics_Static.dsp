# Microsoft Developer Studio Project File - Name="Metrics_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Metrics_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Metrics_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Metrics_Static.mak" CFG="Metrics_Static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Metrics_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Metrics_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Metrics_Static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../" /I "../../" /I "../../../" /I "../../../../" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "TAO_METRICS_BUILD_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Metrics_Static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD   /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I "../" /I "../../" /I "../../../" /I "../../../../" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D TAO_METRICS_HAS_DLL=0 /YX /FD   /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Metrics_Static - Win32 Release"
# Name "Metrics_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache.cpp
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_Logger.cpp
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor.cpp
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_Utils.cpp
# End Source File
# Begin Source File

SOURCE=..\MetricsC.cpp
# End Source File
# Begin Source File

SOURCE=..\MetricsS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Metrics\metrics_export.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager_T.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache_T.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_Logger.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor_T.h
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_Utils.h
# End Source File
# Begin Source File

SOURCE=..\MetricsC.h
# End Source File
# Begin Source File

SOURCE=..\MetricsS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_FrameManager_T.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_LocalCache_T.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_UpcallMonitor_T.i
# End Source File
# Begin Source File

SOURCE=..\Metrics\Metrics_Utils.i
# End Source File
# End Group
# Begin Group "IDL files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Metrics.idl
# End Source File
# End Group
# End Target
# End Project
