# Microsoft Developer Studio Project File - Name="gperf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gperf - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gperf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gperf.mak" CFG="gperf - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gperf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gperf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_DEBUG" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf___Win32_Static_Release"
# PROP BASE Intermediate_Dir "gperf___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 gperf.lib ace.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 aces.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "gperf___Win32_Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 gperfd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 acesd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "gperf - Win32 Release"
# Name "gperf - Win32 Debug"
# Name "gperf - Win32 Static Release"
# Name "gperf - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Bool_Array.cpp
# End Source File
# Begin Source File

SOURCE=.\Gen_Perf.cpp
# End Source File
# Begin Source File

SOURCE=.\gperf.cpp
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
