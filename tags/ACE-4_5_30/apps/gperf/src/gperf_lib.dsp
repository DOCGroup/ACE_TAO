# Microsoft Developer Studio Project File - Name="gperf_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gperf_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gperf_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gperf_lib.mak" CFG="gperf_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gperf_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gperf_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "gperf_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf_li"
# PROP BASE Intermediate_Dir "gperf_li"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\lib"
# PROP Intermediate_Dir ".\lib\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX- /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\gperf.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf_l0"
# PROP BASE Intermediate_Dir "gperf_l0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\lib"
# PROP Intermediate_Dir ".\lib\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX- /Z7 /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\gperfd.lib"

!ENDIF 

# Begin Target

# Name "gperf_lib - Win32 Release"
# Name "gperf_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Bool_Array.cpp
# End Source File
# Begin Source File

SOURCE=.\Gen_Perf.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\Iterator.cpp
# End Source File
# Begin Source File

SOURCE=.\Key_List.cpp
# End Source File
# Begin Source File

SOURCE=.\List_Node.cpp
# End Source File
# Begin Source File

SOURCE=.\new.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Vectors.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Bool_Array.h
# End Source File
# Begin Source File

SOURCE=.\Gen_Perf.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Table.h
# End Source File
# Begin Source File

SOURCE=.\Iterator.h
# End Source File
# Begin Source File

SOURCE=.\Key_List.h
# End Source File
# Begin Source File

SOURCE=.\List_Node.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\Vectors.h
# End Source File
# End Group
# End Target
# End Project
