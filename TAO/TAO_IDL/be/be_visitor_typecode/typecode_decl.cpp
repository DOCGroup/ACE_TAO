//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typecode_decl.cpp
//
// = DESCRIPTION
//    Visitor generating code for TypeCode declaration for a type
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_typecode, 
           typecode_decl, 
           "$Id$")

// ******************************************************
// TypeCode declarations
// ******************************************************

be_visitor_typecode_decl::be_visitor_typecode_decl (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_typecode_decl::~be_visitor_typecode_decl (void)
{
}

int
be_visitor_typecode_decl::visit_type (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (node->is_nested ())
    {
      // We have a scoped name.
      // Is our enclosing scope a module? We need this check because for
      // platforms that support namespaces, the typecode must be declared
      // extern.
      if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
        {
          *os << "TAO_NAMESPACE_STORAGE_CLASS ";
        }
      else
        {
          *os << "static ";
        }

      *os << "::CORBA::TypeCode_ptr "
          << node->tc_name ()->last_component ()
          << ";";
    }
  else
    {
      // We are in the ROOT scope.
      *os << "extern " << be_global->stub_export_macro ()
          << " ::CORBA::TypeCode_ptr "
          << " " << node->tc_name ()->last_component ()
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
be_visitor_typecode_decl::visit_valuetype (be_valuetype *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_decl::visit_eventtype (be_eventtype *node)
{
  return this->visit_type (node);
}
