# Microsoft Developer Studio Project File - Name="TAO_IDL Compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TAO_IDL Compiler - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak" CFG="TAO_IDL Compiler - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL Compiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL Compiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /MD /I "include" /I "be_include" /I "../../" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
BSC32=bscmake.exe
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAO_IDL_BE_DLL.lib TAO_IDL_FE_DLL.lib ace.lib /nologo /subsystem:console /debug /machine:IX86 /out:"..\..\bin\Release\tao_idl.exe" /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "../../" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_IDL_BE_DLLd.lib TAO_IDL_FE_DLLd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\tao_idl.exe" /pdbtype:sept /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "TAO_IDL Compiler - Win32 Release"
# Name "TAO_IDL Compiler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\driver\drv_args.cpp
DEP_CPP_DRV_A=\
	"..\..\ace\ace.h"\
	"..\..\ace\ace.i"\
	"..\..\ace\ace_export.h"\
	"..\..\ace\ace_wchar.h"\
	"..\..\ace\atomic_op.i"\
	"..\..\ace\based_pointer_repository.h"\
	"..\..\ace\based_pointer_t.cpp"\
	"..\..\ace\based_pointer_t.h"\
	"..\..\ace\based_pointer_t.i"\
	"..\..\ace\basic_types.h"\
	"..\..\ace\basic_types.i"\
	"..\..\ace\cdr_stream.h"\
	"..\..\ace\cdr_stream.i"\
	"..\..\ace\config-all.h"\
	"..\..\ace\config-win32-borland.h"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32-msvc.h"\
	"..\..\ace\config-win32-visualage.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\containers.h"\
	"..\..\ace\containers.i"\
	"..\..\ace\containers_t.cpp"\
	"..\..\ace\containers_t.h"\
	"..\..\ace\containers_t.i"\
	"..\..\ace\event_handler.h"\
	"..\..\ace\event_handler.i"\
	"..\..\ace\free_list.cpp"\
	"..\..\ace\free_list.h"\
	"..\..\ace\free_list.i"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\log_msg.h"\
	"..\..\ace\log_priority.h"\
	"..\..\ace\log_record.h"\
	"..\..\ace\log_record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\malloc.i"\
	"..\..\ace\malloc_base.h"\
	"..\..\ace\malloc_t.cpp"\
	"..\..\ace\malloc_t.h"\
	"..\..\ace\malloc_t.i"\
	"..\..\ace\managed_object.cpp"\
	"..\..\ace\managed_object.h"\
	"..\..\ace\managed_object.i"\
	"..\..\ace\mem_map.h"\
	"..\..\ace\mem_map.i"\
	"..\..\ace\memory_pool.h"\
	"..\..\ace\memory_pool.i"\
	"..\..\ace\message_block.h"\
	"..\..\ace\message_block.i"\
	"..\..\ace\message_block_t.cpp"\
	"..\..\ace\message_block_t.h"\
	"..\..\ace\message_block_t.i"\
	"..\..\ace\min_max.h"\
	"..\..\ace\object_manager.h"\
	"..\..\ace\object_manager.i"\
	"..\..\ace\os.h"\
	"..\..\ace\os.i"\
	"..\..\ace\os_dirent.h"\
	"..\..\ace\os_dirent.inl"\
	"..\..\ace\os_export.h"\
	"..\..\ace\os_memory.h"\
	"..\..\ace\os_memory.inl"\
	"..\..\ace\os_string.h"\
	"..\..\ace\os_string.inl"\
	"..\..\ace\os_tli.h"\
	"..\..\ace\os_tli.inl"\
	"..\..\ace\post.h"\
	"..\..\ace\pre.h"\
	"..\..\ace\Process.h"\
	"..\..\ace\process.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\signal.i"\
	"..\..\ace\singleton.cpp"\
	"..\..\ace\singleton.h"\
	"..\..\ace\singleton.i"\
	"..\..\ace\sstring.h"\
	"..\..\ace\sstring.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\svc_export.h"\
	"..\..\ace\synch.h"\
	"..\..\ace\synch.i"\
	"..\..\ace\synch_t.cpp"\
	"..\..\ace\synch_t.h"\
	"..\..\ace\synch_t.i"\
	"..\..\ace\thread.h"\
	"..\..\ace\thread.i"\
	"..\..\ace\Thread_Adapter.h"\
	"..\..\ace\Thread_Adapter.inl"\
	"..\..\ace\Thread_Control.h"\
	"..\..\ace\Thread_Control.inl"\
	"..\..\ace\Thread_Exit.h"\
	"..\..\ace\Thread_Hook.h"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
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
	".\be_include\be_extern.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_global.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_interface_strategy.h"\
	".\be_include\be_module.h"\
	".\be_include\be_native.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_operation_strategy.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_stream_factory.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\be_include\be_valuetype.h"\
	".\be_include\be_valuetype_fwd.h"\
	".\be_include\be_visitor.h"\
	".\be_include\be_visitor_ami_pre_proc.h"\
	".\be_include\be_visitor_context.h"\
	".\be_include\be_visitor_decl.h"\
	".\be_include\be_visitor_factory.h"\
	".\be_include\be_visitor_scope.h"\
	".\be_include\tao_idl_be_export.h"\
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
	".\include\drv_extern.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_narrow.h"\
	".\include\nr_extern.h"\
	".\include\tao_idl_fe_export.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_list.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	
