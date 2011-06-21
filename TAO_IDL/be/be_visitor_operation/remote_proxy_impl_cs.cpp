// $Id$

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_remote_proxy_impl_cs::
be_visitor_operation_remote_proxy_impl_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_remote_proxy_impl_cs::
~be_visitor_operation_remote_proxy_impl_cs (void)
{
}

// Processing to be done after every element in the scope is processed.
int
be_visitor_operation_remote_proxy_impl_cs::post_process (be_decl *bd)
{
  // All we do here is to insert a comma and a newline.
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}

int
be_visitor_operation_remote_proxy_impl_cs::visit_operation (
    be_operation *node
  )
{
  if (node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype ort_visitor (&ctx);

  if (bt->accept (&ort_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // Generate the operation name
  be_decl* scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  be_interface *interface = be_interface::narrow_from_decl (scope);

  *os <<" " << interface->full_remote_proxy_impl_name ()
      << "::" << node->local_name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file).
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS);
  be_visitor_operation_arglist arglist_visitor (&ctx);

  if (node->accept (&arglist_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Generate the code for the operation body.
  return this->gen_stub_operation_body (node,
                                        bt);
}

int
be_visitor_operation_remote_proxy_impl_cs::visit_argument (be_argument *node)
{
  // This method is used to generate the ParamData table entry.

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  // Retrieve the type for this argument.
  bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";

  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}

