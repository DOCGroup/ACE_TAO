
//=============================================================================
/**
 *  @file    public_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union Branch in the client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// **********************************************
//  Visitor for union_branch in the client stubs file.
// **********************************************

be_visitor_union_branch_public_cs::be_visitor_union_branch_public_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_public_cs::~be_visitor_union_branch_public_cs (void)
{
}

int
be_visitor_union_branch_public_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_array (be_array *node)
{
  // If not a typedef and we are defined in the use scope, we must be
  // defined.
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Anonymous array case.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_enum (be_enum *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  be_visitor_enum_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_enum - "
                         "codegen failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_sequence (be_sequence *node)
{
  // If not a typedef and we are defined in the use scope, we must be
  // defined.

  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Anonymous sequence case.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_union_branch_public_cs::visit_structure (be_structure *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  be_visitor_structure_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_struct - "
                         "codegen failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_structure_fwd (be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_cs::visit_union (be_union *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  be_visitor_union_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union - "
                         "codegen failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_public_cs::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

