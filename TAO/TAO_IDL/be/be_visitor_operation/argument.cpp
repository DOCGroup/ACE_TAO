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
        {
          *os << "," << be_nl;
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
  // TAO_ENV_ARG_PARAMETER as the last parameter
  if (!be_global->exception_support ())
    {
      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
        case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
          // applicable only to these cases where the actual upcall is made

          // Use TAO_ENV_SINGLE_ARG_DECL or TAO_ENV_ARG_DECL depending on
          // whether the operation node has parameters.
          if (node->argument_count () > 0)
            *os << " TAO_ENV_ARG_PARAMETER";
          else
            *os << "TAO_ENV_SINGLE_ARG_PARAMETER";
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
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_INVOKE_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_DEMARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_MARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS);
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
