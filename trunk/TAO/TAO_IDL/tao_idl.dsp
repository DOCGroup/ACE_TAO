# Microsoft Developer Studio Project File - Name="tao_idl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=tao_idl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak" CFG="tao_idl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tao_idl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tao_idl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tao_idl - Win32 Release"

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

!ELSEIF  "$(CFG)" == "tao_idl - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D IDL_CFE_VERSION=\"1.3.0\" /YX /FD /D CPP_LOCATION="\"\\Program Files\\DevStudio\\Vc\\bin\\Cl\"" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "tao_idl - Win32 Release"
# Name "tao_idl - Win32 Debug"
# Begin Source File

SOURCE=.\ast\ast_argument.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_array.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_check.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_concrete_type.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_constant.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_decl.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_enum.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_enum_val.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_exception.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_expression.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_field.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_generator.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_interface_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_module.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_operation.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_predefined_type.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_recursive.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_redef.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_root.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_string.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_structure.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_type.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_union.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_union_branch.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_union_label.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_args.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_argument.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_array.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_codegen.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_constant.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_decl.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_enum.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_enum_val.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_exception.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_expression.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_field.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_generator.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_init.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_interface_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_module.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_operation.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_predefined_type.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_produce.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_root.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_scope.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_state.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_string.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_structure.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_sunsoft.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_type.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_union.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_union_branch.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_union_label.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_args.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_fork.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_init.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_link.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_main.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_private.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_declarator.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_extern.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_init.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_interface_header.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_private.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\lex.yy.cpp
# End Source File
# Begin Source File

SOURCE=.\narrow\narrow.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_decllist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_error.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_exceptlist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_exprlist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_global.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_identifier.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_idlist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_indenter.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_labellist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_list.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_namelist.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_scope.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_stack.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_string.cpp
# End Source File
# Begin Source File

SOURCE=.\util\utl_strlist.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\y.tab.cpp
# End Source File
# End Target
# End Project
