##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'include' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

## SUBDIRS = utl_tmpl

noinst_HEADERS = \
  ast.h \
  ast_argument.h \
  ast_array.h \
  ast_attribute.h \
  ast_concrete_type.h \
  ast_constant.h \
  ast_decl.h \
  ast_enum.h \
  ast_enum_val.h \
  ast_exception.h \
  ast_expression.h \
  ast_extern.h \
  ast_field.h \
  ast_generator.h \
  ast_interface.h \
  ast_interface_fwd.h \
  ast_module.h \
  ast_native.h \
  ast_operation.h \
  ast_predefined_type.h \
  ast_root.h \
  ast_sequence.h \
  ast_string.h \
  ast_structure.h \
  ast_structure_fwd.h \
  ast_type.h \
  ast_typedef.h \
  ast_union.h \
  ast_union_fwd.h \
  ast_union_branch.h \
  ast_union_label.h \
  ast_valuetype.h \
  ast_valuetype_fwd.h \
  be_extern.h \
  drv_extern.h \
  drv_link.h \
  drv_private.h \
  fe_declarator.h \
  fe_extern.h \
  fe_interface_header.h \
  fe_private.h \
  global_extern.h \
  idl.h \
  idl_bool.h \
  idl_defines.h \
  idl_extern.h \
  idl_fwd.h \
  idl_global.h \
  idl_narrow.h \
  intlmacros.h \
  nr_extern.h \
  util.h \
  utl_decllist.h \
  utl_err.h \
  utl_exceptlist.h \
  utl_exprlist.h \
  utl_identifier.h \
  utl_idlist.h \
  utl_indenter.h \
  utl_labellist.h \
  utl_list.h \
  utl_namelist.h \
  utl_scope.h \
  utl_scoped_name.h \
  utl_stack.h \
  utl_string.h \
  utl_strlist.h


## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

