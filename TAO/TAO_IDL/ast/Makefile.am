##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'ast' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir) \
           -I$(top_srcdir)/TAO_IDL/include

noinst_LTLIBRARIES = libast.la

libast_la_SOURCES = \
  ast_argument.cpp \
  ast_array.cpp \
  ast_attribute.cpp \
  ast_check.cpp \
  ast_component.cpp \
  ast_component_fwd.cpp \
  ast_concrete_type.cpp \
  ast_constant.cpp \
  ast_decl.cpp \
  ast_enum.cpp \
  ast_enum_val.cpp \
  ast_exception.cpp \
  ast_expression.cpp \
  ast_field.cpp \
  ast_generator.cpp \
  ast_home.cpp \
  ast_interface.cpp \
  ast_interface_fwd.cpp \
  ast_module.cpp \
  ast_native.cpp \
  ast_operation.cpp \
  ast_predefined_type.cpp \
  ast_recursive.cpp \
  ast_redef.cpp \
  ast_root.cpp \
  ast_sequence.cpp \
  ast_string.cpp \
  ast_structure.cpp \
  ast_structure_fwd.cpp \
  ast_type.cpp \
  ast_typedef.cpp \
  ast_union.cpp \
  ast_union_fwd.cpp \
  ast_union_branch.cpp \
  ast_union_label.cpp \
  ast_valuetype.cpp \
  ast_valuetype_fwd.cpp \
  ast_visitor.cpp


## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

