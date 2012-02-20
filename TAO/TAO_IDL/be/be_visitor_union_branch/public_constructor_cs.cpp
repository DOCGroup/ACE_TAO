
//=============================================================================
/**
 *  @file    public_constructor_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union Branch for the constructor operator
 *
 *
 *  @author Johnny Willemsen
 */
//=============================================================================

#include "union_branch.h"

// **********************************************
//  visitor for union_branch in the client stubs file generating the code for
//  the copy ctor and assignment operator
// **********************************************

// constructor
be_visitor_union_branch_public_constructor_cs::
be_visitor_union_branch_public_constructor_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_constructor_cs::
~be_visitor_union_branch_public_constructor_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_constructor_cs::visit_union_branch (
    be_union_branch *node)
{
  // first generate the type information
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }

  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

int
be_visitor_union_branch_public_constructor_cs::visit_array (be_array *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // for anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name

      if (bt->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (bt->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->full_name (),
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", bt->full_name ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", bt->full_name ());
    }

  // set the discriminant to the appropriate label
  *os << "this->u_." << ub->local_name ()
      << "_ = " << be_idt_nl
      << fname << "_alloc ();" << be_uidt << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // set the discriminant to the appropriate label
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_any:
      *os << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << ");" << be_uidt;

      break;
    default:
      break;
    }

  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_sequence (be_sequence *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "ACE_NEW (" << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << ");" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_structure (be_structure *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->size_type () == be_type::VARIABLE || node->has_constructor ())
    {
      *os << "ACE_NEW (" << be_idt_nl
          << "this->u_." << ub->local_name () << "_," << be_nl
          << bt->name () << ");" << be_uidt;
    }

  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_constructor_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_union (be_union *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_constructor_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "ACE_NEW (" << be_idt_nl
      << "this->u_." << ub->local_name () << "_," << be_nl
      << bt->name () << ");" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_constructor_cs::visit_union_fwd (
  be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

