##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for the TAO IDL compiler --- 'be' sub-directory
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir) \
           -I$(top_srcdir)/TAO_IDL/include \
           -I$(top_srcdir)/TAO_IDL/be_include

noinst_LIBRARIES = libbe.a

libbe_a_SOURCES = \
         be_args.cpp  \
         be_argument.cpp  \
         be_array.cpp  \
         be_attribute.cpp  \
         be_codegen.cpp  \
         be_constant.cpp  \
         be_decl.cpp  \
         be_enum.cpp  \
         be_enum_val.cpp  \
         be_exception.cpp  \
         be_expression.cpp  \
         be_field.cpp  \
         be_generator.cpp  \
         be_helper.cpp  \
         be_init.cpp  \
         be_interface.cpp  \
         be_interface_fwd.cpp  \
         be_module.cpp  \
         be_native.cpp  \
         be_operation.cpp  \
         be_predefined_type.cpp  \
         be_produce.cpp  \
         be_root.cpp  \
         be_scope.cpp  \
         be_sequence.cpp  \
         be_stream_factory.cpp  \
         be_string.cpp  \
         be_structure.cpp  \
         be_sunsoft.cpp  \
         be_type.cpp  \
         be_typedef.cpp  \
         be_union.cpp  \
         be_union_branch.cpp  \
         be_union_label.cpp  \
         be_visitor.cpp  \
         be_visitor_argument.cpp  \
         be_visitor_array.cpp  \
         be_visitor_attribute.cpp  \
         be_visitor_constant.cpp  \
         be_visitor_context.cpp  \
         be_visitor_decl.cpp  \
         be_visitor_enum.cpp  \
         be_visitor_exception.cpp  \
         be_visitor_factory.cpp  \
         be_visitor_field.cpp  \
         be_visitor_interface.cpp  \
         be_visitor_interface_fwd.cpp  \
         be_visitor_module.cpp  \
         be_visitor_operation.cpp  \
         be_visitor_root.cpp  \
         be_visitor_scope.cpp  \
         be_visitor_sequence.cpp  \
         be_visitor_structure.cpp  \
         be_visitor_typecode.cpp  \
         be_visitor_typedef.cpp  \
         be_visitor_union.cpp  \
         be_visitor_union_branch.cpp

dist-hook:
	list=`find * -name CVS -prune -o -type d -print`; for p in $$list; do \
          $(mkinstalldirs) $(distdir)/$$p; \
          cp $$p/*.cpp $(distdir)/$$p; \
        done
