#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libbe
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME	 = libbe
LIB	 = $(LIBNAME).a
SHLIB	 = $(LIBNAME).so
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of
BE_FILES	= \
		be_array \
		be_argument \
		be_attribute \
		be_constant \
		be_enum \
		be_enum_val \
		be_exception \
		be_expression \
		be_field \
		be_interface \
		be_interface_fwd \
		be_module \
		be_operation \
		be_predefined_type \
		be_sequence \
		be_string \
		be_structure \
		be_typedef \
		be_union \
		be_union_branch \
		be_union_label \
		be_generator \
		be_generator \
		be_init \
		be_produce \
		be_args \
		be_helper

FILES	 = $(BE_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS  =  -lbe
LIBS	= -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) $(VLIB)

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

CPPFLAGS += -I../include -I../be_include -I. -DCPP_LOCATION=\"$(CXX)\" -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/be_array.o .shobj/be_array.so: be_array.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_argument.o .shobj/be_argument.so: be_argument.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_attribute.o .shobj/be_attribute.so: be_attribute.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_constant.o .shobj/be_constant.so: be_constant.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_enum.o .shobj/be_enum.so: be_enum.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_enum_val.o .shobj/be_enum_val.so: be_enum_val.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_exception.o .shobj/be_exception.so: be_exception.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_expression.o .shobj/be_expression.so: be_expression.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_field.o .shobj/be_field.so: be_field.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_interface.o .shobj/be_interface.so: be_interface.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_interface_fwd.o .shobj/be_interface_fwd.so: be_interface_fwd.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_module.o .shobj/be_module.so: be_module.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_operation.o .shobj/be_operation.so: be_operation.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_predefined_type.o .shobj/be_predefined_type.so: be_predefined_type.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_sequence.o .shobj/be_sequence.so: be_sequence.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_string.o .shobj/be_string.so: be_string.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_structure.o .shobj/be_structure.so: be_structure.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_typedef.o .shobj/be_typedef.so: be_typedef.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_union.o .shobj/be_union.so: be_union.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_union_branch.o .shobj/be_union_branch.so: be_union_branch.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_union_label.o .shobj/be_union_label.so: be_union_label.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_generator.o .shobj/be_generator.so: be_generator.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_generator.o .shobj/be_generator.so: be_generator.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_init.o .shobj/be_init.so: be_init.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_produce.o .shobj/be_produce.so: be_produce.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h
.obj/be_args.o .shobj/be_args.so: be_args.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h ../be_include/be.h \
 ../be_include/be_generator.h ../be_include/be_predefined_type.h \
 ../be_include/be_module.h ../be_include/be_interface.h \
 ../be_include/be_interface_fwd.h ../be_include/be_structure.h \
 ../be_include/be_exception.h ../be_include/be_expression.h \
 ../be_include/be_enum.h ../be_include/be_operation.h \
 ../be_include/be_field.h ../be_include/be_argument.h \
 ../be_include/be_attribute.h ../be_include/be_union.h \
 ../be_include/be_union_branch.h ../be_include/be_union_label.h \
 ../be_include/be_constant.h ../be_include/be_enum_val.h \
 ../be_include/be_array.h ../be_include/be_sequence.h \
 ../be_include/be_string.h ../be_include/be_typedef.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
