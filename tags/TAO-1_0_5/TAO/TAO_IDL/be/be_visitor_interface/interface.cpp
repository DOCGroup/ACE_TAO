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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_TIE_SH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CS);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
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
                           "Bad context state\n"),
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
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_IH:
    case TAO_CodeGen::TAO_INTERFACE_IS:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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

  // Change the state depending on the kind of interface
  // we are defined in.
  be_interface *parent = be_interface::narrow_from_scope (node->defined_in ());
  this->ctx_->state (parent->next_state (this->ctx_->state ()));

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
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CH);
      break;
   case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
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
  //
  // AMI Call back code generation.
  //
#if 0
  // Only if AMI callbacks are enabled.
  if (idl_global->ami_call_back () == I_TRUE)
    {
  
      // Generate AMI <sendc_> method, for this operation, if you are
      // doing client header.

    if (this->ctx_->state () == TAO_CodeGen::TAO_INTERFACE_CH ||
        this->ctx_->state () == TAO_CodeGen::TAO_INTERFACE_CS)
      {
        switch (this->ctx_->state ())
          {
          case TAO_CodeGen::TAO_INTERFACE_CH:
            ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CH);
            break;
          case TAO_CodeGen::TAO_INTERFACE_CS:
            ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CS);
            break;
          default:
            // We dont have to do anything for the other cases.
            return 0;
          }

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

        // Visit the node using this visitor
        if (node->accept (visitor) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_interface::"
                               "visit_operation - "
                               "failed to accept visitor\n"),
                              -1);
          }
        delete visitor;
      }
    else
      {
        switch (this->ctx_->state ())
          {
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
          case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
            {
              // Create new be_operation

              // Create the return type, which is "void"
              be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                               new UTL_ScopedName
                                                                 (new Identifier ("void", 1, 0, I_FALSE), 
                                                                  0),
                                                               0);

              // Create the argument

              // Create the field type
              // Get the name of the interface
              // Get the scope name.
              be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
              ACE_CString interface_name (parent
                                            ->name ()
                                              ->last_component ()
                                                ->get_string ());
              // Add the pre- and suffix
              ACE_CString excep_holder_name ("AMI_");
              excep_holder_name += interface_name;
              excep_holder_name += "ExceptionHolder";
              UTL_ScopedName *field_name = (UTL_ScopedName *)parent->name ()->copy ();
              field_name->last_component ()->replace_string (excep_holder_name.rep ());
              be_interface *field_type= new be_interface (field_name,
                                                         0,
                                                         0,
                                                         0);
              field_type->set_name (field_name);
              field_type->set_defined_in (node->defined_in ());

              // Create the argument
              be_argument *arg = new be_argument (AST_Argument::dir_OUT,
                                                  field_type, // is also a valuetype
                                                  new UTL_ScopedName 
                                                  (new Identifier
                                                      ("excep_holder", 1, 0, I_FALSE),
                                                    0),
                                                  0);

              // Create the new name
              // Append _execp to the name of the operation
              ACE_CString original_op_name (node
                                              ->name ()
                                                ->last_component ()
                                                  ->get_string ());
              ACE_CString new_op_name = original_op_name + ACE_CString ("_excep");

              UTL_ScopedName *op_name = (UTL_ScopedName *)node->name ()-> copy ();
              op_name->last_component ()->replace_string (new_op_name.rep ());

              // create the operation
              be_operation * op = new be_operation (rt, 
                                                    AST_Operation::OP_noflags,
                                                    op_name, 
                                                    0);
              op->set_defined_in (node->defined_in ());
              op->add_argument_to_scope (arg);

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

              // Visit the node using this visitor
              if (op->accept (visitor) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_interface::"
                                     "visit_operation - "
                                     "failed to accept visitor\n"),
                                    -1);
                }
              delete visitor;
              delete op;
            }
            break;
          default:
            return 0;
          }
      }
    }
#endif /* 0 */
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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_TIE_SH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CH:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CI:
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS:
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
