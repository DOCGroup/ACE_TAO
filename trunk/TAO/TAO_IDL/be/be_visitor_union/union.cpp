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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, union, "$Id$")


// generic struct visitor
be_visitor_union::be_visitor_union (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_union::~be_visitor_union (void)
{
}

// visit the Union node and its scope
int
be_visitor_union::visit_union (be_union *)
{
  return -1; // must be overriden
}

int
be_visitor_union::visit_union_branch (be_union_branch *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);
      break;
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CS);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_ACCESS_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_ACCESS_CS);
      break;
    case TAO_CodeGen::TAO_UNION_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_UNION_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_union::"
                           "visit_union_branch - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}
