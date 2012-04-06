
//=============================================================================
/**
 *  @file    union_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Union in the client inline file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_ci::be_visitor_union_ci (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ci::~be_visitor_union_ci (void)
{
}

int be_visitor_union_ci::visit_union (be_union *node)
{
  if (node->cli_inline_gen () || node->imported ())
    {
      return 0;
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for union " << node->name () << be_nl;
  *os << "// *************************************************************";

  // the discriminant type may have to be defined here if it was an enum
  // declaration inside of the union statement.

  be_type *bt = be_type::narrow_from_decl (node->disc_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ci::"
                         "visit_union - "
                         "bad discriminant type\n"),
                        -1);
    }

  be_visitor_union_discriminant_ci visitor (&ctx);

  if (bt->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ci::"
                         "visit union - "
                         "codegen for discrminant failed\n"),
                        -1);
    }

  // Now generate the implementation of the access methods for the
  // union. For this set our state.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ci::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_inline_gen (true);
  return 0;
}
