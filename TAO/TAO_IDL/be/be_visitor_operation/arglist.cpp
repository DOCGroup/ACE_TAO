
//=============================================================================
/**
 *  @file    arglist.cpp
 *
 *  Visitor generating code for the parameter list of the Operation signature.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "operation.h"

// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_arglist::be_visitor_operation_arglist (
    be_visitor_context *ctx)
  : be_visitor_operation (ctx),
    unused_ (false)
{
}

be_visitor_operation_arglist::~be_visitor_operation_arglist ()
{
}

int
be_visitor_operation_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  bool const has_args = node->argument_count () > 0;

  *os << " (";

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS:
      *os << be_idt_nl << "::CORBA::Object *_collocated_tao_target_";

      if (has_args)
        {
          *os << "," << be_nl;
        }

      break;
    default:
      if (has_args)
        {
          *os << be_idt_nl;
        }
      else
        {
          *os << be_idt;
        }
      break;
    }

  // All we do is hand over code generation to our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << ")" << be_uidt;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
      if (node->is_local ())
        {
          *os << " = 0";
        }

      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_TIE_OPERATION_ARGLIST_SH:
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      *os << " = 0";
      break;
    default:
      return 0;
    }

  *os << ";";

  return 0;
}

int
be_visitor_operation_arglist::visit_factory (be_factory *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  bool has_args = node->argument_count () > 0;

  *os << " (";

  if (has_args)
    {
      *os << be_idt_nl;
    }

  // All we do is hand over code generation to our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_arglist::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  if (!has_args)
    {
      *os << "void";
    }

  *os << ")";

  if (has_args)
    {
      *os << be_uidt;
    }

  // At present, visit_factory() is called only from the home
  // servant source visitor, so we don't need to check the state
  // for semicolon generation.

  return 0;
}

int
be_visitor_operation_arglist::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // First grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op =
    dynamic_cast<be_operation*> (this->ctx_->scope ());
  be_interface *intf = nullptr;

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  if (op == nullptr)
    {
      be_factory *f =
        dynamic_cast<be_factory*> (this->ctx_->scope ());

      intf = dynamic_cast<be_interface*> (f->defined_in ());
    }
  else
    {
      intf = this->ctx_->attribute ()
        ? dynamic_cast<be_interface*> (this->ctx_->attribute ()->defined_in ())
        : dynamic_cast<be_interface*> (op->defined_in ());
    }

  // Set new scope.
  ctx.scope (intf);

  // Create a visitor.
  be_visitor_args_arglist visitor (&ctx);
  visitor.unused (unused_);

  if (visitor.visit_argument (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }

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
      *os << "," << be_nl;
    }

  return 0;
}

void
be_visitor_operation_arglist::unused (bool val)
{
  unused_ = val;
}
