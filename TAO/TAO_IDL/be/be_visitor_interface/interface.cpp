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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface, "$Id$")

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

// this method must be overridden by the derived interface visitors
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
    // and the name starts with AMH
    if (ACE_OS::strncmp (node->local_name (), "AMH", 3) == 0)
      // then it is an AMH node
      return 1;
  
  return 0;
}

// =all common visit methods for interface visitor

// visit an attribute
int
be_visitor_interface::visit_attribute (be_attribute *node)
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
                           "Bad context state\n: " ),
                          -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a constant
int
be_visitor_interface::visit_constant (be_constant *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
      break;
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
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_interface::visit_enum (be_enum *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CS);
      break;

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

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an exception
int
be_visitor_interface::visit_exception (be_exception *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
      break;
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

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an operation
int
be_visitor_interface::visit_operation (be_operation *node)
{

  // instantiate a visitor context with a copy of our context. This info
  // will be modified ased on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
  // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_IH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_IS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
      break;

      // AMH stuff
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_AMH_RH_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_AMH_RH_SS);
      break;

    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
      break;

    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
      break;

    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
      break;

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

  // Change the state depending on the kind of node strategy
  ctx.state (node->next_state (ctx.state ()));


  // grab the appropriate visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // visit the node using this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  visitor = 0;

  // Do additional code generation is necessary.
  // Note, this call is delegated to the strategy connected to
  // the node.
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy
      ctx.state (node->next_state (ctx.state (), 1));

      // grab the appropriate visitor
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface::"
                             "visit_operation - "
                             "NUL visitor\n"
                             ),  -1);
        }

      // visit the node using this visitor
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface::"
                             "visit_operation - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      delete visitor;
      visitor = 0;
    }

  return 0;
}

// Visit an structure.
int
be_visitor_interface::visit_structure (be_structure *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
      break;
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
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a union
int
be_visitor_interface::visit_union (be_union *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
      break;
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
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a typedef
int
be_visitor_interface::visit_typedef (be_typedef *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
      break;
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
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}













