# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=TAO_IDL - Win32 Release
!MESSAGE No configuration specified.  Defaulting to TAO_IDL - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "TAO_IDL - Win32 Release" && "$(CFG)" !=\
 "TAO_IDL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL.mak" CFG="TAO_IDL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\TAO_IDL.exe"

CLEAN : 
	-@erase "$(INTDIR)\ast_argument.obj"
	-@erase "$(INTDIR)\ast_array.obj"
	-@erase "$(INTDIR)\ast_attribute.obj"
	-@erase "$(INTDIR)\ast_check.obj"
	-@erase "$(INTDIR)\ast_concrete_type.obj"
	-@erase "$(INTDIR)\ast_constant.obj"
	-@erase "$(INTDIR)\ast_decl.obj"
	-@erase "$(INTDIR)\ast_enum.obj"
	-@erase "$(INTDIR)\ast_enum_val.obj"
	-@erase "$(INTDIR)\ast_exception.obj"
	-@erase "$(INTDIR)\ast_expression.obj"
	-@erase "$(INTDIR)\ast_field.obj"
	-@erase "$(INTDIR)\ast_generator.obj"
	-@erase "$(INTDIR)\ast_interface.obj"
	-@erase "$(INTDIR)\ast_interface_fwd.obj"
	-@erase "$(INTDIR)\ast_module.obj"
	-@erase "$(INTDIR)\ast_operation.obj"
	-@erase "$(INTDIR)\ast_predefined_type.obj"
	-@erase "$(INTDIR)\ast_recursive.obj"
	-@erase "$(INTDIR)\ast_redef.obj"
	-@erase "$(INTDIR)\ast_root.obj"
	-@erase "$(INTDIR)\ast_sequence.obj"
	-@erase "$(INTDIR)\ast_string.obj"
	-@erase "$(INTDIR)\ast_structure.obj"
	-@erase "$(INTDIR)\ast_type.obj"
	-@erase "$(INTDIR)\ast_typedef.obj"
	-@erase "$(INTDIR)\ast_union.obj"
	-@erase "$(INTDIR)\ast_union_branch.obj"
	-@erase "$(INTDIR)\ast_union_label.obj"
	-@erase "$(INTDIR)\be_args.obj"
	-@erase "$(INTDIR)\be_argument.obj"
	-@erase "$(INTDIR)\be_array.obj"
	-@erase "$(INTDIR)\be_attribute.obj"
	-@erase "$(INTDIR)\be_codegen.obj"
	-@erase "$(INTDIR)\be_constant.obj"
	-@erase "$(INTDIR)\be_decl.obj"
	-@erase "$(INTDIR)\be_enum.obj"
	-@erase "$(INTDIR)\be_enum_val.obj"
	-@erase "$(INTDIR)\be_exception.obj"
	-@erase "$(INTDIR)\be_expression.obj"
	-@erase "$(INTDIR)\be_factory.obj"
	-@erase "$(INTDIR)\be_field.obj"
	-@erase "$(INTDIR)\be_generator.obj"
	-@erase "$(INTDIR)\be_helper.obj"
	-@erase "$(INTDIR)\be_init.obj"
	-@erase "$(INTDIR)\be_interface.obj"
	-@erase "$(INTDIR)\be_interface_fwd.obj"
	-@erase "$(INTDIR)\be_module.obj"
	-@erase "$(INTDIR)\be_operation.obj"
	-@erase "$(INTDIR)\be_predefined_type.obj"
	-@erase "$(INTDIR)\be_produce.obj"
	-@erase "$(INTDIR)\be_root.obj"
	-@erase "$(INTDIR)\be_scope.obj"
	-@erase "$(INTDIR)\be_sequence.obj"
	-@erase "$(INTDIR)\be_state.obj"
	-@erase "$(INTDIR)\be_string.obj"
	-@erase "$(INTDIR)\be_structure.obj"
	-@erase "$(INTDIR)\be_sunsoft.obj"
	-@erase "$(INTDIR)\be_type.obj"
	-@erase "$(INTDIR)\be_typedef.obj"
	-@erase "$(INTDIR)\be_union.obj"
	-@erase "$(INTDIR)\be_union_branch.obj"
	-@erase "$(INTDIR)\be_union_label.obj"
	-@erase "$(INTDIR)\drv_args.obj"
	-@erase "$(INTDIR)\drv_fork.obj"
	-@erase "$(INTDIR)\drv_init.obj"
	-@erase "$(INTDIR)\drv_link.obj"
	-@erase "$(INTDIR)\drv_main.obj"
	-@erase "$(INTDIR)\drv_preproc.obj"
	-@erase "$(INTDIR)\drv_private.obj"
	-@erase "$(INTDIR)\fe_declarator.obj"
	-@erase "$(INTDIR)\fe_extern.obj"
	-@erase "$(INTDIR)\fe_init.obj"
	-@erase "$(INTDIR)\fe_interface_header.obj"
	-@erase "$(INTDIR)\fe_private.obj"
	-@erase "$(INTDIR)\lex.yy.obj"
	-@erase "$(INTDIR)\narrow.obj"
	-@erase "$(INTDIR)\utl_decllist.obj"
	-@erase "$(INTDIR)\utl_error.obj"
	-@erase "$(INTDIR)\utl_exceptlist.obj"
	-@erase "$(INTDIR)\utl_exprlist.obj"
	-@erase "$(INTDIR)\utl_global.obj"
	-@erase "$(INTDIR)\utl_identifier.obj"
	-@erase "$(INTDIR)\utl_idlist.obj"
	-@erase "$(INTDIR)\utl_indenter.obj"
	-@erase "$(INTDIR)\utl_labellist.obj"
	-@erase "$(INTDIR)\utl_list.obj"
	-@erase "$(INTDIR)\utl_namelist.obj"
	-@erase "$(INTDIR)\utl_scope.obj"
	-@erase "$(INTDIR)\utl_stack.obj"
	-@erase "$(INTDIR)\utl_string.obj"
	-@erase "$(INTDIR)\utl_strlist.obj"
	-@erase "$(INTDIR)\y.tab.obj"
	-@erase "$(OUTDIR)\TAO_IDL.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D CPP_LOCATION=\"cl\" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D\
 "NDEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D\
 CPP_LOCATION=\"cl\" /Fp"$(INTDIR)/TAO_IDL.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_IDL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/tao_idl.pdb" /machine:I386 /out:"$(OUTDIR)/tao_idl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ast_argument.obj" \
	"$(INTDIR)\ast_array.obj" \
	"$(INTDIR)\ast_attribute.obj" \
	"$(INTDIR)\ast_check.obj" \
	"$(INTDIR)\ast_concrete_type.obj" \
	"$(INTDIR)\ast_constant.obj" \
	"$(INTDIR)\ast_decl.obj" \
	"$(INTDIR)\ast_enum.obj" \
	"$(INTDIR)\ast_enum_val.obj" \
	"$(INTDIR)\ast_exception.obj" \
	"$(INTDIR)\ast_expression.obj" \
	"$(INTDIR)\ast_field.obj" \
	"$(INTDIR)\ast_generator.obj" \
	"$(INTDIR)\ast_interface.obj" \
	"$(INTDIR)\ast_interface_fwd.obj" \
	"$(INTDIR)\ast_module.obj" \
	"$(INTDIR)\ast_operation.obj" \
	"$(INTDIR)\ast_predefined_type.obj" \
	"$(INTDIR)\ast_recursive.obj" \
	"$(INTDIR)\ast_redef.obj" \
	"$(INTDIR)\ast_root.obj" \
	"$(INTDIR)\ast_sequence.obj" \
	"$(INTDIR)\ast_string.obj" \
	"$(INTDIR)\ast_structure.obj" \
	"$(INTDIR)\ast_type.obj" \
	"$(INTDIR)\ast_typedef.obj" \
	"$(INTDIR)\ast_union.obj" \
	"$(INTDIR)\ast_union_branch.obj" \
	"$(INTDIR)\ast_union_label.obj" \
	"$(INTDIR)\be_args.obj" \
	"$(INTDIR)\be_argument.obj" \
	"$(INTDIR)\be_array.obj" \
	"$(INTDIR)\be_attribute.obj" \
	"$(INTDIR)\be_codegen.obj" \
	"$(INTDIR)\be_constant.obj" \
	"$(INTDIR)\be_decl.obj" \
	"$(INTDIR)\be_enum.obj" \
	"$(INTDIR)\be_enum_val.obj" \
	"$(INTDIR)\be_exception.obj" \
	"$(INTDIR)\be_expression.obj" \
	"$(INTDIR)\be_factory.obj" \
	"$(INTDIR)\be_field.obj" \
	"$(INTDIR)\be_generator.obj" \
	"$(INTDIR)\be_helper.obj" \
	"$(INTDIR)\be_init.obj" \
	"$(INTDIR)\be_interface.obj" \
	"$(INTDIR)\be_interface_fwd.obj" \
	"$(INTDIR)\be_module.obj" \
	"$(INTDIR)\be_operation.obj" \
	"$(INTDIR)\be_predefined_type.obj" \
	"$(INTDIR)\be_produce.obj" \
	"$(INTDIR)\be_root.obj" \
	"$(INTDIR)\be_scope.obj" \
	"$(INTDIR)\be_sequence.obj" \
	"$(INTDIR)\be_state.obj" \
	"$(INTDIR)\be_string.obj" \
	"$(INTDIR)\be_structure.obj" \
	"$(INTDIR)\be_sunsoft.obj" \
	"$(INTDIR)\be_type.obj" \
	"$(INTDIR)\be_typedef.obj" \
	"$(INTDIR)\be_union.obj" \
	"$(INTDIR)\be_union_branch.obj" \
	"$(INTDIR)\be_union_label.obj" \
	"$(INTDIR)\drv_args.obj" \
	"$(INTDIR)\drv_fork.obj" \
	"$(INTDIR)\drv_init.obj" \
	"$(INTDIR)\drv_link.obj" \
	"$(INTDIR)\drv_main.obj" \
	"$(INTDIR)\drv_preproc.obj" \
	"$(INTDIR)\drv_private.obj" \
	"$(INTDIR)\fe_declarator.obj" \
	"$(INTDIR)\fe_extern.obj" \
	"$(INTDIR)\fe_init.obj" \
	"$(INTDIR)\fe_interface_header.obj" \
	"$(INTDIR)\fe_private.obj" \
	"$(INTDIR)\lex.yy.obj" \
	"$(INTDIR)\narrow.obj" \
	"$(INTDIR)\utl_decllist.obj" \
	"$(INTDIR)\utl_error.obj" \
	"$(INTDIR)\utl_exceptlist.obj" \
	"$(INTDIR)\utl_exprlist.obj" \
	"$(INTDIR)\utl_global.obj" \
	"$(INTDIR)\utl_identifier.obj" \
	"$(INTDIR)\utl_idlist.obj" \
	"$(INTDIR)\utl_indenter.obj" \
	"$(INTDIR)\utl_labellist.obj" \
	"$(INTDIR)\utl_list.obj" \
	"$(INTDIR)\utl_namelist.obj" \
	"$(INTDIR)\utl_scope.obj" \
	"$(INTDIR)\utl_stack.obj" \
	"$(INTDIR)\utl_string.obj" \
	"$(INTDIR)\utl_strlist.obj" \
	"$(INTDIR)\y.tab.obj"

