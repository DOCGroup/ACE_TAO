# Microsoft Developer Studio Project File - Name="Metrics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Metrics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Metrics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Metrics.mak" CFG="Metrics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Metrics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Metrics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Metrics - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Metrics - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Metrics___Win32_Debug"
# PROP BASE Intermediate_Dir "Metrics___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Metrics___Win32_Debug"
# PROP Intermediate_Dir "Metrics___Win32_Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD   /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_METRICS_BUILD_DLL" /YX /FD   /c
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

# Name "Metrics - Win32 Release"
# Name "Metrics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Metrics_FrameManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_FrameManager_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_Utils.cpp
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

SOURCE=.\metrics_export.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_FrameManager.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_FrameManager_T.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache_T.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_Logger.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor_T.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_Utils.h
# End Source File
# Begin Source File

SOURCE=..\MetricsC.h
# End Source File
# Begin Source File

SOURCE=..\MetricsS.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=..\Metrics.idl
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Metrics_FrameManager.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_FrameManager_T.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_LocalCache_T.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_UpcallMonitor_T.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_Utils.i
# End Source File
# End Group
# End Target
# End Project
