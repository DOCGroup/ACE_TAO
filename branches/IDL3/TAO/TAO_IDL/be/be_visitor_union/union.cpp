//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union.cpp
//
// = DESCRIPTION
//    Generic visitor generating code for Unions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           union, 
           "$Id$")

be_visitor_union::be_visitor_union (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_union::~be_visitor_union (void)
{
}

int
be_visitor_union::visit_union (be_union *)
{
  return -1; // Must be overriden.
}

int
be_visitor_union::visit_union_branch (be_union_branch *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);
        be_visitor_union_branch_public_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);
        be_visitor_union_branch_private_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
        be_visitor_union_branch_public_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CS);
        be_visitor_union_branch_public_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);
        be_visitor_union_branch_public_assign_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS);
        be_visitor_union_branch_public_reset_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_ACCESS_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_ACCESS_CS);
        be_visitor_union_branch_public_access_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CH);
        be_visitor_union_branch_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CI);
        be_visitor_union_branch_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CS);
        be_visitor_union_branch_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_union::"
                           "visit_union_branch - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}
