
//=============================================================================
/**
 *  @file    root.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Root node. This is a generic visitor.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "root.h"

// Generic Root visitor
be_visitor_root::be_visitor_root (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_root::~be_visitor_root (void)
{
}

int
be_visitor_root::visit_module (be_module *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_module_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_module_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_module_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_module_any_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_module_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_module - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}
