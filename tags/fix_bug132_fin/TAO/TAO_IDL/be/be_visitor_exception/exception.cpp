//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    .cpp
//
// = DESCRIPTION
//    Generic visitor generating code for Exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, exception, "$Id$")


// generic struct visitor
be_visitor_exception::be_visitor_exception (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception::~be_visitor_exception (void)
{
}

// visit the Exception node and its scope
int
be_visitor_exception::visit_exception (be_exception *)
{
  return -1; // must be overriden
}

int
be_visitor_exception::visit_field (be_field *node)
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
    case TAO_CodeGen::TAO_EXCEPTION_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CH);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CI:
      ctx.state (TAO_CodeGen::TAO_FIELD_CI);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CS);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_FIELD_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_exception::"
                           "visit_field - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::"
                         "visit_field - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::"
                         "visit_field - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}