"$(OUTDIR)\TAO_IDL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\TAO_IDL.exe"

CLEAN : 
	-@erase "$(INTDIR)\ast_argument.obj"
	-@erase "$(INTDIR)\ast_array.obj"
	-@erase "$(INTDIR)\ast_attribute.obj"
	-@erase "$(INTDIR)\ast_check.obj"
	-@erase "$(INTDIR)\ast_concrete_type.obj"
	-@erase "$(INTDIR)\ast_constant.obj"
	-@erase "$(INTDIR)\ast_decl.obj"
	-@erase "$(INTDIR)\ast_enum.obj"
	-@erase "$(INTDIR)\ast_enum_val.obj"
	-@erase "$(INTDIR)\ast_exception.obj"
	-@erase "$(INTDIR)\ast_expression.obj"
	-@erase "$(INTDIR)\ast_field.obj"
	-@erase "$(INTDIR)\ast_generator.obj"
	-@erase "$(INTDIR)\ast_interface.obj"
	-@erase "$(INTDIR)\ast_interface_fwd.obj"
	-@erase "$(INTDIR)\ast_module.obj"
	-@erase "$(INTDIR)\ast_operation.obj"
	-@erase "$(INTDIR)\ast_predefined_type.obj"
	-@erase "$(INTDIR)\ast_recursive.obj"
	-@erase "$(INTDIR)\ast_redef.obj"
	-@erase "$(INTDIR)\ast_root.obj"
	-@erase "$(INTDIR)\ast_sequence.obj"
	-@erase "$(INTDIR)\ast_string.obj"
	-@erase "$(INTDIR)\ast_structure.obj"
	-@erase "$(INTDIR)\ast_type.obj"
	-@erase "$(INTDIR)\ast_typedef.obj"
	-@erase "$(INTDIR)\ast_union.obj"
	-@erase "$(INTDIR)\ast_union_branch.obj"
	-@erase "$(INTDIR)\ast_union_label.obj"
	-@erase "$(INTDIR)\be_args.obj"
	-@erase "$(INTDIR)\be_argument.obj"
	-@erase "$(INTDIR)\be_array.obj"
	-@erase "$(INTDIR)\be_attribute.obj"
	-@erase "$(INTDIR)\be_codegen.obj"
	-@erase "$(INTDIR)\be_constant.obj"
	-@erase "$(INTDIR)\be_decl.obj"
	-@erase "$(INTDIR)\be_enum.obj"
	-@erase "$(INTDIR)\be_enum_val.obj"
	-@erase "$(INTDIR)\be_exception.obj"
	-@erase "$(INTDIR)\be_expression.obj"
	-@erase "$(INTDIR)\be_factory.obj"
	-@erase "$(INTDIR)\be_field.obj"
	-@erase "$(INTDIR)\be_generator.obj"
	-@erase "$(INTDIR)\be_helper.obj"
	-@erase "$(INTDIR)\be_init.obj"
	-@erase "$(INTDIR)\be_interface.obj"
	-@erase "$(INTDIR)\be_interface_fwd.obj"
	-@erase "$(INTDIR)\be_module.obj"
	-@erase "$(INTDIR)\be_operation.obj"
	-@erase "$(INTDIR)\be_predefined_type.obj"
	-@erase "$(INTDIR)\be_produce.obj"
	-@erase "$(INTDIR)\be_root.obj"
	-@erase "$(INTDIR)\be_scope.obj"
	-@erase "$(INTDIR)\be_sequence.obj"
	-@erase "$(INTDIR)\be_state.obj"
	-@erase "$(INTDIR)\be_string.obj"
	-@erase "$(INTDIR)\be_structure.obj"
	-@erase "$(INTDIR)\be_sunsoft.obj"
	-@erase "$(INTDIR)\be_type.obj"
	-@erase "$(INTDIR)\be_typedef.obj"
	-@erase "$(INTDIR)\be_union.obj"
	-@erase "$(INTDIR)\be_union_branch.obj"
	-@erase "$(INTDIR)\be_union_label.obj"
	-@erase "$(INTDIR)\drv_args.obj"
	-@erase "$(INTDIR)\drv_fork.obj"
	-@erase "$(INTDIR)\drv_init.obj"
	-@erase "$(INTDIR)\drv_link.obj"
	-@erase "$(INTDIR)\drv_main.obj"
	-@erase "$(INTDIR)\drv_preproc.obj"
	-@erase "$(INTDIR)\drv_private.obj"
	-@erase "$(INTDIR)\fe_declarator.obj"
	-@erase "$(INTDIR)\fe_extern.obj"
	-@erase "$(INTDIR)\fe_init.obj"
	-@erase "$(INTDIR)\fe_interface_header.obj"
	-@erase "$(INTDIR)\fe_private.obj"
	-@erase "$(INTDIR)\lex.yy.obj"
	-@erase "$(INTDIR)\narrow.obj"
	-@erase "$(INTDIR)\utl_decllist.obj"
	-@erase "$(INTDIR)\utl_error.obj"
	-@erase "$(INTDIR)\utl_exceptlist.obj"
	-@erase "$(INTDIR)\utl_exprlist.obj"
	-@erase "$(INTDIR)\utl_global.obj"
	-@erase "$(INTDIR)\utl_identifier.obj"
	-@erase "$(INTDIR)\utl_idlist.obj"
	-@erase "$(INTDIR)\utl_indenter.obj"
	-@erase "$(INTDIR)\utl_labellist.obj"
	-@erase "$(INTDIR)\utl_list.obj"
	-@erase "$(INTDIR)\utl_namelist.obj"
	-@erase "$(INTDIR)\utl_scope.obj"
	-@erase "$(INTDIR)\utl_stack.obj"
	-@erase "$(INTDIR)\utl_string.obj"
	-@erase "$(INTDIR)\utl_strlist.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\y.tab.obj"
	-@erase "$(OUTDIR)\TAO_IDL.exe"
	-@erase "$(OUTDIR)\TAO_IDL.ilk"
	-@erase "$(OUTDIR)\TAO_IDL.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "..\.." /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D CPP_LOCATION=\"cl\" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I\
 "..\.." /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D\
 CPP_LOCATION=\"cl\" /Fp"$(INTDIR)/TAO_IDL.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_IDL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/tao_idl.pdb" /debug /machine:I386 /out:"$(OUTDIR)/tao_idl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ast_argument.obj" \
	"$(INTDIR)\ast_array.obj" \
	"$(INTDIR)\ast_attribute.obj" \
	"$(INTDIR)\ast_check.obj" \
	"$(INTDIR)\ast_concrete_type.obj" \
	"$(INTDIR)\ast_constant.obj" \
	"$(INTDIR)\ast_decl.obj" \
	"$(INTDIR)\ast_enum.obj" \
	"$(INTDIR)\ast_enum_val.obj" \
	"$(INTDIR)\ast_exception.obj" \
	"$(INTDIR)\ast_expression.obj" \
	"$(INTDIR)\ast_field.obj" \
	"$(INTDIR)\ast_generator.obj" \
	"$(INTDIR)\ast_interface.obj" \
	"$(INTDIR)\ast_interface_fwd.obj" \
	"$(INTDIR)\ast_module.obj" \
	"$(INTDIR)\ast_operation.obj" \
	"$(INTDIR)\ast_predefined_type.obj" \
	"$(INTDIR)\ast_recursive.obj" \
	"$(INTDIR)\ast_redef.obj" \
	"$(INTDIR)\ast_root.obj" \
	"$(INTDIR)\ast_sequence.obj" \
	"$(INTDIR)\ast_string.obj" \
	"$(INTDIR)\ast_structure.obj" \
	"$(INTDIR)\ast_type.obj" \
	"$(INTDIR)\ast_typedef.obj" \
	"$(INTDIR)\ast_union.obj" \
	"$(INTDIR)\ast_union_branch.obj" \
	"$(INTDIR)\ast_union_label.obj" \
	"$(INTDIR)\be_args.obj" \
	"$(INTDIR)\be_argument.obj" \
	"$(INTDIR)\be_array.obj" \
	"$(INTDIR)\be_attribute.obj" \
	"$(INTDIR)\be_codegen.obj" \
	"$(INTDIR)\be_constant.obj" \
	"$(INTDIR)\be_decl.obj" \
	"$(INTDIR)\be_enum.obj" \
	"$(INTDIR)\be_enum_val.obj" \
	"$(INTDIR)\be_exception.obj" \
	"$(INTDIR)\be_expression.obj" \
	"$(INTDIR)\be_factory.obj" \
	"$(INTDIR)\be_field.obj" \
	"$(INTDIR)\be_generator.obj" \
	"$(INTDIR)\be_helper.obj" \
	"$(INTDIR)\be_init.obj" \
	"$(INTDIR)\be_interface.obj" \
	"$(INTDIR)\be_interface_fwd.obj" \
	"$(INTDIR)\be_module.obj" \
	"$(INTDIR)\be_operation.obj" \
	"$(INTDIR)\be_predefined_type.obj" \
	"$(INTDIR)\be_produce.obj" \
	"$(INTDIR)\be_root.obj" \
	"$(INTDIR)\be_scope.obj" \
	"$(INTDIR)\be_sequence.obj" \
	"$(INTDIR)\be_state.obj" \
	"$(INTDIR)\be_string.obj" \
	"$(INTDIR)\be_structure.obj" \
	"$(INTDIR)\be_sunsoft.obj" \
	"$(INTDIR)\be_type.obj" \
	"$(INTDIR)\be_typedef.obj" \
	"$(INTDIR)\be_union.obj" \
	"$(INTDIR)\be_union_branch.obj" \
	"$(INTDIR)\be_union_label.obj" \
	"$(INTDIR)\drv_args.obj" \
	"$(INTDIR)\drv_fork.obj" \
	"$(INTDIR)\drv_init.obj" \
	"$(INTDIR)\drv_link.obj" \
	"$(INTDIR)\drv_main.obj" \
	"$(INTDIR)\drv_preproc.obj" \
	"$(INTDIR)\drv_private.obj" \
	"$(INTDIR)\fe_declarator.obj" \
	"$(INTDIR)\fe_extern.obj" \
	"$(INTDIR)\fe_init.obj" \
	"$(INTDIR)\fe_interface_header.obj" \
	"$(INTDIR)\fe_private.obj" \
	"$(INTDIR)\lex.yy.obj" \
	"$(INTDIR)\narrow.obj" \
	"$(INTDIR)\utl_decllist.obj" \
	"$(INTDIR)\utl_error.obj" \
	"$(INTDIR)\utl_exceptlist.obj" \
	"$(INTDIR)\utl_exprlist.obj" \
	"$(INTDIR)\utl_global.obj" \
	"$(INTDIR)\utl_identifier.obj" \
	"$(INTDIR)\utl_idlist.obj" \
	"$(INTDIR)\utl_indenter.obj" \
	"$(INTDIR)\utl_labellist.obj" \
	"$(INTDIR)\utl_list.obj" \
	"$(INTDIR)\utl_namelist.obj" \
	"$(INTDIR)\utl_scope.obj" \
	"$(INTDIR)\utl_stack.obj" \
	"$(INTDIR)\utl_string.obj" \
	"$(INTDIR)\utl_strlist.obj" \
	"$(INTDIR)\y.tab.obj"

