#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libast
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libast
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).so
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of
AST_FILES	= \
		ast_array \
		ast_argument \
		ast_attribute \
		ast_check \
		ast_concrete_type \
		ast_constant \
		ast_decl \
		ast_enum \
		ast_enum_val \
		ast_exception \
		ast_expression \
		ast_field \
		ast_interface \
		ast_interface_fwd \
		ast_module \
		ast_operation \
		ast_predefined_type \
		ast_root \
		ast_sequence \
		ast_string \
		ast_structure \
		ast_type \
		ast_typedef \
		ast_union \
		ast_union_branch \
		ast_union_label \
		ast_generator \
		ast_redef \
		ast_recursive

FILES	 = $(AST_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS	= -last
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) #$(VLIB)


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

CPPFLAGS += -I../include -I. -DCPP_LOCATION=\"$(CXX)\" -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ast_array.o .shobj/ast_array.so: ast_array.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_argument.o .shobj/ast_argument.so: ast_argument.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_attribute.o .shobj/ast_attribute.so: ast_attribute.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_check.o .shobj/ast_check.so: ast_check.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_concrete_type.o .shobj/ast_concrete_type.so: ast_concrete_type.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_constant.o .shobj/ast_constant.so: ast_constant.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_decl.o .shobj/ast_decl.so: ast_decl.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_enum.o .shobj/ast_enum.so: ast_enum.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_enum_val.o .shobj/ast_enum_val.so: ast_enum_val.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_exception.o .shobj/ast_exception.so: ast_exception.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_expression.o .shobj/ast_expression.so: ast_expression.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_field.o .shobj/ast_field.so: ast_field.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_interface.o .shobj/ast_interface.so: ast_interface.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_interface_fwd.o .shobj/ast_interface_fwd.so: ast_interface_fwd.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_module.o .shobj/ast_module.so: ast_module.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_operation.o .shobj/ast_operation.so: ast_operation.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_predefined_type.o .shobj/ast_predefined_type.so: ast_predefined_type.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_root.o .shobj/ast_root.so: ast_root.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_sequence.o .shobj/ast_sequence.so: ast_sequence.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_string.o .shobj/ast_string.so: ast_string.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_structure.o .shobj/ast_structure.so: ast_structure.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_type.o .shobj/ast_type.so: ast_type.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_typedef.o .shobj/ast_typedef.so: ast_typedef.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_union.o .shobj/ast_union.so: ast_union.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_union_branch.o .shobj/ast_union_branch.so: ast_union_branch.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_union_label.o .shobj/ast_union_label.so: ast_union_label.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_generator.o .shobj/ast_generator.so: ast_generator.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_redef.o .shobj/ast_redef.so: ast_redef.cpp ../include/idl.h ../include/intlmacros.h \
 ../include/idl_narrow.h ../include/idl_defines.h ../include/idl_fwd.h \
 ../include/idl_bool.h ../include/utl_string.h \
 ../include/utl_identifier.h ../include/ast.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h
.obj/ast_recursive.o .shobj/ast_recursive.so: ast_recursive.cpp ../include/idl.h \
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
 ../include/utl_strlist.h ../include/utl_exprlist.h \
 ../include/ast_generator.h ../include/util.h ../include/utl_stack.h \
 ../include/utl_idlist.h ../include/utl_exceptlist.h \
 ../include/utl_namelist.h ../include/utl_labellist.h \
 ../include/utl_decllist.h ../include/idl_global.h \
 ../include/utl_error.h ../include/utl_indenter.h \
 ../include/idl_extern.h ../include/fe_extern.h ../include/be_extern.h \
 ../include/drv_extern.h ../include/ast_extern.h \
 ../include/nr_extern.h ../include/global_extern.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
