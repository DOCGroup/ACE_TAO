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
#include "be_visitor_sequence.h"
#include "be_visitor_context.h"
#include "be_typedef.h"
#include "be_type.h"
#include "ace/Log_Msg.h"

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

      int status = 0;

      switch (cg_state)
      {
        case TAO_CodeGen::TAO_ROOT_CH:
          {
            be_visitor_sequence_ch visitor (&ctx);
            status = bt->accept (&visitor);
            break;
          }
        case TAO_CodeGen::TAO_ROOT_CI:
          {
            be_visitor_sequence_ci visitor (&ctx);
            status = bt->accept (&visitor);
            break;
          }
        case TAO_CodeGen::TAO_ROOT_CS:
          {
            be_visitor_sequence_cs visitor (&ctx);
            status = bt->accept (&visitor);
            break;
          }
        case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
          {
            be_visitor_sequence_cdr_op_cs visitor (&ctx);
            status = bt->accept (&visitor);
            break;
          }
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_decl::"
                             "gen_anonymous_base_type - "
                             "bad context state\n"),
                            -1);
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_decl::"
                             "gen_anonymous_base_type - "
                             "anonymous base type codegen failed\n"),
                            -1);
        }
    }

  return 0;
}
