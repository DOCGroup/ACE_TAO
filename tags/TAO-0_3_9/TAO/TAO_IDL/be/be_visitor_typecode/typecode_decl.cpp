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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_typecode.h"

ACE_RCSID(be_visitor_typecode, typecode_decl, "$Id$")


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

  // Generate the typecode decl
  if (node->is_nested ())
    {
      // we have a scoped name
      os->indent ();
      // is our enclosing scope a module? We need this check because for
      // platforms that support namespaces, the typecode must be declared
      // extern
      if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
        *os << "TAO_NAMESPACE_STORAGE_CLASS ";
      else
        *os << "static ";
      *os << "CORBA::TypeCode_ptr "
          << node->tc_name ()->last_component () << ";\n\n";
    }
  else
    {
      // we are in the ROOT scope
      os->indent ();
      *os << "extern " << idl_global->export_macro () << " CORBA::TypeCode_ptr "
          << " " << node->tc_name ()->last_component () << ";\n\n";
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
