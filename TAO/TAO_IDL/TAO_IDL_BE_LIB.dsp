# Microsoft Developer Studio Project File - Name="TAO_IDL_BE_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO_IDL_BE_LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_BE_LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_BE_LIB.mak" CFG="TAO_IDL_BE_LIB - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL_BE_LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_IDL_BE_LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL_BE_LIB - Win32 Static Release"

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
# ADD LIB32 /nologo /out:"TAO_IDL_BE_LIBs.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\Release\tao_idl_static.exe del ..\..\bin\Release\tao_idl_static.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TAO_IDL_BE_LIB - Win32 Static Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "../../" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_IDL_BE_LIBsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\tao_idl_static.exe del ..\..\bin\tao_idl_static.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TAO_IDL_BE_LIB - Win32 Static Release"
# Name "TAO_IDL_BE_LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\be\be_component.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_component_fwd.cpp
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

SOURCE=.\be\be_global.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_home.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_interface_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_interface_strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_module.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_native.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_operation.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_operation_strategy.cpp
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

SOURCE=.\be\be_stream_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_string.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_structure.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_structure_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_sunsoft.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_tmplinst.cpp
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

SOURCE=.\be\be_union_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_union_label.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_valuetype.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_valuetype_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_amh_pre_proc.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_ami_pre_proc.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_argument.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_array.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_component.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_component_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_constant.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_context.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_decl.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_enum.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_exception.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_field.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_home.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_interface_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_module.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_operation.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_root.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_scope.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_structure.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_structure_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_union.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_union_branch.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_union_fwd.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_valuetype.cpp
# End Source File
# Begin Source File

SOURCE=.\be\be_visitor_valuetype_fwd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\be_include\be_component.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_component_fwd.h
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

SOURCE=.\be_include\be_extern.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_factory.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_field.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_generator.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_global.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_helper.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_home.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_interface.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_interface_fwd.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_interface_strategy.h
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

SOURCE=.\be_include\be_operation_strategy.h
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

SOURCE=.\be_include\be_stream_factory.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_string.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_structure.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_structure_fwd.h
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

SOURCE=.\be_include\be_union_fwd.h
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

SOURCE=.\be_include\be_visitor_ami_pre_proc.h
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

SOURCE=.\be_include\be_visitor_component.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_component_fwd.h
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

SOURCE=.\be_include\be_visitor_home.h
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

SOURCE=.\be_include\be_visitor_structure_fwd.h
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

SOURCE=.\be_include\be_visitor_union_fwd.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_valuetype.h
# End Source File
# Begin Source File

SOURCE=.\be_include\be_visitor_valuetype_fwd.h
# End Source File
# End Group
# End Target
# End Project
