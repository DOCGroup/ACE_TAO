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
!MESSAGE NMAKE /f "TAO_IDL4.mak" CFG="TAO_IDL - Win32 Release"
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
# PROP Target_Last_Scanned "TAO_IDL - Win32 Release"
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
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\TAO_IDL4.exe"

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
	-@erase "$(INTDIR)\be_state_argument.obj"
	-@erase "$(INTDIR)\be_state_array.obj"
	-@erase "$(INTDIR)\be_state_attribute.obj"
	-@erase "$(INTDIR)\be_state_operation.obj"
	-@erase "$(INTDIR)\be_state_sequence.obj"
	-@erase "$(INTDIR)\be_state_structure.obj"
	-@erase "$(INTDIR)\be_state_typedef.obj"
	-@erase "$(INTDIR)\be_state_union.obj"
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
	-@erase "$(OUTDIR)\TAO_IDL4.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D CPP_LOCATION=\"cl\" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "..\.." /D\
 "NDEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D\
 CPP_LOCATION=\"cl\" /Fp"$(INTDIR)/TAO_IDL4.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_IDL4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/TAO_IDL4.pdb" /machine:I386 /out:"$(OUTDIR)/TAO_IDL4.exe" 
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
	"$(INTDIR)\be_state_argument.obj" \
	"$(INTDIR)\be_state_array.obj" \
	"$(INTDIR)\be_state_attribute.obj" \
	"$(INTDIR)\be_state_operation.obj" \
	"$(INTDIR)\be_state_sequence.obj" \
	"$(INTDIR)\be_state_structure.obj" \
	"$(INTDIR)\be_state_typedef.obj" \
	"$(INTDIR)\be_state_union.obj" \
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

"$(OUTDIR)\TAO_IDL4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\TAO_IDL4.exe"

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
	-@erase "$(INTDIR)\be_state_argument.obj"
	-@erase "$(INTDIR)\be_state_array.obj"
	-@erase "$(INTDIR)\be_state_attribute.obj"
	-@erase "$(INTDIR)\be_state_operation.obj"
	-@erase "$(INTDIR)\be_state_sequence.obj"
	-@erase "$(INTDIR)\be_state_structure.obj"
	-@erase "$(INTDIR)\be_state_typedef.obj"
	-@erase "$(INTDIR)\be_state_union.obj"
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
	-@erase "$(OUTDIR)\TAO_IDL4.exe"
	-@erase "$(OUTDIR)\TAO_IDL4.ilk"
	-@erase "$(OUTDIR)\TAO_IDL4.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "..\.." /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D CPP_LOCATION=\"cl\" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I\
 "..\.." /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D IDL_CFE_VERSION=\"1.3.0\" /D\
 CPP_LOCATION=\"cl\" /Fp"$(INTDIR)/TAO_IDL4.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_IDL4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/TAO_IDL4.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/TAO_IDL4.exe" 
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
	"$(INTDIR)\be_state_argument.obj" \
	"$(INTDIR)\be_state_array.obj" \
	"$(INTDIR)\be_state_attribute.obj" \
	"$(INTDIR)\be_state_operation.obj" \
	"$(INTDIR)\be_state_sequence.obj" \
	"$(INTDIR)\be_state_structure.obj" \
	"$(INTDIR)\be_state_typedef.obj" \
	"$(INTDIR)\be_state_union.obj" \
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