NODEP_CPP_DRV_A=\
	"..\..\ace\pace\ctype.h"\
	"..\..\ace\pace\dirent.h"\
	"..\..\ace\pace\fcntl.h"\
	"..\..\ace\pace\pthread.h"\
	"..\..\ace\pace\pwd.h"\
	"..\..\ace\pace\sched.h"\
	"..\..\ace\pace\semaphore.h"\
	"..\..\ace\pace\signal.h"\
	"..\..\ace\pace\stddef.h"\
	"..\..\ace\pace\stdio.h"\
	"..\..\ace\pace\stdlib.h"\
	"..\..\ace\pace\string.h"\
	"..\..\ace\pace\sys\mman.h"\
	"..\..\ace\pace\sys\stat.h"\
	"..\..\ace\pace\sys\utsname.h"\
	"..\..\ace\pace\sys\wait.h"\
	"..\..\ace\pace\time.h"\
	"..\..\ace\pace\unistd.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_init.cpp
DEP_CPP_DRV_I=\
	"..\..\ace\ace.h"\
	"..\..\ace\ace.i"\
	"..\..\ace\ace_export.h"\
	"..\..\ace\ace_wchar.h"\
	"..\..\ace\atomic_op.i"\
	"..\..\ace\based_pointer_repository.h"\
	"..\..\ace\based_pointer_t.cpp"\
	"..\..\ace\based_pointer_t.h"\
	"..\..\ace\based_pointer_t.i"\
	"..\..\ace\basic_types.h"\
	"..\..\ace\basic_types.i"\
	"..\..\ace\cdr_stream.h"\
	"..\..\ace\cdr_stream.i"\
	"..\..\ace\config-all.h"\
	"..\..\ace\config-win32-borland.h"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32-msvc.h"\
	"..\..\ace\config-win32-visualage.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\containers.h"\
	"..\..\ace\containers.i"\
	"..\..\ace\containers_t.cpp"\
	"..\..\ace\containers_t.h"\
	"..\..\ace\containers_t.i"\
	"..\..\ace\event_handler.h"\
	"..\..\ace\event_handler.i"\
	"..\..\ace\free_list.cpp"\
	"..\..\ace\free_list.h"\
	"..\..\ace\free_list.i"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\log_msg.h"\
	"..\..\ace\log_priority.h"\
	"..\..\ace\log_record.h"\
	"..\..\ace\log_record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\malloc.i"\
	"..\..\ace\malloc_base.h"\
	"..\..\ace\malloc_t.cpp"\
	"..\..\ace\malloc_t.h"\
	"..\..\ace\malloc_t.i"\
	"..\..\ace\managed_object.cpp"\
	"..\..\ace\managed_object.h"\
	"..\..\ace\managed_object.i"\
	"..\..\ace\mem_map.h"\
	"..\..\ace\mem_map.i"\
	"..\..\ace\memory_pool.h"\
	"..\..\ace\memory_pool.i"\
	"..\..\ace\message_block.h"\
	"..\..\ace\message_block.i"\
	"..\..\ace\message_block_t.cpp"\
	"..\..\ace\message_block_t.h"\
	"..\..\ace\message_block_t.i"\
	"..\..\ace\min_max.h"\
	"..\..\ace\object_manager.h"\
	"..\..\ace\object_manager.i"\
	"..\..\ace\os.h"\
	"..\..\ace\os.i"\
	"..\..\ace\os_dirent.h"\
	"..\..\ace\os_dirent.inl"\
	"..\..\ace\os_export.h"\
	"..\..\ace\os_memory.h"\
	"..\..\ace\os_memory.inl"\
	"..\..\ace\os_string.h"\
	"..\..\ace\os_string.inl"\
	"..\..\ace\os_tli.h"\
	"..\..\ace\os_tli.inl"\
	"..\..\ace\post.h"\
	"..\..\ace\pre.h"\
	"..\..\ace\Process.h"\
	"..\..\ace\process.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\signal.i"\
	"..\..\ace\singleton.cpp"\
	"..\..\ace\singleton.h"\
	"..\..\ace\singleton.i"\
	"..\..\ace\sstring.h"\
	"..\..\ace\sstring.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\svc_export.h"\
	"..\..\ace\synch.h"\
	"..\..\ace\synch.i"\
	"..\..\ace\synch_t.cpp"\
	"..\..\ace\synch_t.h"\
	"..\..\ace\synch_t.i"\
	"..\..\ace\thread.h"\
	"..\..\ace\thread.i"\
	"..\..\ace\Thread_Adapter.h"\
	"..\..\ace\Thread_Adapter.inl"\
	"..\..\ace\Thread_Control.h"\
	"..\..\ace\Thread_Control.inl"\
	"..\..\ace\Thread_Exit.h"\
	"..\..\ace\Thread_Hook.h"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
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
	".\be_include\be_extern.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_global.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_interface_strategy.h"\
	".\be_include\be_module.h"\
	".\be_include\be_native.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_operation_strategy.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_stream_factory.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\be_include\be_valuetype.h"\
	".\be_include\be_valuetype_fwd.h"\
	".\be_include\be_visitor.h"\
	".\be_include\be_visitor_ami_pre_proc.h"\
	".\be_include\be_visitor_context.h"\
	".\be_include\be_visitor_decl.h"\
	".\be_include\be_visitor_factory.h"\
	".\be_include\be_visitor_scope.h"\
	".\be_include\tao_idl_be_export.h"\
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
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_narrow.h"\
	".\include\nr_extern.h"\
	".\include\tao_idl_fe_export.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_list.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	
