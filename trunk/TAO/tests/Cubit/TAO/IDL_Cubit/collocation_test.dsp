# Microsoft Developer Studio Project File - Name="collocation_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=collocation_test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "collocation_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "collocation_test.mak" CFG="collocation_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "collocation_test - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "collocation_test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP WCE_Configuration "H/PC Ver. 2.00"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "collocation_test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "collocat"
# PROP BASE Intermediate_Dir "collocat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\..\..\..\orbsvcs" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib orbsvcs.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\tao\\" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "collocation_test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "colloca0"
# PROP BASE Intermediate_Dir "colloca0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\..\..\..\orbsvcs" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAO.lib orbsvcs.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\tao\\" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\orbsvcs\orbsvcs"

!ENDIF 

# Begin Target

# Name "collocation_test - Win32 Release"
# Name "collocation_test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;idl"
# Begin Source File

SOURCE=.\client_i.cpp
# End Source File
# Begin Source File

SOURCE=.\collocation_test.cpp
# End Source File
# Begin Source File

SOURCE=.\cubit.idl

!IF  "$(CFG)" == "collocation_test - Win32 Release"

# Begin Custom Build
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl $(InputName).idl

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "collocation_test - Win32 Debug"

# Begin Custom Build
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl $(InputName).idl

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cubit_i.cpp
# End Source File
# Begin Source File

SOURCE=.\cubitC.cpp
# End Source File
# Begin Source File

SOURCE=.\cubitS.cpp
# End Source File
# Begin Source File

SOURCE=.\server_i.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# End Group
# End Target
# End Project
