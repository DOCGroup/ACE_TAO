#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libdrv
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libdrv
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).so
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of
DRIVER_FILES = \
		drv_init \
		drv_private \
		drv_main \
		drv_args \
		drv_fork \
		drv_link \
		drv_preproc

FILES	 = $(DRIVER_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS	= -ldrv
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

# Need the following. But we should be able to handle this using ACE's functionality
OSV	= SOLARIS2 
CPPFLAGS += -I../include -I. -DCPP_LOCATION=\"$(CXX)\" -DIDL_CFE_VERSION=$(IDL_CFE_VERSION) -D$(OSV)

$(LOBJS): $(LHDRS)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/drv_init.o .shobj/drv_init.so: drv_init.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h ../include/drv_link.h
.obj/drv_private.o .shobj/drv_private.so: drv_private.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h
.obj/drv_main.o .shobj/drv_main.so: drv_main.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h ../include/drv_link.h
.obj/drv_args.o .shobj/drv_args.so: drv_args.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h ../include/drv_link.h
.obj/drv_fork.o .shobj/drv_fork.so: drv_fork.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h
.obj/drv_link.o .shobj/drv_link.so: drv_link.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h ../include/drv_link.h
.obj/drv_preproc.o .shobj/drv_preproc.so: drv_preproc.cpp ../include/idl.h \
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
 ../include/nr_extern.h ../include/global_extern.h \
 ../include/drv_private.h ../include/drv_link.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