NODEP_CPP_DRV_I=\
	"..\..\ace\pace\ctype.h"\
	"..\..\ace\pace\dirent.h"\
	"..\..\ace\pace\fcntl.h"\
	"..\..\ace\pace\pthread.h"\
	"..\..\ace\pace\pwd.h"\
	"..\..\ace\pace\sched.h"\
	"..\..\ace\pace\semaphore.h"\
	"..\..\ace\pace\signal.h"\
	"..\..\ace\pace\stddef.h"\
	"..\..\ace\pace\stdio.h"\
	"..\..\ace\pace\stdlib.h"\
	"..\..\ace\pace\string.h"\
	"..\..\ace\pace\sys\mman.h"\
	"..\..\ace\pace\sys\stat.h"\
	"..\..\ace\pace\sys\utsname.h"\
	"..\..\ace\pace\sys\wait.h"\
	"..\..\ace\pace\time.h"\
	"..\..\ace\pace\unistd.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
DEP_CPP_DRV_P=\
	"..\..\ace\ace.h"\
	"..\..\ace\ace.i"\
	"..\..\ace\ace_export.h"\
	"..\..\ace\ace_wchar.h"\
	"..\..\ace\ARGV.h"\
	"..\..\ace\ARGV.i"\
	"..\..\ace\atomic_op.i"\
	"..\..\ace\based_pointer_repository.h"\
	"..\..\ace\based_pointer_t.cpp"\
	"..\..\ace\based_pointer_t.h"\
	"..\..\ace\based_pointer_t.i"\
	"..\..\ace\basic_types.h"\
	"..\..\ace\basic_types.i"\
	"..\..\ace\cdr_stream.h"\
	"..\..\ace\cdr_stream.i"\
	"..\..\ace\config-all.h"\
	"..\..\ace\config-win32-borland.h"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32-msvc.h"\
	"..\..\ace\config-win32-visualage.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\containers.h"\
	"..\..\ace\containers.i"\
	"..\..\ace\containers_t.cpp"\
	"..\..\ace\containers_t.h"\
	"..\..\ace\containers_t.i"\
	"..\..\ace\Env_Value_T.cpp"\
	"..\..\ace\Env_Value_T.h"\
	"..\..\ace\Env_Value_T.i"\
	"..\..\ace\event_handler.h"\
	"..\..\ace\event_handler.i"\
	"..\..\ace\free_list.cpp"\
	"..\..\ace\free_list.h"\
	"..\..\ace\free_list.i"\
	"..\..\ace\Handle_Set.h"\
	"..\..\ace\Handle_Set.i"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\log_msg.h"\
	"..\..\ace\log_priority.h"\
	"..\..\ace\log_record.h"\
	"..\..\ace\log_record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\malloc.i"\
	"..\..\ace\malloc_base.h"\
	"..\..\ace\malloc_t.cpp"\
	"..\..\ace\malloc_t.h"\
	"..\..\ace\malloc_t.i"\
	"..\..\ace\managed_object.cpp"\
	"..\..\ace\managed_object.h"\
	"..\..\ace\managed_object.i"\
	"..\..\ace\mem_map.h"\
	"..\..\ace\mem_map.i"\
	"..\..\ace\memory_pool.h"\
	"..\..\ace\memory_pool.i"\
	"..\..\ace\message_block.h"\
	"..\..\ace\message_block.i"\
	"..\..\ace\message_block_t.cpp"\
	"..\..\ace\message_block_t.h"\
	"..\..\ace\message_block_t.i"\
	"..\..\ace\min_max.h"\
	"..\..\ace\object_manager.h"\
	"..\..\ace\object_manager.i"\
	"..\..\ace\os.h"\
	"..\..\ace\os.i"\
	"..\..\ace\os_dirent.h"\
	"..\..\ace\os_dirent.inl"\
	"..\..\ace\os_export.h"\
	"..\..\ace\os_memory.h"\
	"..\..\ace\os_memory.inl"\
	"..\..\ace\os_string.h"\
	"..\..\ace\os_string.inl"\
	"..\..\ace\os_tli.h"\
	"..\..\ace\os_tli.inl"\
	"..\..\ace\post.h"\
	"..\..\ace\pre.h"\
	"..\..\ace\Process.h"\
	"..\..\ace\process.i"\
	"..\..\ace\Process_Manager.h"\
	"..\..\ace\Process_Manager.i"\
	"..\..\ace\Reactor.h"\
	"..\..\ace\Reactor.i"\
	"..\..\ace\Reactor_Impl.h"\
	"..\..\ace\Signal.h"\
	"..\..\ace\signal.i"\
	"..\..\ace\singleton.cpp"\
	"..\..\ace\singleton.h"\
	"..\..\ace\singleton.i"\
	"..\..\ace\sstring.h"\
	"..\..\ace\sstring.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\svc_export.h"\
	"..\..\ace\synch.h"\
	"..\..\ace\synch.i"\
	"..\..\ace\synch_t.cpp"\
	"..\..\ace\synch_t.h"\
	"..\..\ace\synch_t.i"\
	"..\..\ace\thread.h"\
	"..\..\ace\thread.i"\
	"..\..\ace\Thread_Adapter.h"\
	"..\..\ace\Thread_Adapter.inl"\
	"..\..\ace\Thread_Control.h"\
	"..\..\ace\Thread_Control.inl"\
	"..\..\ace\Thread_Exit.h"\
	"..\..\ace\Thread_Hook.h"\
	"..\..\ace\Timer_Queue.h"\
	"..\..\ace\Timer_Queue_T.cpp"\
	"..\..\ace\Timer_Queue_T.h"\
	"..\..\ace\Timer_Queue_T.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
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
	".\include\drv_extern.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_narrow.h"\
	".\include\nr_extern.h"\
	".\include\tao_idl_fe_export.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_list.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	
