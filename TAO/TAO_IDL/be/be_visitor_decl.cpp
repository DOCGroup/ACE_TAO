//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_decl.cpp
//
// = DESCRIPTION
//    Visitor for the base be_decl node. This serves to maintain the current
//    state (context) of code generation for the derived visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_visitor_decl.h"
#include "be_visitor_context.h"
#include "be_typedef.h"
#include "be_type.h"

ACE_RCSID (be, 
           be_visitor_decl, 
           "$Id$")


// Root visitor for client header
be_visitor_decl::be_visitor_decl (be_visitor_context *ctx)
  : ctx_ (ctx)
{
}

be_visitor_decl::~be_visitor_decl (void)
{
//  delete this->ctx_;
}

int
be_visitor_decl::gen_anonymous_base_type (be_type *bt,
                                          TAO_CodeGen::CG_STATE cg_state)
{
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  if (!tdef)
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.state (cg_state);

      // In case our container was typedef'd.
      ctx.tdef (0);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_decl::"
                             "gen_anonymous_base_type - "
                             "bad visitor to anonymous abase type\n"),
                            -1);
        }

      if (bt->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_decl::"
                             "gen_anonymous_base_type - "
                             "anonymous base type codegen failed\n"),
                            -1);
        }

      delete visitor;
    }

  return 0;
}
