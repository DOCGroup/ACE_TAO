# Microsoft Developer Studio Project File - Name="TAO_IDL Compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=TAO_IDL Compiler - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak" CFG="TAO_IDL Compiler - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL Compiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL Compiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL Compiler - Win32 Alpha Debug" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "TAO_IDL Compiler - Win32 Alpha Release" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\Release\tao_idl.exe" /libpath:"../../ace"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "include" /I "be_include" /I "..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\tao_idl.exe" /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_IDL_"
# PROP BASE Intermediate_Dir "TAO_IDL_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "include" /I "be_include" /I "..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D IDL_CFE_VERSION=\"1.3.0\" /YX /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "include" /I "be_include" /I "..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /out:"..\..\bin\tao_idl.exe" /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TAO_IDL0"
# PROP BASE Intermediate_Dir "TAO_IDL0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D IDL_CFE_VERSION=\"1.3.0\" /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /D CPP_LOCATION="\"C:\\Program Files\\DevStudio\\VC\\bin\\CL.EXE\"" /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"../../ace"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /out:"..\..\bin\Release\tao_idl.exe" /libpath:"../../ace"
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "TAO_IDL Compiler - Win32 Release"
# Name "TAO_IDL Compiler - Win32 Debug"
# Name "TAO_IDL Compiler - Win32 Alpha Debug"
# Name "TAO_IDL Compiler - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\ast\ast_argument.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_array.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_attribute.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_check.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_concrete_type.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_constant.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_decl.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_enum.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_enum_val.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_exception.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_expression.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_field.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_generator.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_interface.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_interface_fwd.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_module.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_native.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_operation.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_predefined_type.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_recursive.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_redef.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_root.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_sequence.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_string.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_structure.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_type.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_typedef.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_union.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_union_branch.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ast\ast_union_label.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_args.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_argument.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_array.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_attribute.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_codegen.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_constant.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_decl.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_enum.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_enum_val.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_exception.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_expression.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_field.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_generator.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_helper.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_init.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_interface.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_interface_fwd.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_module.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_native.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_operation.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_predefined_type.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_produce.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_root.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_scope.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_sequence.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_stream_factory.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_string.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_structure.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_sunsoft.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_type.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_typedef.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_union.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_union_branch.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_union_label.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_valuetype.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_valuetype_fwd.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_argument.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_array.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_attribute.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_constant.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_context.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_decl.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_enum.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_exception.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_factory.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_field.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_interface.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_interface_fwd.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_module.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_operation.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_root.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_scope.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_sequence.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_structure.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_typecode.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_typedef.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_union.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_union_branch.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_valuetype.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_valuetype_fwd.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_args.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_fork.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_init.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_link.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_private.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_declarator.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_extern.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_init.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_interface_header.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_lookup.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\fe_private.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\lex.yy.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\narrow\narrow.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tao_idl.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_decllist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_error.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_exceptlist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_exprlist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_global.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_identifier.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_idlist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_indenter.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_labellist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_list.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_namelist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_scope.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_stack.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_string.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util\utl_strlist.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fe\y.tab.cpp

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Alpha Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\include\ast.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_argument.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_array.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_attribute.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_concrete_type.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_constant.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_decl.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_enum.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_enum_val.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_exception.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_expression.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_field.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_generator.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_interface.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_interface_fwd.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_module.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_native.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_operation.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_predefined_type.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_root.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_sequence.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_string.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_structure.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_type.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_typedef.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_union.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_union_branch.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_union_label.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_argument.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_array.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_attribute.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_codegen.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_constant.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_decl.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_enum.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_enum_val.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_exception.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_expression.h
# End Source File
# Begin Source File

SOURCE=.\include\be_extern.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_field.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_generator.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_helper.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_interface.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_interface_fwd.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_module.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_native.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_operation.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_predefined_type.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_root.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_scope.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_sequence.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_state.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_stream_factory.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_string.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_structure.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_sunsoft.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_type.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_typedef.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_union.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_union_branch.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_union_label.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_valuetype.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_valuetype_fwd.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_argument.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_array.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_attribute.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_constant.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_context.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_decl.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_enum.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_exception.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_factory.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_field.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_interface.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_interface_fwd.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_module.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_operation.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_root.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_scope.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_sequence.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_structure.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_typecode.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_typedef.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_union.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_union_branch.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_valuetype.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_valuetype_fwd.h
# End Source File
# Begin Source File

SOURCE=.\include\drv_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\drv_link.h
# End Source File
# Begin Source File

SOURCE=.\include\drv_private.h
# End Source File
# Begin Source File

SOURCE=.\include\fe_declarator.h
# End Source File
# Begin Source File

SOURCE=.\include\fe_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\fe_interface_header.h
# End Source File
# Begin Source File

SOURCE=.\include\fe_private.h
# End Source File
# Begin Source File

SOURCE=.\include\global_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\idl.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_bool.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_defines.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_fwd.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_global.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_narrow.h
# End Source File
# Begin Source File

SOURCE=.\include\intlmacros.h
# End Source File
# Begin Source File

SOURCE=.\include\nr_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\util.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_decllist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_error.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_exceptlist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_exprlist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_identifier.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_idlist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_indenter.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_labellist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_list.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_namelist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_scope.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_scoped_name.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_stack.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_string.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_strlist.h
# End Source File
# Begin Source File

SOURCE=.\fe\y.tab.h
# End Source File
# End Group
# End Target
# End Project
