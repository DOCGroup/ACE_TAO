//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, union_ci, "$Id$")


// ******************************************************
// for client inline
// ******************************************************

be_visitor_union_ci::be_visitor_union_ci (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ci::~be_visitor_union_ci (void)
{
}

// visit the Union node and its scope
int be_visitor_union_ci::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  if (!node->cli_inline_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Inline operations for union " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      bt = be_type::narrow_from_decl (node->disc_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad discriminant type\n"), -1);
        }

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI); // set current code
                                                          // gen state
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad visitor\n"), -1);
        }
      // generate code for the discriminant
      if (bt->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit union - "
                             "codegen for discrminant failed\n"), -1);
        }

      // now generate the implementation of the access methods for the
      // union. For this set our state
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for scope failed\n"), -1);
        }

      // generate the ifdefined macro for the array type
      os->gen_ifdef_macro (node->flat_name (), "_var");
      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdefined macro for the array type then generate the _out
      // impl
      os->gen_ifdef_macro (node->flat_name (), "_out");
      if (node->size_type () == be_decl::VARIABLE
          && node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _out failed\n"), -1);
        }
      os->gen_endif ();

      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}
