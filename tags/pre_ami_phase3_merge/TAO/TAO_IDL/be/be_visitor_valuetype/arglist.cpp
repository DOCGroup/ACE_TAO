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
//    Visitor generating the parameter list of operations
//    in the Valuetype class.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, arglist, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_obv_operation_arglist::be_visitor_obv_operation_arglist (be_visitor_context
                                                            *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_obv_operation_arglist::~be_visitor_obv_operation_arglist (void)
{
}

int
be_visitor_obv_operation_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << " (";
  // *os << " (" << be_idt << be_idt << "\n";

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_operation_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }


  // *os << be_uidt;
  // os->indent ();
  *os << ")";// << be_uidt;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH:
      // each method is pure virtual in the Valuetype class
      *os << " = 0;\n";
      break;
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IH:
      break;
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IS:
      break;
    default:
       *os << "\n";
    }
  if (!this->ctx_->attribute ())    // hack to get a nice newline
    *os << "\n";
  return 0;
}

int
be_visitor_obv_operation_arglist::visit_argument (be_argument *node)
{
  // TAO_OutStream *os = this->ctx_->stream ();

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
  // %! use AST_Interface
  be_valuetype *intf;
  intf = this->ctx_->attribute ()
    ? be_valuetype::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_valuetype::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  //  snipped from
  //  be_visitor_args_arglist::visit_argument (be_argument *node)
  ctx.node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // os->indent (); // start with current indentation level

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  // end of be_visitor_args_arglist::visit_argument ()

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_OTHERS:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IS:
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
  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "codegen for argument failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}

int
be_visitor_obv_operation_arglist::post_process (be_decl *bd)
// derived from be_visitor_operation_argument::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH:
        case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_OTHERS:
        case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_SH:
        case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IH:
        case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IS:
          if (!this->last_node (bd))
            *os << ", "; // "\n";
          else
            *os << "";  // "\n";
          break;
        default:
          break;
        }

  return 0;
}