"$(OUTDIR)\TAO_IDL4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_D=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_decllist.obj" : $(SOURCE) $(DEP_CPP_UTL_D) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_D=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_decllist.obj" : $(SOURCE) $(DEP_CPP_UTL_D) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_error.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_E=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_error.obj" : $(SOURCE) $(DEP_CPP_UTL_E) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_E=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_error.obj" : $(SOURCE) $(DEP_CPP_UTL_E) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_exceptlist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_exceptlist.obj" : $(SOURCE) $(DEP_CPP_UTL_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_exceptlist.obj" : $(SOURCE) $(DEP_CPP_UTL_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_exprlist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_EXP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_exprlist.obj" : $(SOURCE) $(DEP_CPP_UTL_EXP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_EXP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_exprlist.obj" : $(SOURCE) $(DEP_CPP_UTL_EXP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_global.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_G=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_global.obj" : $(SOURCE) $(DEP_CPP_UTL_G) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_G=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_global.obj" : $(SOURCE) $(DEP_CPP_UTL_G) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_identifier.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_I=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_identifier.obj" : $(SOURCE) $(DEP_CPP_UTL_I) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_I=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_identifier.obj" : $(SOURCE) $(DEP_CPP_UTL_I) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_idlist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_ID=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_idlist.obj" : $(SOURCE) $(DEP_CPP_UTL_ID) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_ID=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_idlist.obj" : $(SOURCE) $(DEP_CPP_UTL_ID) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_indenter.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_indenter.obj" : $(SOURCE) $(DEP_CPP_UTL_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_indenter.obj" : $(SOURCE) $(DEP_CPP_UTL_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_labellist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_L=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_labellist.obj" : $(SOURCE) $(DEP_CPP_UTL_L) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_L=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_labellist.obj" : $(SOURCE) $(DEP_CPP_UTL_L) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_list.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_LI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_list.obj" : $(SOURCE) $(DEP_CPP_UTL_LI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_LI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_list.obj" : $(SOURCE) $(DEP_CPP_UTL_LI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_namelist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_N=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_namelist.obj" : $(SOURCE) $(DEP_CPP_UTL_N) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_N=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_namelist.obj" : $(SOURCE) $(DEP_CPP_UTL_N) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_scope.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_S=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_scope.obj" : $(SOURCE) $(DEP_CPP_UTL_S) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_S=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_scope.obj" : $(SOURCE) $(DEP_CPP_UTL_S) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_stack.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_ST=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_stack.obj" : $(SOURCE) $(DEP_CPP_UTL_ST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_ST=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_stack.obj" : $(SOURCE) $(DEP_CPP_UTL_ST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_string.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_STR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_string.obj" : $(SOURCE) $(DEP_CPP_UTL_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_STR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_string.obj" : $(SOURCE) $(DEP_CPP_UTL_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\util\utl_strlist.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_UTL_STRL=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_strlist.obj" : $(SOURCE) $(DEP_CPP_UTL_STRL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_UTL_STRL=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utl_strlist.obj" : $(SOURCE) $(DEP_CPP_UTL_STRL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\narrow\narrow.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_NARRO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\narrow.obj" : $(SOURCE) $(DEP_CPP_NARRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_NARRO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\narrow.obj" : $(SOURCE) $(DEP_CPP_NARRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_declarator.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_FE_DE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_declarator.obj" : $(SOURCE) $(DEP_CPP_FE_DE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_FE_DE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_declarator.obj" : $(SOURCE) $(DEP_CPP_FE_DE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_extern.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_FE_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_extern.obj" : $(SOURCE) $(DEP_CPP_FE_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_FE_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_extern.obj" : $(SOURCE) $(DEP_CPP_FE_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_init.cpp

!IF  "$(CFG)" == "TAO_IDL - Win32 Release"

DEP_CPP_FE_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_init.obj" : $(SOURCE) $(DEP_CPP_FE_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TAO_IDL - Win32 Debug"

DEP_CPP_FE_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/Basic_Types.h"\
	"..\..\ace/Basic_Types.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/inc_user_config.h"\
	"..\..\ace/iosfwd.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/streams.h"\
	"..\..\ace/sys_conf.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_native.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_init.obj" : $(SOURCE) $(DEP_CPP_FE_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_interface_header.cpp
DEP_CPP_FE_INT=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_interface_header.obj" : $(SOURCE) $(DEP_CPP_FE_INT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\fe_private.cpp
DEP_CPP_FE_PR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fe_private.obj" : $(SOURCE) $(DEP_CPP_FE_PR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\lex.yy.cpp
DEP_CPP_LEX_Y=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\fe\y.tab.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\lex.yy.obj" : $(SOURCE) $(DEP_CPP_LEX_Y) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fe\y.tab.cpp
DEP_CPP_Y_TAB=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_declarator.h"\
	".\include\fe_extern.h"\
	".\include\fe_interface_header.h"\
	".\include\fe_private.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\y.tab.obj" : $(SOURCE) $(DEP_CPP_Y_TAB) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_args.cpp
DEP_CPP_DRV_A=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_link.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_args.obj" : $(SOURCE) $(DEP_CPP_DRV_A) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_fork.cpp
DEP_CPP_DRV_F=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Process.h"\
	"..\..\ace/Process.i"\
	"..\..\ace/Process_Manager.h"\
	"..\..\ace/Process_Manager.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_fork.obj" : $(SOURCE) $(DEP_CPP_DRV_F) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_init.cpp
DEP_CPP_DRV_I=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_link.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_init.obj" : $(SOURCE) $(DEP_CPP_DRV_I) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_link.cpp
DEP_CPP_DRV_L=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_link.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_link.obj" : $(SOURCE) $(DEP_CPP_DRV_L) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_main.cpp
DEP_CPP_DRV_M=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_link.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_main.obj" : $(SOURCE) $(DEP_CPP_DRV_M) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
DEP_CPP_DRV_P=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Process.h"\
	"..\..\ace/Process.i"\
	"..\..\ace/Process_Manager.h"\
	"..\..\ace/Process_Manager.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_link.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_preproc.obj" : $(SOURCE) $(DEP_CPP_DRV_P) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\driver\drv_private.cpp
DEP_CPP_DRV_PR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\drv_private.obj" : $(SOURCE) $(DEP_CPP_DRV_PR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_args.cpp
DEP_CPP_BE_AR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_args.obj" : $(SOURCE) $(DEP_CPP_BE_AR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_argument.cpp
DEP_CPP_BE_ARG=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_argument.obj" : $(SOURCE) $(DEP_CPP_BE_ARG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_array.cpp
DEP_CPP_BE_ARR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_array.obj" : $(SOURCE) $(DEP_CPP_BE_ARR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_attribute.cpp
DEP_CPP_BE_AT=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_attribute.obj" : $(SOURCE) $(DEP_CPP_BE_AT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_codegen.cpp
DEP_CPP_BE_CO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_codegen.obj" : $(SOURCE) $(DEP_CPP_BE_CO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_constant.cpp
DEP_CPP_BE_CON=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_constant.obj" : $(SOURCE) $(DEP_CPP_BE_CON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_decl.cpp
DEP_CPP_BE_DE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_decl.obj" : $(SOURCE) $(DEP_CPP_BE_DE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_enum.cpp
DEP_CPP_BE_EN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_enum.obj" : $(SOURCE) $(DEP_CPP_BE_EN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_enum_val.cpp
DEP_CPP_BE_ENU=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_enum_val.obj" : $(SOURCE) $(DEP_CPP_BE_ENU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_exception.cpp
DEP_CPP_BE_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_exception.obj" : $(SOURCE) $(DEP_CPP_BE_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_expression.cpp
DEP_CPP_BE_EXP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_expression.obj" : $(SOURCE) $(DEP_CPP_BE_EXP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_factory.cpp
DEP_CPP_BE_FA=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_sunsoft.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_factory.obj" : $(SOURCE) $(DEP_CPP_BE_FA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_field.cpp
DEP_CPP_BE_FI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_field.obj" : $(SOURCE) $(DEP_CPP_BE_FI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_generator.cpp
DEP_CPP_BE_GE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_generator.obj" : $(SOURCE) $(DEP_CPP_BE_GE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_helper.cpp
DEP_CPP_BE_HE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_helper.obj" : $(SOURCE) $(DEP_CPP_BE_HE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_init.cpp
DEP_CPP_BE_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_init.obj" : $(SOURCE) $(DEP_CPP_BE_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_interface.cpp
DEP_CPP_BE_INT=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_interface.obj" : $(SOURCE) $(DEP_CPP_BE_INT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_interface_fwd.cpp
DEP_CPP_BE_INTE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_interface_fwd.obj" : $(SOURCE) $(DEP_CPP_BE_INTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_module.cpp
DEP_CPP_BE_MO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_module.obj" : $(SOURCE) $(DEP_CPP_BE_MO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_operation.cpp
DEP_CPP_BE_OP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_operation.obj" : $(SOURCE) $(DEP_CPP_BE_OP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_predefined_type.cpp
DEP_CPP_BE_PR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_predefined_type.obj" : $(SOURCE) $(DEP_CPP_BE_PR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_produce.cpp
DEP_CPP_BE_PRO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_produce.obj" : $(SOURCE) $(DEP_CPP_BE_PRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_root.cpp
DEP_CPP_BE_RO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_root.obj" : $(SOURCE) $(DEP_CPP_BE_RO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_scope.cpp
DEP_CPP_BE_SC=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_scope.obj" : $(SOURCE) $(DEP_CPP_BE_SC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_sequence.cpp
DEP_CPP_BE_SE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_sequence.obj" : $(SOURCE) $(DEP_CPP_BE_SE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state.cpp
DEP_CPP_BE_ST=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state.obj" : $(SOURCE) $(DEP_CPP_BE_ST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_string.cpp
DEP_CPP_BE_STR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_string.obj" : $(SOURCE) $(DEP_CPP_BE_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_structure.cpp
DEP_CPP_BE_STRU=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_structure.obj" : $(SOURCE) $(DEP_CPP_BE_STRU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_sunsoft.cpp
DEP_CPP_BE_SU=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_sunsoft.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_sunsoft.obj" : $(SOURCE) $(DEP_CPP_BE_SU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_type.cpp
DEP_CPP_BE_TY=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_type.obj" : $(SOURCE) $(DEP_CPP_BE_TY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_typedef.cpp
DEP_CPP_BE_TYP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_typedef.obj" : $(SOURCE) $(DEP_CPP_BE_TYP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union.cpp
DEP_CPP_BE_UN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_union.obj" : $(SOURCE) $(DEP_CPP_BE_UN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union_branch.cpp
DEP_CPP_BE_UNI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_union_branch.obj" : $(SOURCE) $(DEP_CPP_BE_UNI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_union_label.cpp
DEP_CPP_BE_UNIO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_union_label.obj" : $(SOURCE) $(DEP_CPP_BE_UNIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_argument.cpp
DEP_CPP_AST_A=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_argument.obj" : $(SOURCE) $(DEP_CPP_AST_A) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_array.cpp
DEP_CPP_AST_AR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_array.obj" : $(SOURCE) $(DEP_CPP_AST_AR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_attribute.cpp
DEP_CPP_AST_AT=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_attribute.obj" : $(SOURCE) $(DEP_CPP_AST_AT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_check.cpp
DEP_CPP_AST_C=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_check.obj" : $(SOURCE) $(DEP_CPP_AST_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_concrete_type.cpp
DEP_CPP_AST_CO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_concrete_type.obj" : $(SOURCE) $(DEP_CPP_AST_CO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_constant.cpp
DEP_CPP_AST_CON=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_constant.obj" : $(SOURCE) $(DEP_CPP_AST_CON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_decl.cpp
DEP_CPP_AST_D=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_decl.obj" : $(SOURCE) $(DEP_CPP_AST_D) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_enum.cpp
DEP_CPP_AST_E=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_enum.obj" : $(SOURCE) $(DEP_CPP_AST_E) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_enum_val.cpp
DEP_CPP_AST_EN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_enum_val.obj" : $(SOURCE) $(DEP_CPP_AST_EN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_exception.cpp
DEP_CPP_AST_EX=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_exception.obj" : $(SOURCE) $(DEP_CPP_AST_EX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_expression.cpp
DEP_CPP_AST_EXP=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_expression.obj" : $(SOURCE) $(DEP_CPP_AST_EXP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_field.cpp
DEP_CPP_AST_F=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_field.obj" : $(SOURCE) $(DEP_CPP_AST_F) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_generator.cpp
DEP_CPP_AST_G=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_generator.obj" : $(SOURCE) $(DEP_CPP_AST_G) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_interface.cpp
DEP_CPP_AST_I=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_interface.obj" : $(SOURCE) $(DEP_CPP_AST_I) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_interface_fwd.cpp
DEP_CPP_AST_IN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_interface_fwd.obj" : $(SOURCE) $(DEP_CPP_AST_IN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_module.cpp
DEP_CPP_AST_M=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_module.obj" : $(SOURCE) $(DEP_CPP_AST_M) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_operation.cpp
DEP_CPP_AST_O=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_operation.obj" : $(SOURCE) $(DEP_CPP_AST_O) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_predefined_type.cpp
DEP_CPP_AST_P=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_predefined_type.obj" : $(SOURCE) $(DEP_CPP_AST_P) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_recursive.cpp
DEP_CPP_AST_R=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_recursive.obj" : $(SOURCE) $(DEP_CPP_AST_R) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_redef.cpp
DEP_CPP_AST_RE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_redef.obj" : $(SOURCE) $(DEP_CPP_AST_RE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_root.cpp
DEP_CPP_AST_RO=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_root.obj" : $(SOURCE) $(DEP_CPP_AST_RO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_sequence.cpp
DEP_CPP_AST_S=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_sequence.obj" : $(SOURCE) $(DEP_CPP_AST_S) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_string.cpp
DEP_CPP_AST_ST=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_string.obj" : $(SOURCE) $(DEP_CPP_AST_ST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_structure.cpp
DEP_CPP_AST_STR=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_structure.obj" : $(SOURCE) $(DEP_CPP_AST_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_type.cpp
DEP_CPP_AST_T=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_type.obj" : $(SOURCE) $(DEP_CPP_AST_T) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_typedef.cpp
DEP_CPP_AST_TY=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_typedef.obj" : $(SOURCE) $(DEP_CPP_AST_TY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union.cpp
DEP_CPP_AST_U=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_union.obj" : $(SOURCE) $(DEP_CPP_AST_U) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union_branch.cpp
DEP_CPP_AST_UN=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_union_branch.obj" : $(SOURCE) $(DEP_CPP_AST_UN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ast\ast_union_label.cpp
DEP_CPP_AST_UNI=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ast_union_label.obj" : $(SOURCE) $(DEP_CPP_AST_UNI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_union.cpp
DEP_CPP_BE_STA=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_union.obj" : $(SOURCE) $(DEP_CPP_BE_STA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_array.cpp
DEP_CPP_BE_STAT=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_array.obj" : $(SOURCE) $(DEP_CPP_BE_STAT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_attribute.cpp
DEP_CPP_BE_STATE=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_attribute.obj" : $(SOURCE) $(DEP_CPP_BE_STATE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_operation.cpp
DEP_CPP_BE_STATE_=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_operation.obj" : $(SOURCE) $(DEP_CPP_BE_STATE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_sequence.cpp
DEP_CPP_BE_STATE_S=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_sequence.obj" : $(SOURCE) $(DEP_CPP_BE_STATE_S) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_structure.cpp
DEP_CPP_BE_STATE_ST=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_structure.obj" : $(SOURCE) $(DEP_CPP_BE_STATE_ST)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_typedef.cpp
DEP_CPP_BE_STATE_T=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_typedef.obj" : $(SOURCE) $(DEP_CPP_BE_STATE_T) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\be\be_state_argument.cpp
DEP_CPP_BE_STATE_A=\
	"..\..\ace/ACE.h"\
	"..\..\ace/ACE.i"\
	"..\..\ace/Atomic_Op.i"\
	"..\..\ace/Auto_Ptr.cpp"\
	"..\..\ace/Auto_Ptr.h"\
	"..\..\ace/Auto_Ptr.i"\
	"..\..\ace/config-win32-common.h"\
	"..\..\ace/config-win32.h"\
	"..\..\ace/config.h"\
	"..\..\ace/Event_Handler.h"\
	"..\..\ace/Event_Handler.i"\
	"..\..\ace/Log_Msg.h"\
	"..\..\ace/Log_Priority.h"\
	"..\..\ace/Log_Record.h"\
	"..\..\ace/Log_Record.i"\
	"..\..\ace/Managed_Object.cpp"\
	"..\..\ace/Managed_Object.h"\
	"..\..\ace/Managed_Object.i"\
	"..\..\ace/Object_Manager.h"\
	"..\..\ace/Object_Manager.i"\
	"..\..\ace/OS.h"\
	"..\..\ace/OS.i"\
	"..\..\ace/Singleton.cpp"\
	"..\..\ace/Singleton.h"\
	"..\..\ace/Singleton.i"\
	"..\..\ace/SString.h"\
	"..\..\ace/SString.i"\
	"..\..\ace/stdcpp.h"\
	"..\..\ace/SV_Semaphore_Complex.h"\
	"..\..\ace/SV_Semaphore_Complex.i"\
	"..\..\ace/SV_Semaphore_Simple.h"\
	"..\..\ace/SV_Semaphore_Simple.i"\
	"..\..\ace/Synch.h"\
	"..\..\ace/Synch.i"\
	"..\..\ace/Synch_T.cpp"\
	"..\..\ace/Synch_T.h"\
	"..\..\ace/Synch_T.i"\
	"..\..\ace/Thread.h"\
	"..\..\ace/Thread.i"\
	"..\..\ace/Trace.h"\
	"..\..\ace/Version.h"\
	"..\..\ace/ws2tcpip.h"\
	".\be_include\be.h"\
	".\be_include\be_argument.h"\
	".\be_include\be_array.h"\
	".\be_include\be_attribute.h"\
	".\be_include\be_codegen.h"\
	".\be_include\be_constant.h"\
	".\be_include\be_decl.h"\
	".\be_include\be_enum.h"\
	".\be_include\be_enum_val.h"\
	".\be_include\be_exception.h"\
	".\be_include\be_expression.h"\
	".\be_include\be_factory.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_module.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_state.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\include\ast.h"\
	".\include\ast_argument.h"\
	".\include\ast_array.h"\
	".\include\ast_attribute.h"\
	".\include\ast_concrete_type.h"\
	".\include\ast_constant.h"\
	".\include\ast_decl.h"\
	".\include\ast_enum.h"\
	".\include\ast_enum_val.h"\
	".\include\ast_exception.h"\
	".\include\ast_expression.h"\
	".\include\ast_extern.h"\
	".\include\ast_field.h"\
	".\include\ast_generator.h"\
	".\include\ast_interface.h"\
	".\include\ast_interface_fwd.h"\
	".\include\ast_module.h"\
	".\include\ast_operation.h"\
	".\include\ast_predefined_type.h"\
	".\include\ast_root.h"\
	".\include\ast_sequence.h"\
	".\include\ast_string.h"\
	".\include\ast_structure.h"\
	".\include\ast_type.h"\
	".\include\ast_typedef.h"\
	".\include\ast_union.h"\
	".\include\ast_union_branch.h"\
	".\include\ast_union_label.h"\
	".\include\be_extern.h"\
	".\include\drv_extern.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_global.h"\
	".\include\idl_narrow.h"\
	".\include\intlmacros.h"\
	".\include\nr_extern.h"\
	".\include\util.h"\
	".\include\utl_decllist.h"\
	".\include\utl_error.h"\
	".\include\utl_exceptlist.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_idlist.h"\
	".\include\utl_indenter.h"\
	".\include\utl_labellist.h"\
	".\include\utl_list.h"\
	".\include\utl_namelist.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_stack.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\be_state_argument.obj" : $(SOURCE) $(DEP_CPP_BE_STATE_A) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
