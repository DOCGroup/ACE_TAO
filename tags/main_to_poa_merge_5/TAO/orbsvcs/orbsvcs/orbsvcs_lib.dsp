# Microsoft Developer Studio Project File - Name="orbsvcs_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=orbsvcs_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs_lib.mak" CFG="orbsvcs_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "orbsvcs_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "orbsvcs_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "orbsvcs_lib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "orbsvcs_lib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I ".." /I "../.." /I "../../.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "orbsvcs_lib - Win32 Release"
# Name "orbsvcs_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "*.idl,*.cpp"
# Begin Source File

SOURCE=.\Channel_Clients.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNaming.idl

!IF  "$(CFG)" == "orbsvcs_lib - Win32 Release"

# Begin Custom Build - Invoking TAO IDL compiler
InputPath=.\CosNaming.idl

BuildCmds= \
	..\..\TAO_IDL\tao_idl ${InputName}.idl

"${InputName}C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs_lib - Win32 Debug"

# Begin Custom Build - Invoking TAO IDL compiler on CosNaming
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\TAO_IDL\tao_idl $(InputName).idl

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

SOURCE=.\CosNamingC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "orbsvcs_lib - Win32 Release"

# Begin Custom Build - Invoking TAO IDL compiler
InputPath=.\RtecEventChannelAdmin.idl

BuildCmds= \
	..\..\TAO_IDL\tao_idl ${InputName}.idl

"${InputName}C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs_lib - Win32 Debug"

# Begin Custom Build - Invoking TAO IDL compiler RtecEventChannelAdmin
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\TAO_IDL\tao_idl $(InputName).idl

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

SOURCE=.\RtecEventChannelAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventComm.idl

!IF  "$(CFG)" == "orbsvcs_lib - Win32 Release"

# Begin Custom Build - Invoking TAO IDL compiler
InputPath=.\RtecEventComm.idl

BuildCmds= \
	,,\,,\TAO_IDL\tao_idl ${InputName}.idl

"${InputName}C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs_lib - Win32 Debug"

# Begin Custom Build - Invoking TAO IDL compiler on RtecEventComm
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\TAO_IDL\tao_idl $(InputName).idl

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

SOURCE=.\RtecEventCommC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecScheduler.idl

!IF  "$(CFG)" == "orbsvcs_lib - Win32 Release"

# Begin Custom Build - Invoking TAO IDL compiler
InputPath=.\RtecScheduler.idl

BuildCmds= \
	..\..\TAO_IDL\tao_idl ${InputName}.idl

"${InputName}C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"${InputName}S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs_lib - Win32 Debug"

# Begin Custom Build - Invoking TAO IDL compiler on RtecScheduler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\TAO_IDL\tao_idl $(InputName).idl

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

SOURCE=.\RtecSchedulerC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Channel_Clients.h
# End Source File
# Begin Source File

SOURCE=.\Channel_Clients_T.h
# End Source File
# Begin Source File

SOURCE=.\Event_Service_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "*.i"
# Begin Source File

SOURCE=.\Event_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.i
# End Source File
# End Group
# End Target
# End Project
