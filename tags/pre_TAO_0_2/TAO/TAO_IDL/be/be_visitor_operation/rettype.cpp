//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype.cpp
//
// = DESCRIPTION
//    Visitor generating code for return type of the Operation node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"


// ****************************************************************************
// Operation visitor for return types. This generates the mapping for a return
// type in an operation signature
// ****************************************************************************

be_visitor_operation_rettype::be_visitor_operation_rettype (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype::~be_visitor_operation_rettype (void)
{
}

int
be_visitor_operation_rettype::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_slice") << " *";
  else
    *os << bt->name () << "_slice *";
  return 0;
}

int
be_visitor_operation_rettype::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int
be_visitor_operation_rettype::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << bt->name () << "_ptr";
  return 0;
}

int
be_visitor_operation_rettype::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << bt->name () << "_ptr";
  return 0;
}

int
be_visitor_operation_rettype::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
  else
    *os << bt->name () << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
      else
        *os << bt->name () << "_ptr";
      break;
    case AST_PredefinedType::PT_any:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
      else
        *os << bt->name () << " *";
      break;
    default:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope ());
      else
        *os << bt->name ();
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype::visit_sequence (be_sequence *node)
{
  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
  else
    *os << bt->name () << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "char *";
  return 0;
}

int
be_visitor_operation_rettype::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();

  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();

  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}
