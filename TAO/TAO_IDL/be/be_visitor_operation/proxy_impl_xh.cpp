// $Id$

ACE_RCSID (be_visitor_operation, 
           proxy_impl_xh, 
           "$Id$")

be_visitor_operation_proxy_impl_xh::be_visitor_operation_proxy_impl_xh (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_proxy_impl_xh::~be_visitor_operation_proxy_impl_xh (void)
{
}

int be_visitor_operation_proxy_impl_xh::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "static void" << be_nl
      << node->local_name () << " (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Object_out obj_forward," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));" << be_uidt;

  return 0;
}
