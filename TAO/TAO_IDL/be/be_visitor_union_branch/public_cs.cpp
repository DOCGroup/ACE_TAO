//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch in the client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union_branch, 
           public_cs, 
           "$Id$")

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
      && node->is_child (this->ctx_->scope ()))
    {
      // Anonymous array case.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
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
  ctx.state (TAO_CodeGen::TAO_ENUM_CS);
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
      && node->is_child (this->ctx_->scope ()))
    {
      // Anonymous sequence case.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
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
  ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
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
be_visitor_union_branch_public_cs::visit_union (be_union *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  ctx.state (TAO_CodeGen::TAO_UNION_CS);
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
