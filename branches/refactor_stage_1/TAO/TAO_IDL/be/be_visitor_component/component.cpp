//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components. This is a generic visitor.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component, 
           component, 
           "$Id$")

// ******************************************************
// Generic Component visitor
// ******************************************************

be_visitor_component::be_visitor_component (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_component::~be_visitor_component (void)
{
}

int
be_visitor_component::visit_attribute (be_attribute *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      break;
    case TAO_CodeGen::TAO_ROOT_IH:
      break;
    case TAO_CodeGen::TAO_ROOT_SS:
      break;
    case TAO_CodeGen::TAO_ROOT_IS:
      break;
    default:
      return 0; // nothing to be done
    }

  // Same visitor for all the above cases where an action is taken.
  be_visitor_attribute visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_attribute - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_operation (be_operation *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified ased on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These first two cases may have the context state changed
    // by a strategy, so we use the visitor factory below.
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_component::"
                           "visit_operation - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_operation - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  // Change the state depending on the kind of node strategy.
  ctx.state (node->next_state (ctx.state ()));

  // Grab the appropriate visitor.
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_operation - "
                         "NUL visitor\n"),  
                         -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_operation - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  delete visitor;
  visitor = 0;

  // Do additional code generation is necessary.
  // Note, this call is delegated to the strategy connected to
  // the node.
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy.
      ctx.state (node->next_state (ctx.state (), 1));

      // Grab the appropriate visitor.
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_component::"
                             "visit_operation - "
                             "NUL visitor\n"),  
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_component::"
                             "visit_operation - "
                             "failed to accept visitor\n"),  
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  return 0;
}

int
be_visitor_component::visit_structure (be_structure *node)
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
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_structure - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_typedef (be_typedef *node)
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
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
		  return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_component::"
                           "visit_typedef - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component::"
                         "visit_typedef - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

