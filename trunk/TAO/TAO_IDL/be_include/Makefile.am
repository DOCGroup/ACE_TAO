##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'be_include' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

noinst_HEADERS = \
         be.h  \
         be_argument.h  \
         be_array.h  \
         be_attribute.h  \
         be_codegen.h  \
         be_constant.h  \
         be_decl.h  \
         be_enum.h  \
         be_enum_val.h  \
         be_exception.h  \
         be_expression.h  \
         be_field.h  \
         be_generator.h  \
         be_helper.h  \
         be_interface.h  \
         be_interface_fwd.h  \
         be_module.h  \
         be_native.h  \
         be_operation.h  \
         be_predefined_type.h  \
         be_root.h  \
         be_scope.h  \
         be_sequence.h  \
         be_state.h  \
         be_stream_factory.h  \
         be_string.h  \
         be_structure.h  \
         be_sunsoft.h  \
         be_type.h  \
         be_typedef.h  \
         be_union.h  \
         be_union_branch.h  \
         be_union_label.h  \
         be_visitor.h  \
         be_visitor_argument.h  \
         be_visitor_array.h  \
         be_visitor_attribute.h  \
         be_visitor_constant.h  \
         be_visitor_context.h  \
         be_visitor_decl.h  \
         be_visitor_enum.h  \
         be_visitor_exception.h  \
         be_visitor_factory.h  \
         be_visitor_field.h  \
         be_visitor_interface.h  \
         be_visitor_interface_fwd.h  \
         be_visitor_module.h  \
         be_visitor_operation.h  \
         be_visitor_root.h  \
         be_visitor_scope.h  \
         be_visitor_sequence.h  \
         be_visitor_structure.h  \
         be_visitor_typecode.h  \
         be_visitor_typedef.h  \
         be_visitor_union.h  \
         be_visitor_union_branch.h

dist-hook:
	list=`find * -name CVS -prune -o -type d -print`; for p in $$list; do \
          $(mkinstalldirs) $(distdir)/$$p; \
          cp $$p/*.h $(distdir)/$$p; \
        done
