//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_branch_cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union_Branch in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union_branch,
           cdr_op_cs,
           "$Id$")

// **********************************************
//  Visitor for union_branch in the client stubs file.
// **********************************************

be_visitor_union_branch_cdr_op_cs::be_visitor_union_branch_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_cdr_op_cs::~be_visitor_union_branch_cdr_op_cs (void)
{
}

int
be_visitor_union_branch_cdr_op_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_array (be_array *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // This is the case for anonymous arrays.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_sequence (be_sequence *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // Anonymous sequence
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_structure (be_structure *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
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
be_visitor_union_branch_cdr_op_cs::visit_union (be_union *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}
