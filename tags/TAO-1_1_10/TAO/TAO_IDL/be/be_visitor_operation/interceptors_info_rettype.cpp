//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_info_rettype.cpp
//
// = DESCRIPTION
//    Visitor generating code for return type stored in RequestInfo class for 
//    Interceptors.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, interceptors_info_rettype, "$Id$")


// ****************************************************************************
// Operation visitor for return types. This generates the mapping for a return
// type in an operation signature
// ****************************************************************************

be_visitor_operation_interceptors_info_rettype::be_visitor_operation_interceptors_info_rettype (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_interceptors_info_rettype::~be_visitor_operation_interceptors_info_rettype (void)
{
}

int
be_visitor_operation_interceptors_info_rettype::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;
  // ACE_NESTED_CLASS macros needed for MSVC.
  be_decl* scope = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
  
  if (bt->is_nested () && (scope->node_type () == AST_Decl::NT_interface 
                           || scope->node_type () == AST_Decl::NT_union ))
    {
      *os << "ACE_NESTED_CLASS (";
      *os << scope->name () << ",";
      *os << bt->local_name ();
      *os  << "_slice) *";
    }
  else
  *os << bt->name () << "_slice *";

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_enum (be_enum *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name ();

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_interface (be_interface *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name () << "_ptr";

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_interface_fwd (be_interface_fwd *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name () << "_ptr";

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_native (be_native *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name ();

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_predefined_type (be_predefined_type *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  // ACE_NESTED_CLASS macros needed for MSVC, but only for stub code,
  // because otherwise the types are not defined in the
  // same scope, which they must be to use ACE_NESTED_CLASS.

  be_decl* scope =
    be_scope::narrow_from_scope (bt->defined_in ())->decl ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      *os << bt->name () << "_ptr";
      break;
    case AST_PredefinedType::PT_any:
      *os << bt->name () << " *";
      break;  
    default:
      {
        if (bt->is_nested () 
            && (scope->node_type () == AST_Decl::NT_interface
                || scope->node_type () == AST_Decl::NT_union ))
          {
            *os << "ACE_NESTED_CLASS (";
            *os << scope->name () << ", ";
            *os << bt->local_name ();
	          *os  << ")";
          }
        else
          {
            *os << bt->name ();
          }

        break;
      }
    }

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_sequence (be_sequence *node)
{

  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;
  // ACE_NESTED_CLASS macros needed for MSVC.
  be_decl* scope = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
  
  if (bt->is_nested () && (scope->node_type () == AST_Decl::NT_interface 
                           || scope->node_type () == AST_Decl::NT_union ))
    {
      *os << "ACE_NESTED_CLASS (";
      *os << scope->name () << ",";
      *os << bt->local_name ();
      *os  << ") *";
    }
  else
    *os << bt->name () << " *";
  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_string (be_string *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  if (node->width () == (long) sizeof (char))
    {
      *os << "char *";
    }
  else
    {
      *os << "CORBA::WChar *";
    }

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_structure (be_structure *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;
  // ACE_NESTED_CLASS macros needed for MSVC, but only for stub code,
  // because otherwise the types are not defined in the
  // same scope, which they must be to use ACE_NESTED_CLASS.

  be_decl* scope =
    be_scope::narrow_from_scope (bt->defined_in ())->decl ();

  if (bt->is_nested () && (scope->node_type () == AST_Decl::NT_interface 
                               || scope->node_type () == AST_Decl::NT_union ))
    {
      *os << "ACE_NESTED_CLASS (";
      *os << scope->name () << ",";
      *os << bt->local_name ();
      *os  << ")";
    }
  else
    *os << bt->name ();
  
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_typedef (be_typedef *node)
{

  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_interceptors_info_rettype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_union (be_union *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  // ACE_NESTED_CLASS macros needed for MSVC.
  be_decl* scope = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
  
  if (bt->is_nested () && (scope->node_type () == AST_Decl::NT_interface 
                           || scope->node_type () == AST_Decl::NT_union ))
    {
      *os << "ACE_NESTED_CLASS (";
      *os << scope->name () << ",";
      *os << bt->local_name ();
      *os  << ")";
    }
  else
    *os << bt->name ();
  
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

int
be_visitor_operation_interceptors_info_rettype::visit_valuetype (be_valuetype *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name () << " *";

  return 0;
}

int
be_visitor_operation_interceptors_info_rettype::visit_valuetype_fwd (be_valuetype_fwd *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  *os << bt->name () << " *";

  return 0;
}

#endif /* IDL_HAS_VALUETYPE */
