//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_reset_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch in the client inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"


// *****************************************************
//  visitor for union_branch in the client
//  stubs file for the reset method
// *****************************************************

// constructor
be_visitor_union_branch_public_reset_cs::
be_visitor_union_branch_public_reset_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_reset_cs::
~be_visitor_union_branch_public_reset_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_reset_cs::
visit_union_branch (be_union_branch *node)
{
  TAO_OutStream *os;
  be_type *bt; // union_branch's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node

  if (node->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
    {
      *os << "case " << node->label ()->label_val ()->n ()  << ":" << be_idt_nl;
    }
  else
    {
      *os << "case " << node->label ()->label_val () << ":" << be_idt_nl;
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

int
be_visitor_union_branch_public_reset_cs::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << bt->name () << "_free (this->u_." << ub->local_name () << "_);" << be_nl;
  *os << "break;" << be_uidt_nl;
  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "CORBA::release (this->u_." << ub->local_name () << "_);" << be_nl;
  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "CORBA::release (this->u_." << ub->local_name () << "_);" << be_nl;
  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      *os << "CORBA::release (this->u_." << ub->local_name () << "_);" << be_nl;
      *os << "break;" << be_uidt_nl;
      break;
    case AST_PredefinedType::PT_any:
      *os << "delete this->u_." << ub->local_name () << "_;" << be_nl;
      *os << "break;" << be_uidt_nl;
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "break;" << be_uidt_nl;
    }
  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "delete this->u_." << ub->local_name () << "_;" << be_nl;
  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_string (be_string *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "CORBA::string_free (this->u_." << ub->local_name () << "_);" << be_nl;
  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  if (bt->size_type () == be_type::VARIABLE)
    {
      *os << "delete this->u_." << ub->local_name () << "_;" << be_nl;
    }

  *os << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  *os << "delete this->u_." << ub->local_name () << "_;" << be_nl;
  *os << "break;" << be_uidt_nl;

  return 0;
}
