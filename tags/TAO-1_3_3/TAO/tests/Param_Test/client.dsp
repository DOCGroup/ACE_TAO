# Microsoft Developer Studio Project File - Name="Param_Test Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Param_Test Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="Param_Test Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Param_Test Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Param_Test Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Param_Test Client - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_DynamicInterface.lib TAO.lib ace.lib TAO_Messaging.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\tao\DynamicInterface" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\Messaging"

!ELSEIF  "$(CFG)" == "Param_Test Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_DynamicInterfaced.lib TAOd.lib aced.lib TAO_Messagingd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\tao\DynamicInterface" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\Messaging"

!ENDIF 

# Begin Target

# Name "Param_Test Client - Win32 Release"
# Name "Param_Test Client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\any.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_array_seq.cpp
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

SOURCE=.\bd_string.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_struct_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_wstr_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_wstring.cpp
# End Source File
# Begin Source File

SOURCE=.\big_union.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\complex_any.cpp
# End Source File
# Begin Source File

SOURCE=.\driver.cpp
# End Source File
# Begin Source File

SOURCE=.\except.cpp
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

SOURCE=.\multdim_array.cpp
# End Source File
# Begin Source File

SOURCE=.\nested_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\objref.cpp
# End Source File
# Begin Source File

SOURCE=.\objref_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\param_testC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\recursive_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\recursive_union.cpp
# End Source File
# Begin Source File

SOURCE=.\results.cpp
# End Source File
# Begin Source File

SOURCE=.\short.cpp
# End Source File
# Begin Source File

SOURCE=.\small_union.cpp
# End Source File
# Begin Source File

SOURCE=.\typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_any_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_array_seq.cpp
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

SOURCE=.\ub_wstr_seq.cpp
# End Source File
# Begin Source File

SOURCE=.\ub_wstring.cpp
# End Source File
# Begin Source File

SOURCE=.\ulonglong.cpp
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

SOURCE=.\bd_array_seq.h
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

SOURCE=.\bd_string.h
# End Source File
# Begin Source File

SOURCE=.\bd_struct_seq.h
# End Source File
# Begin Source File

SOURCE=.\bd_wstr_seq.h
# End Source File
# Begin Source File

SOURCE=.\bd_wstring.h
# End Source File
# Begin Source File

SOURCE=.\big_union.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\complex_any.h
# End Source File
# Begin Source File

SOURCE=.\driver.h
# End Source File
# Begin Source File

SOURCE=.\except.h
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

SOURCE=.\longlong.h
# End Source File
# Begin Source File

SOURCE=.\multdim_array.h
# End Source File
# Begin Source File

SOURCE=.\nested_struct.h
# End Source File
# Begin Source File

SOURCE=.\objref.h
# End Source File
# Begin Source File

SOURCE=.\objref_struct.h
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\param_testC.h
# End Source File
# Begin Source File

SOURCE=.\recursive_struct.h
# End Source File
# Begin Source File

SOURCE=.\recursive_union.h
# End Source File
# Begin Source File

SOURCE=.\results.h
# End Source File
# Begin Source File

SOURCE=.\short.h
# End Source File
# Begin Source File

SOURCE=.\small_union.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\ub_any_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_array_seq.h
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

SOURCE=.\ub_wstr_seq.h
# End Source File
# Begin Source File

SOURCE=.\ub_wstring.h
# End Source File
# Begin Source File

SOURCE=.\ulonglong.h
# End Source File
# Begin Source File

SOURCE=.\var_array.h
# End Source File
# Begin Source File

SOURCE=.\var_struct.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\param_testC.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\param_test.idl

!IF  "$(CFG)" == "Param_Test Client - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__PARAM="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\param_test.idl
InputName=param_test

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I..\..\tao\ -Ge 1  $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Param_Test Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PARAM="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\param_test.idl
InputName=param_test

BuildCmds= \
	..\..\..\bin\tao_idl -I..\..\tao\ -Ge 1 $(InputName).idl

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

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
