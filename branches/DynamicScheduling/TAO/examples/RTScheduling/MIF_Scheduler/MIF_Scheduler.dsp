# Microsoft Developer Studio Project File - Name="MIF_Scheduler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MIF_Scheduler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MIF_Scheduler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MIF_Scheduler.mak" CFG="MIF_Scheduler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MIF_Scheduler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MIF_Scheduler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MIF_Scheduler - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MIF_Scheduler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "../../../../" /I "../../../" /I "../../../tao/RTScheduling" /I "../../../tao/RTCORBA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "MIF_DT_CREATOR_BUILD_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aced.lib taod.lib TAO_RTCORBAd.lib TAO_PortableServerd.lib TAO_RTSchedulerd.lib TAO_CosNamingd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(ACE_ROOT)\ace" /libpath:"../../../tao" /libpath:"../../../tao/RTCORBA" /libpath:"../../../tao/PortableServer" /libpath:"../../../tao/RTScheduling" /libpath:"../../../orbsvcs/orbsvcs/"

!ENDIF 

# Begin Target

# Name "MIF_Scheduler - Win32 Release"
# Name "MIF_Scheduler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\DT_Creator.cpp
# End Source File
# Begin Source File

SOURCE=..\Job_i.cpp
# End Source File
# Begin Source File

SOURCE=..\JobC.cpp
# End Source File
# Begin Source File

SOURCE=..\JobS.cpp
# End Source File
# Begin Source File

SOURCE=.\MIF_DT_Creator.cpp
# End Source File
# Begin Source File

SOURCE=.\MIF_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=..\MIF_SchedulingC.cpp
# End Source File
# Begin Source File

SOURCE=.\MIF_Task.cpp
# End Source File
# Begin Source File

SOURCE=..\POA_Holder.cpp
# End Source File
# Begin Source File

SOURCE=..\Synch_i.cpp
# End Source File
# Begin Source File

SOURCE=..\SynchC.cpp
# End Source File
# Begin Source File

SOURCE=..\SynchS.cpp
# End Source File
# Begin Source File

SOURCE=..\Task_Stats.cpp
# End Source File
# Begin Source File

SOURCE=.\test.cpp
# End Source File
# Begin Source File

SOURCE=..\Thread_Task.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\DT_Creator.h
# End Source File
# Begin Source File

SOURCE=..\Job_i.h
# End Source File
# Begin Source File

SOURCE=..\JobC.h
# End Source File
# Begin Source File

SOURCE=..\JobS.h
# End Source File
# Begin Source File

SOURCE=.\MIF_DT_Creator.h
# End Source File
# Begin Source File

SOURCE=.\mif_dt_creator_export.h
# End Source File
# Begin Source File

SOURCE=.\MIF_Scheduler.h
# End Source File
# Begin Source File

SOURCE=..\MIF_SchedulingC.h
# End Source File
# Begin Source File

SOURCE=.\MIF_Task.h
# End Source File
# Begin Source File

SOURCE=..\POA_Holder.h
# End Source File
# Begin Source File

SOURCE=..\Synch_i.h
# End Source File
# Begin Source File

SOURCE=..\SynchC.h
# End Source File
# Begin Source File

SOURCE=..\SynchS.h
# End Source File
# Begin Source File

SOURCE=..\Task.h
# End Source File
# Begin Source File

SOURCE=..\Task_Stats.h
# End Source File
# Begin Source File

SOURCE=..\Task_Stats.inl
# End Source File
# Begin Source File

SOURCE=.\test.h
# End Source File
# Begin Source File

SOURCE=..\Thread_Task.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
