# Microsoft Developer Studio Project File - Name="TAO_IDL_FE_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO_IDL_FE_LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_FE_LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_FE_LIB.mak" CFG="TAO_IDL_FE_LIB - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL_FE_LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_IDL_FE_LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL_FE_LIB - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LIB\Release"
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "../../" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_IDL_FE_LIBs.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\Release\tao_idl_static.exe del ..\..\bin\Release\tao_idl_static.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TAO_IDL_FE_LIB - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LIB\Debug"
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "../../" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_IDL_FE_LIBsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\tao_idl_static.exe del ..\..\bin\tao_idl_static.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TAO_IDL_FE_LIB - Win32 Static Release"
# Name "TAO_IDL_FE_LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\ast\ast_component.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_component_fwd.cpp
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

SOURCE=.\ast\ast_eventtype.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_eventtype_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_exception.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_expression.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_field.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_generator.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_home.cpp
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

SOURCE=.\ast\ast_native.cpp
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

SOURCE=.\ast\ast_structure_fwd.cpp
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

SOURCE=.\ast\ast_union_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_union_label.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_valuetype.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_valuetype_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\ast\ast_visitor.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_declarator.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_extern.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_global.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_init.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_interface_header.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_lookup.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_private.cpp
# End Source File
# Begin Source File

SOURCE=.\fe\fe_tmplinst.cpp
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

SOURCE=.\util\utl_err.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\include\ast_component.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_component_fwd.h
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

SOURCE=.\include\ast_eventtype.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_eventtype_fwd.h
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

SOURCE=.\include\ast_factory.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_field.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_generator.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_home.h
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

SOURCE=.\include\ast_structure_fwd.h
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

SOURCE=.\include\ast_union_fwd.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_union_label.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_valuetype.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_valuetype_fwd.h
# End Source File
# Begin Source File

SOURCE=.\include\ast_visitor.h
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

SOURCE=.\include\idl_bool.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_defines.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_global.h
# End Source File
# Begin Source File

SOURCE=.\include\idl_narrow.h
# End Source File
# Begin Source File

SOURCE=.\include\nr_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\TAO_IDL_FE_Export.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_decllist.h
# End Source File
# Begin Source File

SOURCE=.\include\utl_err.h
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
# End Group
# End Target
# End Project
