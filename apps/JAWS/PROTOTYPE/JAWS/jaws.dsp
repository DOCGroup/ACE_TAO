# Microsoft Developer Studio Project File - Name="jaws" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jaws - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jaws.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jaws.mak" CFG="jaws - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jaws - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jaws - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "jaws - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".." /I "..\..\..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".." /I "..\..\..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"jawsd.lib"

!ENDIF 

# Begin Target

# Name "jaws - Win32 Release"
# Name "jaws - Win32 Debug"
# Begin Group "Source files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Concurrency.cpp
# End Source File
# Begin Source File

SOURCE=.\Data_Block.cpp
# End Source File
# Begin Source File

SOURCE=.\IO.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Waiter.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Assoc_Array.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency.h
# End Source File
# Begin Source File

SOURCE=.\Data_Block.h
# End Source File
# Begin Source File

SOURCE=.\Export.h
# End Source File
# Begin Source File

SOURCE=.\IO.h
# End Source File
# Begin Source File

SOURCE=.\IO_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IO_Handler.h
# End Source File
# Begin Source File

SOURCE=.\IO_State.h
# End Source File
# Begin Source File

SOURCE=.\JAWS.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\Policy.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\Waiter.h
# End Source File
# End Group
# Begin Group "Template files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Assoc_Array.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipeline_Handler.cpp
# End Source File
# End Group
# End Target
# End Project
