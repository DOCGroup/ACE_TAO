
//=============================================================================
/**
 *  @file    field_ci.cpp
 *
 *  $Id$
 *
 *   Visitor for the Valuetype class.
 *   This one generates code for accessor and modifier functions of
 *   valuetype state members (in the inline file).
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "valuetype.h"

be_visitor_valuetype_field_ci::be_visitor_valuetype_field_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx),
    in_obv_space_ (0)
{
  setenclosings ("");
}

be_visitor_valuetype_field_ci::~be_visitor_valuetype_field_ci (void)
{
}

int
be_visitor_valuetype_field_ci::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types that a field can be.

int
be_visitor_valuetype_field_ci::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_ci::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_ci::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());
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
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_ci visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

void
be_visitor_valuetype_field_ci::setenclosings (const char *pre)
{
  this->pre_op_ = pre;
}

const char*
be_visitor_valuetype_field_ci::pre_op ()
{
  return this->pre_op_;
}

// Retrieve the fully scoped skeleton name.
void
be_visitor_valuetype_field_ci::op_name (be_valuetype *node,
                                        TAO_OutStream *os)
{
  if (this->in_obv_space_)
    {
      *os << node->full_obv_skel_name ();
    }
  else
    {
      *os << node->name ();
    }
}
