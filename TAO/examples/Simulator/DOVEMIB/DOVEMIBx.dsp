# Microsoft Developer Studio Project File - Name="DOVEMIBx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DOVEMIBx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DOVEMIBx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DOVEMIBx.mak" CFG="DOVEMIBx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DOVEMIBx - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "DOVEMIBx - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DOVEMIBx - Win32 Release"

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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "DOVEMIBx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DOVEMIB0"
# PROP BASE Intermediate_Dir "DOVEMIB0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DOVEMIB0"
# PROP Intermediate_Dir "DOVEMIB0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\..\\" /I "..\..\..\..\\" /I "..\..\..\..\..\..\\" /I "..\..\..\\" /I "..\..\..\tao" /I "..\..\..\orbsvcs" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "TAO_ORBSVCS_HAS_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib orbsvcsd.lib /nologo /subsystem:console /pdb:"DOVEMIBx.pdb" /debug /machine:I386 /out:"DOVEMIBx.exe" /pdbtype:sept /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\orbsvcs"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DOVEMIBx - Win32 Release"
# Name "DOVEMIBx - Win32 Debug"
# Begin Source File

SOURCE=.\any_test.idl

!IF  "$(CFG)" == "DOVEMIBx - Win32 Release"

!ELSEIF  "$(CFG)" == "DOVEMIBx - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\any_test.idl
InputName=any_test

BuildCmds= \
	..\..\..\..\..\bin\tao_idl $(InputName).idl

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\any_test_i.cpp
# End Source File
# Begin Source File

SOURCE=.\any_testC.cpp
# End Source File
# Begin Source File

SOURCE=.\any_testS.cpp
# End Source File
# Begin Source File

SOURCE=.\AnyAnalyser.cpp
# End Source File
# Begin Source File

SOURCE=.\NavWeap.idl

!IF  "$(CFG)" == "DOVEMIBx - Win32 Release"

!ELSEIF  "$(CFG)" == "DOVEMIBx - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\NavWeap.idl
InputName=NavWeap

BuildCmds= \
	xcopy ..\$(InputName).idl . \
	..\..\..\..\..\bin\tao_idl $(InputName).idl \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NavWeapC.cpp
# End Source File
# Begin Source File

SOURCE=.\Node.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\svr.cpp
# End Source File
# End Target
# End Project
