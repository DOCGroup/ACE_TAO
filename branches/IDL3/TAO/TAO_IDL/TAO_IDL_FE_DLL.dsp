# Microsoft Developer Studio Project File - Name="TAO_IDL_FE_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_IDL_FE_DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_FE_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IDL_FE_DLL.mak" CFG="TAO_IDL_FE_DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL_FE_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_IDL_FE_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL_FE_DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IDL_FE_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "../../" /D TAO_IDL_FE_HAS_DLL=1 /D "TAO_IDL_FE_BUILD_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IDL_FE_DLL_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib /nologo /dll /debug /machine:I386 /out:"..\..\bin\TAO_IDL_FE_DLL.dll" /libpath:"..\..\ace"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\Release\tao_idl.exe del ..\..\bin\Release\tao_idl.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TAO_IDL_FE_DLL - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IDL_FE_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "../../" /D TAO_IDL_FE_HAS_DLL=1 /D "TAO_IDL_FE_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IDL_FE_DLL_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\bin\TAO_IDL_FE_DLLd.dll" /pdbtype:sept /libpath:"..\..\ace"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=if exist ..\..\bin\tao_idl.exe del ..\..\bin\tao_idl.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TAO_IDL_FE_DLL - Win32 Release"
# Name "TAO_IDL_FE_DLL - Win32 Debug"
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

SOURCE=.\ast\ast_factory.cpp
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

SOURCE=.\include\ast_component.h
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

SOURCE=.\include\ast_factory.h
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

SOURCE=.\include\nr_extern.h
# End Source File
# Begin Source File

SOURCE=.\TAO_IDL_FE_Export.h
# End Source File
# Begin Source File

SOURCE=.\include\util.h
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
