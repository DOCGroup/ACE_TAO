//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    arglist.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the Operation signature.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, arglist, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_arglist::
be_visitor_operation_arglist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_arglist::~be_visitor_operation_arglist (void)
{
}

int
be_visitor_operation_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << " (" << be_idt << be_idt << "\n";

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // generate the CORBA::Environment parameter for the alternative mapping 
  if (!idl_global->exception_support ())
    {
      // if the operation node has parameters, then we need to insert a comma
      if (node->argument_count () > 0)
        *os << ",\n";

      os->indent ();
      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
        case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
        case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
          // last argument - is always CORBA::Environment
          *os << "CORBA::Environment &ACE_TRY_ENV";
          *os << " = " << be_idt_nl
              << "TAO_default_environment ()"
              << be_uidt;
          break;
        case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
        case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
          // last argument - is always CORBA::Environment
          *os << "CORBA::Environment &ACE_TRY_ENV";
          break;
        default:
          *os << "CORBA::Environment &ACE_TRY_ENV";
          break;
        }
    }
  *os << be_uidt_nl << ")" << be_uidt;

  // now generate the throw specs
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_visitor_operation_arglist")
                         ASYS_TEXT ("::visit_operation - ")
                         ASYS_TEXT ("Failed to generate throw spec\n")),
                        -1);
    }

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
      *os << ";\n\n";
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // each method is pure virtual in the server header
      *os << " = 0;\n\n";
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    default:
      *os << "\n";
    }

  return 0;
}

int
be_visitor_operation_arglist::visit_argument (be_argument *node)
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
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_OTHERS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_arglist::"
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
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"

                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}

int
be_visitor_operation_arglist::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // if we are not the last node in the list of arguments, generate a comma
  // else decide if we are generating code to support true exceptions - in
  // which case there will not be any CORBA::Environment parameter
  if (!this->last_node (bd))
    {
      *os << ",\n";
    }
  return 0;
}