NODEP_CPP_DRV_P=\
	"..\..\ace\pace\ctype.h"\
	"..\..\ace\pace\dirent.h"\
	"..\..\ace\pace\fcntl.h"\
	"..\..\ace\pace\pthread.h"\
	"..\..\ace\pace\pwd.h"\
	"..\..\ace\pace\sched.h"\
	"..\..\ace\pace\semaphore.h"\
	"..\..\ace\pace\signal.h"\
	"..\..\ace\pace\stddef.h"\
	"..\..\ace\pace\stdio.h"\
	"..\..\ace\pace\stdlib.h"\
	"..\..\ace\pace\string.h"\
	"..\..\ace\pace\sys\mman.h"\
	"..\..\ace\pace\sys\stat.h"\
	"..\..\ace\pace\sys\utsname.h"\
	"..\..\ace\pace\sys\wait.h"\
	"..\..\ace\pace\time.h"\
	"..\..\ace\pace\unistd.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_private.cpp
DEP_CPP_DRV_PR=\
	"..\..\ace\ace.h"\
	"..\..\ace\ace.i"\
	"..\..\ace\ace_export.h"\
	"..\..\ace\ace_wchar.h"\
	"..\..\ace\atomic_op.i"\
	"..\..\ace\based_pointer_repository.h"\
	"..\..\ace\based_pointer_t.cpp"\
	"..\..\ace\based_pointer_t.h"\
	"..\..\ace\based_pointer_t.i"\
	"..\..\ace\basic_types.h"\
	"..\..\ace\basic_types.i"\
	"..\..\ace\cdr_stream.h"\
	"..\..\ace\cdr_stream.i"\
	"..\..\ace\config-all.h"\
	"..\..\ace\config-win32-borland.h"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32-msvc.h"\
	"..\..\ace\config-win32-visualage.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\containers.h"\
	"..\..\ace\containers.i"\
	"..\..\ace\containers_t.cpp"\
	"..\..\ace\containers_t.h"\
	"..\..\ace\containers_t.i"\
	"..\..\ace\event_handler.h"\
	"..\..\ace\event_handler.i"\
	"..\..\ace\free_list.cpp"\
	"..\..\ace\free_list.h"\
	"..\..\ace\free_list.i"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\log_msg.h"\
	"..\..\ace\log_priority.h"\
	"..\..\ace\log_record.h"\
	"..\..\ace\log_record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\malloc.i"\
	"..\..\ace\malloc_base.h"\
	"..\..\ace\malloc_t.cpp"\
	"..\..\ace\malloc_t.h"\
	"..\..\ace\malloc_t.i"\
	"..\..\ace\managed_object.cpp"\
	"..\..\ace\managed_object.h"\
	"..\..\ace\managed_object.i"\
	"..\..\ace\mem_map.h"\
	"..\..\ace\mem_map.i"\
	"..\..\ace\memory_pool.h"\
	"..\..\ace\memory_pool.i"\
	"..\..\ace\message_block.h"\
	"..\..\ace\message_block.i"\
	"..\..\ace\message_block_t.cpp"\
	"..\..\ace\message_block_t.h"\
	"..\..\ace\message_block_t.i"\
	"..\..\ace\min_max.h"\
	"..\..\ace\object_manager.h"\
	"..\..\ace\object_manager.i"\
	"..\..\ace\os.h"\
	"..\..\ace\os.i"\
	"..\..\ace\os_dirent.h"\
	"..\..\ace\os_dirent.inl"\
	"..\..\ace\os_export.h"\
	"..\..\ace\os_memory.h"\
	"..\..\ace\os_memory.inl"\
	"..\..\ace\os_string.h"\
	"..\..\ace\os_string.inl"\
	"..\..\ace\os_tli.h"\
	"..\..\ace\os_tli.inl"\
	"..\..\ace\post.h"\
	"..\..\ace\pre.h"\
	"..\..\ace\Signal.h"\
	"..\..\ace\signal.i"\
	"..\..\ace\singleton.cpp"\
	"..\..\ace\singleton.h"\
	"..\..\ace\singleton.i"\
	"..\..\ace\sstring.h"\
	"..\..\ace\sstring.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\svc_export.h"\
	"..\..\ace\synch.h"\
	"..\..\ace\synch.i"\
	"..\..\ace\synch_t.cpp"\
	"..\..\ace\synch_t.h"\
	"..\..\ace\synch_t.i"\
	"..\..\ace\thread.h"\
	"..\..\ace\thread.i"\
	"..\..\ace\Thread_Adapter.h"\
	"..\..\ace\Thread_Adapter.inl"\
	"..\..\ace\Thread_Control.h"\
	"..\..\ace\Thread_Control.inl"\
	"..\..\ace\Thread_Exit.h"\
	"..\..\ace\Thread_Hook.h"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
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
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_narrow.h"\
	".\include\nr_extern.h"\
	".\include\tao_idl_fe_export.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_list.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	
