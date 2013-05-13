
//=============================================================================
/**
 *  @file    exception.cpp
 *
 *  $Id$
 *
 *  Generic visitor generating code for Exceptions.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "exception.h"

// Generic exception visitor.
be_visitor_exception::be_visitor_exception (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception::~be_visitor_exception (void)
{
}

// Visit the Exception node and its scope.
int
be_visitor_exception::visit_exception (be_exception *)
{
  // Must be overriden.
  return -1;
}

int
be_visitor_exception::visit_field (be_field *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  // This switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        be_visitor_field_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_field_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_field_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_field_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_field_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_exception::"
                           "visit_field - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::"
                         "visit_field - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}