"$(OUTDIR)\TAO_IDL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "TAO_IDL - Win32 Release"
# Name "TAO_IDL - Win32 Debug"

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\util\utl_decllist.cpp
NODEP_CPP_UTL_D=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_decllist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_error.cpp
NODEP_CPP_UTL_E=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_error.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_exceptlist.cpp
NODEP_CPP_UTL_EX=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_exceptlist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_exprlist.cpp
NODEP_CPP_UTL_EXP=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_exprlist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_global.cpp
NODEP_CPP_UTL_G=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_global.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_identifier.cpp
NODEP_CPP_UTL_I=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_identifier.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_idlist.cpp
NODEP_CPP_UTL_ID=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_idlist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_indenter.cpp
NODEP_CPP_UTL_IN=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_indenter.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_labellist.cpp
NODEP_CPP_UTL_L=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_labellist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_list.cpp
NODEP_CPP_UTL_LI=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_list.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_namelist.cpp
NODEP_CPP_UTL_N=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_namelist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_scope.cpp
NODEP_CPP_UTL_S=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_scope.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_stack.cpp
NODEP_CPP_UTL_ST=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_stack.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_string.cpp
NODEP_CPP_UTL_STR=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_string.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_strlist.cpp
NODEP_CPP_UTL_STRL=\
	".\util\idl.h"\
	".\util\idl_extern.h"\
	

