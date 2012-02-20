
//=============================================================================
/**
 *  @file    typecode_decl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TypeCode declaration for a type
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "typecode.h"

be_visitor_typecode_decl::be_visitor_typecode_decl (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
  if (be_global->gen_anyop_files ())
    {
      // The context is always a copy, so this is ok.
      this->ctx_->stream (tao_cg->anyop_header ());
    }
}

be_visitor_typecode_decl::~be_visitor_typecode_decl (void)
{
}

int
be_visitor_typecode_decl::visit_type (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // If -GA is used but anyop macro isn't set, defaults to stub macro.
  const char *export_macro = (be_global->gen_anyop_files ()
                              ? this->ctx_->non_null_export_macro ()
                              : be_global->stub_export_macro ());

  if (node->is_nested ())
    {
      // We have a scoped name.
      // Is our enclosing scope a module? We need this check because
      // for platforms that support namespaces, the TypeCode must be
      // declared extern.
      if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
        {
          *os << "extern " << export_macro << " ";
        }
      else
        {
          *os << "static ";
        }

      *os << "::CORBA::TypeCode_ptr const "
          << node->tc_name ()->last_component ()
          << ";";
    }
  else
    {
      // We are in the ROOT scope.
      *os << "extern " << export_macro
          << " ::CORBA::TypeCode_ptr const "
          << node->tc_name ()->last_component ()
          << ";";
    }

  return 0;
}

int
be_visitor_typecode_decl::visit_array (be_array *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_enum (be_enum *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_exception (be_exception *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_interface (be_interface *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_component (be_component *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_connector (be_connector *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_home (be_home *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_sequence (be_sequence *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_structure (be_structure *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_typedef (be_typedef *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_union (be_union *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_valuebox (be_valuebox *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_valuetype (be_valuetype *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_eventtype (be_eventtype *node)
{
  return this->visit_type (node);
}
