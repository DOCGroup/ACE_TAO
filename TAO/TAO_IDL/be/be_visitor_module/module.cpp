//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module.cpp
//
// = DESCRIPTION
//    Generic visitor generating code for Module
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_module, 
           module, 
           "$Id$")


// ******************************************************
// Generic Module visitor
// ******************************************************

be_visitor_module::be_visitor_module (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_module::~be_visitor_module (void)
{
}

int be_visitor_module::visit_module (be_module *node)
{
  if (node->nmembers () == 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_EMPTY_MODULE,
                                  node);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::visit_module - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}

// All common visit methods for module visitor.

int
be_visitor_module::visit_constant (be_constant *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
        be_visitor_constant_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
        be_visitor_constant_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_constant - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_constant - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_enum (be_enum *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CH);
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CS);
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
        be_visitor_enum_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_enum - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_enum - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_exception (be_exception *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
        be_visitor_exception_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
        be_visitor_exception_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
        be_visitor_exception_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
        be_visitor_exception_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
        be_visitor_exception_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
        be_visitor_exception_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
        be_visitor_exception_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
        be_visitor_exception_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_exception - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_interface (be_interface *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      // This is the only context state involved in strategies.
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CI);
        be_visitor_interface_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CS);
        be_visitor_interface_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SH);
        be_visitor_interface_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_IH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_IH);
        be_visitor_interface_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SI);
        be_visitor_interface_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SS);
        be_visitor_interface_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_IS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_IS);
        be_visitor_interface_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH);
        be_visitor_interface_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS);
        be_visitor_interface_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH);
        be_visitor_interface_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI);
        be_visitor_interface_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS);
        be_visitor_interface_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface - "
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
                         "(%N:%l) be_visitor_module::"
                         "visit_interface - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  // Change the state depending on the kind of node strategy
  ctx.state (node->next_state (ctx.state ()));

  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface - "
                         "NUL visitor\n"),  
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface - "
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

      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module::"
                             "visit_interface - "
                             "NUL visitor\n"),  
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module::"
                             "visit_interface - "
                             "failed to accept visitor\n"),  
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  return 0;
}

int
be_visitor_module::visit_interface_fwd (be_interface_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CH);
        be_visitor_interface_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CI);
        be_visitor_interface_fwd_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CI);
        be_visitor_interface_fwd_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_ANY_OP_CH);
        be_visitor_interface_fwd_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CH);
        be_visitor_interface_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_valuetype (be_valuetype *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CH);
        be_visitor_valuetype_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CI);
        be_visitor_valuetype_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CS);
        be_visitor_valuetype_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH);
        be_visitor_valuetype_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI);
        be_visitor_valuetype_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS);
        be_visitor_valuetype_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH);
        be_visitor_valuetype_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS);
        be_visitor_valuetype_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0;    // nothing to do.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_valuetype - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CH);
        be_visitor_valuetype_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CI);
        be_visitor_valuetype_fwd_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CH);
        be_visitor_valuetype_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CI);
        be_visitor_valuetype_fwd_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_valuetype_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_valuetype_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_structure (be_structure *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_structure - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_structure - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_structure_fwd (be_structure_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_FWD_CH);
        be_visitor_structure_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_structure_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_structure_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_union (be_union *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CH);
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CI);
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CS);
        be_visitor_union_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
        be_visitor_union_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_union - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_union - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_union_fwd (be_union_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
  {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_FWD_CH);
        be_visitor_union_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_union_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
  }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_union_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_typedef (be_typedef *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_typedef - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_typedef - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}
