//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_arglist.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the Operation signature.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, interceptors_arglist, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_interceptors_arglist::
be_visitor_operation_interceptors_arglist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_arglist::~be_visitor_operation_interceptors_arglist (void)
{
}

int
be_visitor_operation_interceptors_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // generate the TAO_ENV_ARG_PARAMETER for the alternative mapping
  if (!be_global->exception_support ())
    {
      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS:
          //        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_RESULT:
          break;
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS:
          {
            *os << " TAO_ENV_ARG_PARAMETER";
            break;
          }
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS:
          {
           // last argument - is always CORBA::Environment
            *os << " TAO_ENV_ARG_DECL_NOT_USED";
            break;
          }
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH:
        case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH:
            {
              // @@ Do it for all cases i.e arg count > = 0

              // last argument - is always CORBA::Environment
              *os << " TAO_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl;
              break;
            }
        default:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_interceptors_arglist::"
                               "visit_operation - "
                               "Bad context\n"),
                              -1);
          }
        }
    }

  return 0;
}

int
be_visitor_operation_interceptors_arglist::pre_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_compiled_visitor_operation_argument_invoke"
                         "::post_process - "
                         "Bad argument node\n"),
                        -1);
    }

  if (arg->direction () == AST_Argument::dir_OUT)
    return 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS:
      {
        // If we are not the last node in the list of arguments, generate a comma
        // else decide if we are generating code to support true exceptions - in
        // which case there will not be any CORBA::Environment parameter.
        *os << "," << be_nl;
        break;
      }
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH:
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_arglist::"
                           "post_process - "
                           "Bad context\n"),
                          -1);
      }

    }

  return 0;
}

int
be_visitor_operation_interceptors_arglist::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // First grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();

  if (op == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_arglist::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We don't generate the "out" args as part of arglist
  if (node->direction () == AST_Argument::dir_OUT)
    {
      return 0;
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context.
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }

  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_PARAMLIST);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_arglist::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // Grab a visitor.
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_arglist::"
                         "visit_argument - "
                         "codegen for interceptors_arglist failed\n"),
                        -1);
    }

  delete visitor;
  return 0;
}

int
be_visitor_operation_interceptors_arglist::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_compiled_visitor_operation_argument_invoke"
                         "::post_process - "
                                   "Bad argument node\n"),
                        -1);
    }

  if (arg->direction () != AST_Argument::dir_OUT)
    return 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS:
      {
        if (!this->last_node (bd))
          {
            os->indent ();
            *os << "\n";
          }
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_arglist::"
                           "post_process - "
                           "Bad context\n"),
                          -1);
      }

    }
  return 0;
}
