//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    buffer_type.cpp
//
// = DESCRIPTION
//    Visitor generating code for the internal buffer type of the Sequence node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"


// ****************************************************************
// We have to generate the buffer type in the constructor
// ****************************************************************

be_visitor_sequence_buffer_type::
be_visitor_sequence_buffer_type (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_buffer_type::~be_visitor_sequence_buffer_type (void)
{
}

// helper
int
be_visitor_sequence_buffer_type::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << node->name () << "_ptr";
  else
    *os << node->name ();
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << node->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << node->name () << "_ptr";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << node->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << node->name () << "_ptr";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << "char *";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_buffer_type::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
