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

ACE_RCSID (be_visitor_operation, 
           arglist, 
           "$Id$")

// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_arglist::be_visitor_operation_arglist (
    be_visitor_context *ctx
  )
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

  *os << " (" << be_idt
      << be_idt_nl;

  int arg_emitted = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS:
      *os << "CORBA_Object *_collocated_tao_target_";

      if (node->argument_count () > 0)
        {
          *os << "," << be_nl;
        }

      arg_emitted = 1;
      break;
    default:
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

  if (this->gen_environment_decl (arg_emitted, node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_arglist::"
                         "visit_operation - "
                         "gen_environment_decl failed\n"),
                        -1);
    }

  *os << be_uidt_nl // idt = 1
      << ")" << be_uidt; // idt = 0

  // Now generate the throw specs.
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_operation_arglist")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("Failed to generate throw spec\n")),
                        -1);
    }

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
      if (node->is_local ())
        {
          *os << " = 0;" << be_nl << be_nl;
        }
      else
        {
          *os << ";" << be_nl << be_nl;
        }

      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH:
      *os << ";" << be_nl << be_nl;
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // Each method is pure virtual in the server header.
      *os << " = 0;" << be_nl << be_nl;
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS:
    default:
      *os << be_nl;
    }

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

  // Set new scope.
  ctx.scope (intf);

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH:
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

  // Create a visitor.
  be_visitor_args_arglist visitor (&ctx);

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
