//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    argument.cpp
//
// = DESCRIPTION
//    Visitor that calls the visitor for arguments.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, argument, "$Id$")


// ************************************************************
// generic operation visitor to handle the pre/post
// do_static_call/upcall stuff with arguments
// ************************************************************

be_visitor_operation_argument::
be_visitor_operation_argument (be_visitor_context
                               *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_argument::~be_visitor_operation_argument (void)
{
}

int
be_visitor_operation_argument::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();


  // if we are not the last parameter, we insert a comma. This is only
  // applicable for the upcalls or the call to (de)marshal that we use in the
  // interpreted marshaling.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
        if (!this->last_node (bd))
          *os << ",\n";
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_COLLOCATED_ARG_UPCALL_CS:          
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_UPCALL_CS:          
      if (!this->last_inout_or_out_node (bd))
        {
          // @@ Michael
          // In the case of of AMI, we do not want to 
          // print a comma for in arguments.
          // This is due to the concept of post processing,
          // which I was not able to prevent to be executed
          // in the case of in arguments. Post processing
          // does always write a comma, though.
          be_argument *arg = this->ctx_->be_node_as_argument ();
          ACE_ASSERT (arg != 0);
          if (arg->direction () != AST_Argument::dir_IN)
            *os << ",\n";
        }
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_operation_argument::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_argument::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // if we are supporting the alternate mapping, we must pass the
  // CORBA::Environment parameter as the last parameter
  if (!idl_global->exception_support ())
    {
      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
        case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
          // applicable only to these cases where the actual upcall is made 

          if (node->argument_count () > 0)
            {
              // insert a comma only if there were previous parameters
              *os << ",\n";
            }

          // last argument is the environment
          os->indent ();
          *os << "ACE_TRY_ENV";
          break;
        case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_COLLOCATED_ARG_UPCALL_CS:
        case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_UPCALL_CS:
          // applicable only to these cases where the actual upcall is made 
          if (this->ctx_->sub_state () == TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_HAS_ARGUMENTS)
            {
              // insert a comma only if there were previous parameters
              *os << ",\n";
            }
          // last argument is the environment
          os->indent ();
          *os << "ACE_TRY_ENV";
          break;
        default:
          break;
        }
    }

  return 0;
}

int
be_visitor_operation_argument::visit_argument (be_argument *node)
{
  // get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // first grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_INVOKE_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_PRE_INVOKE_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INVOKE_CS);
      break;
    case TAO_CodeGen::TAO_AMI_OPERATION_ARG_INVOKE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_ARGUMENT_INVOKE_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_INVOKE_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_DECL_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_VARDECL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_DEMARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_DEMARSHAL_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_DEMARSHAL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_MARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_MARSHAL_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_MARSHAL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_COLLOCATED_ARG_UPCALL_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_COLLOCATED_UPCALL_CS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_UPCALL_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_UPCALL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_MARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_MARSHAL_SS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_argument::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // grab a visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_argument::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_argument::"

                         "visit_argument - "
                         "codegen for argument failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}
