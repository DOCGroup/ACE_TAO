#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libfe
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libfe
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).so
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of
FE_FILES	= \
		fe_declarator \
		fe_private \
		fe_init \
		fe_extern \
		fe_interface_header

OTHER_FILES	= \
		y.tab \
		lex.yy 

FILES	 = $(FE_FILES) $(OTHER_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LHDR	= $(addsuffix .h,$(HDRS))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS	= -lfe
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
YACC	= yacc
LEX	= lex
YFLAGS	= -t -d
LFLAGS	= -t

# Create the parser and lexer
y.tab.hh y.tab.cpp: idl.yy
	@-$(RM) $@
	$(YACC) $(YFLAGS) idl.yy
	mv -f y.tab.c y.tab.cpp

lex.yy.cpp: idl.ll y.tab.h
	$(LEX) $(LFLAGS) idl.ll > $@


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/fe_declarator.o .shobj/fe_declarator.so: fe_declarator.cpp ../include/idl.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/fe_private.o .shobj/fe_private.so: fe_private.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/fe_init.o .shobj/fe_init.so: fe_init.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/fe_extern.o .shobj/fe_extern.so: fe_extern.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/fe_interface_header.o .shobj/fe_interface_header.so: fe_interface_header.cpp ../include/idl.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/y.tab.o .shobj/y.tab.so: y.tab.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h
.obj/lex.yy.o .shobj/lex.yy.so: lex.yy.cpp ../include/idl.h ../include/intlmacros.h \
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
 ../include/fe_private.h ../include/fe_interface_header.h \
 ../include/fe_declarator.h y.tab.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