"$(INTDIR)\utl_strlist.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\narrow\narrow.cpp
NODEP_CPP_NARRO=\
	".\narrow\idl.h"\
	".\narrow\idl_extern.h"\
	

"$(INTDIR)\narrow.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_declarator.cpp
NODEP_CPP_FE_DE=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\fe_declarator.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_extern.cpp
NODEP_CPP_FE_EX=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	".\fe\utl_error.h"\
	".\fe\utl_indenter.h"\
	".\fe\utl_string.h"\
	

"$(INTDIR)\fe_extern.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_init.cpp
NODEP_CPP_FE_IN=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\fe_init.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_interface_header.cpp
NODEP_CPP_FE_INT=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\fe_interface_header.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_private.cpp
NODEP_CPP_FE_PR=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\fe_private.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\lex.yy.cpp
DEP_CPP_LEX_Y=\
	".\fe\y.tab.h"\
	
NODEP_CPP_LEX_Y=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\lex.yy.obj" : $(SOURCE) $(DEP_CPP_LEX_Y) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\y.tab.cpp
NODEP_CPP_Y_TAB=\
	".\fe\fe_private.h"\
	".\fe\idl.h"\
	".\fe\idl_extern.h"\
	

"$(INTDIR)\y.tab.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_args.cpp
NODEP_CPP_DRV_A=\
	".\driver\drv_link.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_args.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_fork.cpp
