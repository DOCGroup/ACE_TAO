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
    case TAO_CodeGen::TAO_COMPONENT_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CH);
      break;
    case TAO_CodeGen::TAO_COMPONENT_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CS);
      break;
    case TAO_CodeGen::TAO_COMPONENT_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SH);
      break;
    case TAO_CodeGen::TAO_COMPONENT_IH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_IH);
      break;
    case TAO_CodeGen::TAO_COMPONENT_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SS);
      break;
    case TAO_CodeGen::TAO_COMPONENT_IS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_IS);
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
    case TAO_CodeGen::TAO_COMPONENT_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_COMPONENT_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_COMPONENT_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
        be_visitor_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_IH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
        be_visitor_operation_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
        be_visitor_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_IS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
        be_visitor_operation_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CH:
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CS:
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CH:
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CI:
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CS:
    case TAO_CodeGen::TAO_COMPONENT_CI:
    case TAO_CodeGen::TAO_COMPONENT_SI:
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
    case TAO_CodeGen::TAO_COMPONENT_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_SH:
    case TAO_CodeGen::TAO_COMPONENT_IH:
    case TAO_CodeGen::TAO_COMPONENT_IS:
    case TAO_CodeGen::TAO_COMPONENT_SI:
    case TAO_CodeGen::TAO_COMPONENT_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_component::"
                           "visit_structure - "
                           "Bad context state\n"), 
                          -1);
      }
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
    case TAO_CodeGen::TAO_COMPONENT_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_COMPONENT_SH:
    case TAO_CodeGen::TAO_COMPONENT_IH:
    case TAO_CodeGen::TAO_COMPONENT_IS:
    case TAO_CodeGen::TAO_COMPONENT_SI:
    case TAO_CodeGen::TAO_COMPONENT_SS:
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

