//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces. This is a generic visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           interface, 
           "$Id$")

// ******************************************************
// Generic Interface visitor
// ******************************************************

be_visitor_interface::be_visitor_interface (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_interface::~be_visitor_interface (void)
{
}

// This method must be overridden by the derived interface visitors.
int
be_visitor_interface::visit_interface (be_interface *)
{
  return -1;
}

int
be_visitor_interface::is_amh_rh_node (be_interface *node)
{
   //If, is implied-IDL
  if (node->original_interface () != 0) 
  {
    // and the name starts with AMH
    if (ACE_OS::strncmp (node->local_name (), "AMH", 3) == 0)
      {
        // then it is an AMH node.
        return 1;
      }
  }
  return 0;
}

// All common visit methods for interface visitor.

int
be_visitor_interface::visit_attribute (be_attribute *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_IH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_IH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_IS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_IS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS);
      break;

      // AMH stuff
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
		ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SH);
		break;
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SS);
		break;

    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_attribute - "
                           "Bad context state\n: "),
                          -1);
      }
    }

  // Same visitor for all the above cases where an action is taken.
  be_visitor_attribute visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_constant (be_constant *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
        be_visitor_constant_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
        be_visitor_constant_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		  return 0; // nothing to be done

	default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_constant - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_enum (be_enum *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CH);
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CS);
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
        be_visitor_enum_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }

    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		return 0; // nothing to be done

    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

// visit an exception
int
be_visitor_interface::visit_exception (be_exception *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
        be_visitor_exception_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
        be_visitor_exception_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
        be_visitor_exception_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
        be_visitor_exception_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
        be_visitor_exception_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
        be_visitor_exception_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
        be_visitor_exception_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
        be_visitor_exception_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		  return 0; // nothing to be done

    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

// visit an operation
int
be_visitor_interface::visit_operation (be_operation *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
        be_visitor_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_IH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
        be_visitor_operation_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
        be_visitor_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_IS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
        be_visitor_operation_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
        be_visitor_operation_tie_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
        be_visitor_operation_tie_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
        be_visitor_operation_thru_poa_collocated_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
        be_visitor_operation_thru_poa_collocated_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
        be_visitor_operation_direct_collocated_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
        be_visitor_operation_direct_collocated_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
        be_visitor_operation_interceptors_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
        be_visitor_operation_interceptors_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
        be_visitor_operation_interceptors_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
        be_visitor_operation_interceptors_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
        be_visitor_operation_base_proxy_impl_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_AMH_RH_SH);
        be_visitor_amh_rh_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_AMH_RH_SS);
        be_visitor_amh_rh_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
        be_visitor_operation_remote_proxy_impl_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
        be_visitor_operation_thru_poa_proxy_impl_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
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
                         "(%N:%l) be_visitor_interface::"
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "NUL visitor\n"),  
                         -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
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
                             "(%N:%l) be_visitor_interface::"
                             "visit_operation - "
                             "NUL visitor\n"),  
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface::"
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
be_visitor_interface::visit_structure (be_structure *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_structure - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_union (be_union *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CH);
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CI);
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CS);
        be_visitor_union_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
        be_visitor_union_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		  return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_union - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_typedef (be_typedef *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CS:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SH:
    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_SS:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:

    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
		  return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_typedef - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}