DEP_CPP_DRV_F=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Atomic_Op.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config-win32.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Managed_Object.cpp"\
	{$(INCLUDE)}"\ace\Managed_Object.h"\
	{$(INCLUDE)}"\ace\Managed_Object.i"\
	{$(INCLUDE)}"\ace\Object_Manager.h"\
	{$(INCLUDE)}"\ace\Object_Manager.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Process.h"\
	{$(INCLUDE)}"\ace\Process.i"\
	{$(INCLUDE)}"\ace\Process_Manager.h"\
	{$(INCLUDE)}"\ace\Process_Manager.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Version.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	
NODEP_CPP_DRV_F=\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_fork.obj" : $(SOURCE) $(DEP_CPP_DRV_F) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_init.cpp
NODEP_CPP_DRV_I=\
	".\driver\drv_link.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_init.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_link.cpp
NODEP_CPP_DRV_L=\
	".\driver\drv_link.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_link.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_main.cpp
NODEP_CPP_DRV_M=\
	".\driver\drv_link.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_main.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
DEP_CPP_DRV_P=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Atomic_Op.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config-win32.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Managed_Object.cpp"\
	{$(INCLUDE)}"\ace\Managed_Object.h"\
	{$(INCLUDE)}"\ace\Managed_Object.i"\
	{$(INCLUDE)}"\ace\Object_Manager.h"\
	{$(INCLUDE)}"\ace\Object_Manager.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Process.h"\
	{$(INCLUDE)}"\ace\Process.i"\
	{$(INCLUDE)}"\ace\Process_Manager.h"\
	{$(INCLUDE)}"\ace\Process_Manager.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Version.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	
