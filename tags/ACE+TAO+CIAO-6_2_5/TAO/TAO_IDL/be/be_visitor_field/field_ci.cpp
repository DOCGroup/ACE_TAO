
//=============================================================================
/**
 *  @file    field_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Field in the client inline file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "field.h"
#include "be_visitor_array/array_ci.h"
#include "be_visitor_structure/structure_ci.h"
#include "be_visitor_union/union_ci.h"

be_visitor_field_ci::be_visitor_field_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_field_ci::~be_visitor_field_ci (void)
{
}

int
be_visitor_field_ci::visit_field (be_field *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ci::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ci::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types  that a field can be

int
be_visitor_field_ci::visit_array (be_array *node)
{
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_ci::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor_field_ci::visit_structure (be_structure *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_ci::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_field_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_field_ci::visit_union (be_union *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_ci::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

