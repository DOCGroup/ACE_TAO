# Microsoft Developer Studio Project File - Name="client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO.lib ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "client - Win32 Release"
# Name "client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\any.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_long_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_short_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_str_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_struct_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\driver.cpp
# End Source File
# Begin Source File

SOURCE=.\fixed_array.cpp
# End Source File
# Begin Source File

SOURCE=.\fixed_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\helper.cpp
# End Source File
# Begin Source File

SOURCE=.\nested_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\objref.cpp
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\Param_TestC.cpp
# End Source File
# Begin Source File

SOURCE=.\Param_TestS.cpp
# End Source File
# Begin Source File

SOURCE=.\results.cpp
# End Source File
# Begin Source File

SOURCE=.\short.cpp
# End Source File
# Begin Source File

SOURCE=.\tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=.\typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_any_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_long_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_objref_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_short_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_str_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_string.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_struct_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\var_array.cpp
# End Source File
# Begin Source File

SOURCE=.\var_struct.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\bd_long_seq.h
# End Source File
# Begin Source File

SOURCE=.\bd_short_seq.h
# End Source File
# Begin Source File

SOURCE=.\bd_str_seq.h
# End Source File
# Begin Source File

SOURCE=.\bd_struct_seq.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\driver.h
# End Source File
# Begin Source File

SOURCE=.\fixed_array.h
# End Source File
# Begin Source File

SOURCE=.\fixed_struct.h
# End Source File
# Begin Source File

SOURCE=.\helper.h
# End Source File
# Begin Source File

SOURCE=.\nested_struct.h
# End Source File
# Begin Source File

SOURCE=.\objref.h
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\Param_TestC.h
# End Source File
# Begin Source File

SOURCE=.\Param_TestS.h
# End Source File
# Begin Source File

SOURCE=.\results.h
# End Source File
# Begin Source File

SOURCE=.\short.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\ub_any_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_long_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_objref_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_short_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_str_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_string.h
# End Source File
# Begin Source File

SOURCE=.\ub_struct_seq.h
# End Source File
# Begin Source File

SOURCE=.\var_array.h
# End Source File
# Begin Source File

SOURCE=.\var_struct.h
# End Source File
# End Group
# Begin Group "Include Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Param_TestC.i
# End Source File
# Begin Source File

SOURCE=.\Param_TestS.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\param_test.idl

!IF  "$(CFG)" == "client - Win32 Release"

# Begin Custom Build
InputPath=.\param_test.idl

BuildCmds= \
	..\..\tao_idl\tao_idl Param_Test.idl

"param_testS.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testS.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testS.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

# Begin Custom Build
InputPath=.\param_test.idl

BuildCmds= \
	..\..\tao_idl\tao_idl Param_Test.idl

"param_testS.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testS.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testS.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"param_testC.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