NODEP_CPP_DRV_P=\
	".\driver\drv_link.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_preproc.obj" : $(SOURCE) $(DEP_CPP_DRV_P) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_private.cpp
NODEP_CPP_DRV_PR=\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

"$(INTDIR)\drv_private.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_args.cpp
NODEP_CPP_BE_AR=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_args.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_argument.cpp
NODEP_CPP_BE_ARG=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_argument.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_array.cpp
NODEP_CPP_BE_ARR=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_array.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_attribute.cpp
NODEP_CPP_BE_AT=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_attribute.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_codegen.cpp
NODEP_CPP_BE_CO=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_codegen.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_constant.cpp
NODEP_CPP_BE_CON=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_constant.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_decl.cpp
NODEP_CPP_BE_DE=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_decl.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_enum.cpp
NODEP_CPP_BE_EN=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_enum.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_enum_val.cpp
NODEP_CPP_BE_ENU=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_enum_val.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_exception.cpp
NODEP_CPP_BE_EX=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_exception.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_expression.cpp
NODEP_CPP_BE_EXP=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_expression.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_factory.cpp
NODEP_CPP_BE_FA=\
	".\be\be.h"\
	".\be\be_sunsoft.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_factory.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_field.cpp
NODEP_CPP_BE_FI=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_field.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_generator.cpp
NODEP_CPP_BE_GE=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_generator.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_helper.cpp
NODEP_CPP_BE_HE=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_helper.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_init.cpp
NODEP_CPP_BE_IN=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_init.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_interface.cpp
NODEP_CPP_BE_INT=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_interface.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_interface_fwd.cpp
NODEP_CPP_BE_INTE=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_interface_fwd.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_module.cpp
NODEP_CPP_BE_MO=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_module.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_operation.cpp
NODEP_CPP_BE_OP=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_operation.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_predefined_type.cpp
NODEP_CPP_BE_PR=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_predefined_type.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_produce.cpp
NODEP_CPP_BE_PRO=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_produce.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_root.cpp
NODEP_CPP_BE_RO=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_root.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_scope.cpp
NODEP_CPP_BE_SC=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_scope.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_sequence.cpp
NODEP_CPP_BE_SE=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_sequence.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state.cpp
NODEP_CPP_BE_ST=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_state.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_string.cpp
NODEP_CPP_BE_STR=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_string.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_structure.cpp
NODEP_CPP_BE_STRU=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_structure.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_sunsoft.cpp
NODEP_CPP_BE_SU=\
	".\be\be.h"\
	".\be\be_sunsoft.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_sunsoft.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_type.cpp
