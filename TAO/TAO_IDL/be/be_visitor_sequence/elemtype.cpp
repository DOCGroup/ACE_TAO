//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    elemtype.cpp
//
// = DESCRIPTION
//    Visitor generating code for the element type for the Sequence
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, elemtype, "$Id$")


// ***********************************************************
// sequence element type visitor for return types of [] methods
// ***********************************************************


be_visitor_sequence_elemtype::
be_visitor_sequence_elemtype (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_elemtype::~be_visitor_sequence_elemtype (void)
{
}

// helper
int
be_visitor_sequence_elemtype::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " &";
  else
    *os << bt->name () << " &";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;
  
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // !! branching in either compile time template instantiation
      // or manual template instatiation
      os->gen_ifdef_AHETI();
  
      if (this->ctx_->state() != TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
        {
          if (this->ctx_->scope()->fullname())
            *os << this->ctx_->scope()->fullname() << "::";
        }

      *os << "_TAO_Object_Manager_" 
          << this->ctx_->scope()->flatname() 
          << "_" 
          << bt->flatname ();

      os->gen_else_AHETI();

      if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
        {
          *os << "TAO_Object_Manager<" 
              << bt->nested_type_name (this->ctx_->scope ()) << "> ";
        }
      else
        *os << "TAO_Object_Manager<" << bt->name () << "> ";        

      os->gen_endif_AHETI();
      break;
    default:
      if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
	      *os << bt->nested_type_name (this->ctx_->scope ()) << " &";
      else
	      *os << bt->name () << " &";
    }
  return 0;
}

int
be_visitor_sequence_elemtype::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  if (this->ctx_->state() != TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      if (this->ctx_->scope()->fullname())
        *os << this->ctx_->scope()->fullname() << "::";
    }

  *os << "_TAO_Object_Manager_" 
      << this->ctx_->scope()->flatname() 
      << "_" 
      << bt->flatname ();
  
  os->gen_else_AHETI();

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Object_Manager<" 
          << bt->nested_type_name (this->ctx_->scope ()) << "> ";
    }
  else
    *os << "TAO_Object_Manager<" << bt->name () << "> ";        

  os->gen_endif_AHETI();

  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->gen_ifdef_AHETI();

  if (this->ctx_->state() != TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
  {
    if (this->ctx_->scope()->fullname())
      *os << this->ctx_->scope()->fullname() << "::";
  }

  *os << "_TAO_Object_Manager_" 
      << this->ctx_->scope()->flatname() 
      << "_" 
      << bt->flatname ();

  os->gen_else_AHETI();
 
  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Object_Manager<" 
          << bt->nested_type_name (this->ctx_->scope ()) << "> ";
    }
  else
    *os << "TAO_Object_Manager<" << bt->name () << "> ";        

  os->gen_endif_AHETI();

  return 0;
}

int
be_visitor_sequence_elemtype::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "TAO_SeqElem_String_Manager ";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_elemtype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