NODEP_CPP_DRV_PR=\
	"..\..\ace\pace\ctype.h"\
	"..\..\ace\pace\dirent.h"\
	"..\..\ace\pace\fcntl.h"\
	"..\..\ace\pace\pthread.h"\
	"..\..\ace\pace\pwd.h"\
	"..\..\ace\pace\sched.h"\
	"..\..\ace\pace\semaphore.h"\
	"..\..\ace\pace\signal.h"\
	"..\..\ace\pace\stddef.h"\
	"..\..\ace\pace\stdio.h"\
	"..\..\ace\pace\stdlib.h"\
	"..\..\ace\pace\string.h"\
	"..\..\ace\pace\sys\mman.h"\
	"..\..\ace\pace\sys\stat.h"\
	"..\..\ace\pace\sys\utsname.h"\
	"..\..\ace\pace\sys\wait.h"\
	"..\..\ace\pace\time.h"\
	"..\..\ace\pace\unistd.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tao_idl.cpp
DEP_CPP_TAO_I=\
	"..\..\ace\ace.h"\
	"..\..\ace\ace.i"\
	"..\..\ace\ace_export.h"\
	"..\..\ace\ace_wchar.h"\
	"..\..\ace\atomic_op.i"\
	"..\..\ace\based_pointer_repository.h"\
	"..\..\ace\based_pointer_t.cpp"\
	"..\..\ace\based_pointer_t.h"\
	"..\..\ace\based_pointer_t.i"\
	"..\..\ace\basic_types.h"\
	"..\..\ace\basic_types.i"\
	"..\..\ace\cdr_stream.h"\
	"..\..\ace\cdr_stream.i"\
	"..\..\ace\config-all.h"\
	"..\..\ace\config-win32-borland.h"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32-msvc.h"\
	"..\..\ace\config-win32-visualage.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\containers.h"\
	"..\..\ace\containers.i"\
	"..\..\ace\containers_t.cpp"\
	"..\..\ace\containers_t.h"\
	"..\..\ace\containers_t.i"\
	"..\..\ace\event_handler.h"\
	"..\..\ace\event_handler.i"\
	"..\..\ace\free_list.cpp"\
	"..\..\ace\free_list.h"\
	"..\..\ace\free_list.i"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\log_msg.h"\
	"..\..\ace\log_priority.h"\
	"..\..\ace\log_record.h"\
	"..\..\ace\log_record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\malloc.i"\
	"..\..\ace\malloc_base.h"\
	"..\..\ace\malloc_t.cpp"\
	"..\..\ace\malloc_t.h"\
	"..\..\ace\malloc_t.i"\
	"..\..\ace\managed_object.cpp"\
	"..\..\ace\managed_object.h"\
	"..\..\ace\managed_object.i"\
	"..\..\ace\mem_map.h"\
	"..\..\ace\mem_map.i"\
	"..\..\ace\memory_pool.h"\
	"..\..\ace\memory_pool.i"\
	"..\..\ace\message_block.h"\
	"..\..\ace\message_block.i"\
	"..\..\ace\message_block_t.cpp"\
	"..\..\ace\message_block_t.h"\
	"..\..\ace\message_block_t.i"\
	"..\..\ace\min_max.h"\
	"..\..\ace\object_manager.h"\
	"..\..\ace\object_manager.i"\
	"..\..\ace\os.h"\
	"..\..\ace\os.i"\
	"..\..\ace\os_dirent.h"\
	"..\..\ace\os_dirent.inl"\
	"..\..\ace\os_export.h"\
	"..\..\ace\os_memory.h"\
	"..\..\ace\os_memory.inl"\
	"..\..\ace\os_string.h"\
	"..\..\ace\os_string.inl"\
	"..\..\ace\os_tli.h"\
	"..\..\ace\os_tli.inl"\
	"..\..\ace\post.h"\
	"..\..\ace\pre.h"\
	"..\..\ace\Process.h"\
	"..\..\ace\process.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\signal.i"\
	"..\..\ace\singleton.cpp"\
	"..\..\ace\singleton.h"\
	"..\..\ace\singleton.i"\
	"..\..\ace\sstring.h"\
	"..\..\ace\sstring.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\svc_export.h"\
	"..\..\ace\synch.h"\
	"..\..\ace\synch.i"\
	"..\..\ace\synch_t.cpp"\
	"..\..\ace\synch_t.h"\
	"..\..\ace\synch_t.i"\
	"..\..\ace\thread.h"\
	"..\..\ace\thread.i"\
	"..\..\ace\Thread_Adapter.h"\
	"..\..\ace\Thread_Adapter.inl"\
	"..\..\ace\Thread_Control.h"\
	"..\..\ace\Thread_Control.inl"\
	"..\..\ace\Thread_Exit.h"\
	"..\..\ace\Thread_Hook.h"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\tao\Version.h"\
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
	".\be_include\be_extern.h"\
	".\be_include\be_field.h"\
	".\be_include\be_generator.h"\
	".\be_include\be_global.h"\
	".\be_include\be_helper.h"\
	".\be_include\be_interface.h"\
	".\be_include\be_interface_fwd.h"\
	".\be_include\be_interface_strategy.h"\
	".\be_include\be_module.h"\
	".\be_include\be_native.h"\
	".\be_include\be_operation.h"\
	".\be_include\be_operation_strategy.h"\
	".\be_include\be_predefined_type.h"\
	".\be_include\be_root.h"\
	".\be_include\be_scope.h"\
	".\be_include\be_sequence.h"\
	".\be_include\be_stream_factory.h"\
	".\be_include\be_string.h"\
	".\be_include\be_structure.h"\
	".\be_include\be_type.h"\
	".\be_include\be_typedef.h"\
	".\be_include\be_union.h"\
	".\be_include\be_union_branch.h"\
	".\be_include\be_union_label.h"\
	".\be_include\be_valuetype.h"\
	".\be_include\be_valuetype_fwd.h"\
	".\be_include\be_visitor.h"\
	".\be_include\be_visitor_ami_pre_proc.h"\
	".\be_include\be_visitor_context.h"\
	".\be_include\be_visitor_decl.h"\
	".\be_include\be_visitor_factory.h"\
	".\be_include\be_visitor_scope.h"\
	".\be_include\tao_idl_be_export.h"\
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
	".\include\drv_extern.h"\
	".\include\drv_private.h"\
	".\include\fe_extern.h"\
	".\include\global_extern.h"\
	".\include\idl.h"\
	".\include\idl_bool.h"\
	".\include\idl_defines.h"\
	".\include\idl_extern.h"\
	".\include\idl_fwd.h"\
	".\include\idl_narrow.h"\
	".\include\nr_extern.h"\
	".\include\tao_idl_fe_export.h"\
	".\include\utl_exprlist.h"\
	".\include\utl_identifier.h"\
	".\include\utl_list.h"\
	".\include\utl_scope.h"\
	".\include\utl_scoped_name.h"\
	".\include\utl_string.h"\
	".\include\utl_strlist.h"\
	
NODEP_CPP_TAO_I=\
	"..\..\ace\pace\ctype.h"\
	"..\..\ace\pace\dirent.h"\
	"..\..\ace\pace\fcntl.h"\
	"..\..\ace\pace\pthread.h"\
	"..\..\ace\pace\pwd.h"\
	"..\..\ace\pace\sched.h"\
	"..\..\ace\pace\semaphore.h"\
	"..\..\ace\pace\signal.h"\
	"..\..\ace\pace\stddef.h"\
	"..\..\ace\pace\stdio.h"\
	"..\..\ace\pace\stdlib.h"\
	"..\..\ace\pace\string.h"\
	"..\..\ace\pace\sys\mman.h"\
	"..\..\ace\pace\sys\stat.h"\
	"..\..\ace\pace\sys\utsname.h"\
	"..\..\ace\pace\sys\wait.h"\
	"..\..\ace\pace\time.h"\
	"..\..\ace\pace\unistd.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\drv_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\drv_private.h
# End Source File
# End Group
# End Target
# End Project