NODEP_CPP_BE_TY=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_type.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_typedef.cpp
NODEP_CPP_BE_TYP=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_typedef.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union.cpp
NODEP_CPP_BE_UN=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_union.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union_branch.cpp
NODEP_CPP_BE_UNI=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_union_branch.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union_label.cpp
NODEP_CPP_BE_UNIO=\
	".\be\be.h"\
	".\be\idl.h"\
	".\be\idl_extern.h"\
	

"$(INTDIR)\be_union_label.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_argument.cpp
NODEP_CPP_AST_A=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_argument.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_array.cpp
NODEP_CPP_AST_AR=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_array.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_attribute.cpp
NODEP_CPP_AST_AT=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_attribute.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_check.cpp
NODEP_CPP_AST_C=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_check.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_concrete_type.cpp
NODEP_CPP_AST_CO=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_concrete_type.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_constant.cpp
NODEP_CPP_AST_CON=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_constant.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_decl.cpp
NODEP_CPP_AST_D=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_decl.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_enum.cpp
NODEP_CPP_AST_E=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_enum.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_enum_val.cpp
NODEP_CPP_AST_EN=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_enum_val.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_exception.cpp
NODEP_CPP_AST_EX=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_exception.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_expression.cpp
NODEP_CPP_AST_EXP=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_expression.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_field.cpp
NODEP_CPP_AST_F=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_field.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_generator.cpp
NODEP_CPP_AST_G=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_generator.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_interface.cpp
NODEP_CPP_AST_I=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_interface.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_interface_fwd.cpp
NODEP_CPP_AST_IN=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_interface_fwd.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_module.cpp
NODEP_CPP_AST_M=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_module.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_operation.cpp
NODEP_CPP_AST_O=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_operation.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_predefined_type.cpp
NODEP_CPP_AST_P=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_predefined_type.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_recursive.cpp
NODEP_CPP_AST_R=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_recursive.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_redef.cpp
NODEP_CPP_AST_RE=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_redef.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_root.cpp
NODEP_CPP_AST_RO=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_root.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_sequence.cpp
NODEP_CPP_AST_S=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_sequence.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_string.cpp
NODEP_CPP_AST_ST=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_string.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_structure.cpp
NODEP_CPP_AST_STR=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_structure.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_type.cpp
NODEP_CPP_AST_T=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_type.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_typedef.cpp
NODEP_CPP_AST_TY=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_typedef.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union.cpp
NODEP_CPP_AST_U=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_union.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union_branch.cpp
NODEP_CPP_AST_UN=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_union_branch.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union_label.cpp
NODEP_CPP_AST_UNI=\
	".\ast\idl.h"\
	".\ast\idl_extern.h"\
	

"$(INTDIR)\ast_union_label.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
