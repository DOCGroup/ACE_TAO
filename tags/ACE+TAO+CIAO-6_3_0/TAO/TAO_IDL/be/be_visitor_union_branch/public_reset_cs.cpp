
//=============================================================================
/**
 *  @file    public_reset_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union Branch in the client inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union_branch.h"

// *****************************************************
//  visitor for union_branch in the client
//  stubs file for the reset method
// *****************************************************

be_visitor_union_branch_public_reset_cs::
be_visitor_union_branch_public_reset_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_public_reset_cs::
~be_visitor_union_branch_public_reset_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_reset_cs::visit_union_branch (
    be_union_branch *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node); // save the node

  *os << be_nl;

  for (unsigned long i = 0; i < node->label_list_length (); ++i)
    {
      // check if we are printing the default case
      if (node->label (i)->label_kind () == AST_UnionLabel::UL_default)
        {
          *os << "default:";
        }
      else
        {
          *os << "case ";
          node->gen_label_value (os, i);
          *os << ":";
        }
      if (i == (node->label_list_length () - 1))
        *os << be_idt_nl;
      else
        *os << be_nl;
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_array (be_array *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_enum - "
                         "bad context information\n"),
                         -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and.

  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name.

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
      ACE_OS::sprintf (fname, "%s", bt->full_name ());
    }

    TAO_OutStream *os = this->ctx_->stream ();

  *os << fname << "_free (this->u_." << ub->local_name ()
      << "_);" << be_nl
      << "this->u_." << ub->local_name () << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_enum (be_enum *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  *os << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_interface (be_interface *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_." << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_interface_fwd (be_interface_fwd *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_." << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_valuebox (
  be_valuebox *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_valuebox - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_." << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt_nl;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_valuetype (
  be_valuetype *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_valuetype - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_." << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_valuetype_fwd (
  be_valuetype_fwd *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_valuetype_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_." << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_object:
      *os << "delete this->u_."
          << ub->local_name () << "_;" << be_nl;
      *os << "this->u_." << ub->local_name ()
          << "_ = 0;" << be_nl
          << "break;" << be_uidt;

      break;
    case AST_PredefinedType::PT_pseudo:
      *os << "::CORBA::release (this->u_."
          << ub->local_name () << "_);" << be_nl;
      *os << "this->u_." << ub->local_name ()
          << "_ = 0;" << be_nl
          << "break;" << be_uidt;

      break;
    case AST_PredefinedType::PT_any:
      *os << "delete this->u_."
          << ub->local_name () << "_;" << be_nl
          << "this->u_." << ub->local_name ()
          << "_ = 0;" << be_nl
          << "break;" << be_uidt;

      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "break;" << be_uidt;

      break;
    }

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_sequence (
  be_sequence *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_."
      << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_string (
  be_string *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->width () == (long) sizeof (char))
    {
      *os << "::CORBA::string_free (this->u_.";
    }
  else
    {
      *os << "::CORBA::wstring_free (this->u_.";
    }

  *os << ub->local_name () << "_);" << be_nl
      << "this->u_."
      << ub->local_name ()
      << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_structure (
  be_structure *node)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (bt->size_type () == be_type::VARIABLE
      || node->has_constructor ())
    {
      *os << "delete this->u_." << ub->local_name ()
          << "_;" << be_nl
          << "this->u_."
          << ub->local_name ()
          << "_ = 0;" << be_nl;
   }

  *os << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_reset_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_union (
  be_union *)
{
  be_union_branch *ub =
    be_union_branch::narrow_from_decl (this->ctx_->node ());
  be_union *bu =
    be_union::narrow_from_scope (this->ctx_->scope ());

  if (ub == 0 || bu == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_reset_cs::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "delete this->u_."
      << ub->local_name () << "_;" << be_nl
      << "this->u_."
      << ub->local_name () << "_ = 0;" << be_nl
      << "break;" << be_uidt;

  return 0;
}

int
be_visitor_union_branch_public_reset_cs::visit_union_fwd (
  be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

