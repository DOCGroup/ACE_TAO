#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libutil
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libtao_idl_util
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of

UTIL_FILES	= \
		utl_scope \
		utl_stack \
		utl_string \
		utl_decllist \
		utl_labellist \
		utl_namelist \
		utl_exceptlist \
		utl_strlist \
		utl_list \
		utl_exprlist \
		utl_error \
		utl_indenter \
		utl_identifier \
		utl_idlist \
		utl_global

FILES	 = $(UTIL_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS	= -lACE
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) #$(VLIB)


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

CPPFLAGS += -I../include -I. -DCPP_LOCATION=\"$(CXX)\" -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

$(LOBJS): $(LHDRS)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/utl_scope.o .shobj/utl_scope.so: utl_scope.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_stack.o .shobj/utl_stack.so: utl_stack.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_string.o .shobj/utl_string.so: utl_string.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_decllist.o .shobj/utl_decllist.so: utl_decllist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_labellist.o .shobj/utl_labellist.so: utl_labellist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_namelist.o .shobj/utl_namelist.so: utl_namelist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_exceptlist.o .shobj/utl_exceptlist.so: utl_exceptlist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_strlist.o .shobj/utl_strlist.so: utl_strlist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_list.o .shobj/utl_list.so: utl_list.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_exprlist.o .shobj/utl_exprlist.so: utl_exprlist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_error.o .shobj/utl_error.so: utl_error.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_indenter.o .shobj/utl_indenter.so: utl_indenter.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_identifier.o .shobj/utl_identifier.so: utl_identifier.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_idlist.o .shobj/utl_idlist.so: utl_idlist.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h
.obj/utl_global.o .shobj/utl_global.so: utl_global.cpp ../include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
