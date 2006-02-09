// $Id$

ACE_RCSID (be_visitor_operation,
           proxy_impl_xh,
           "$Id$")

be_visitor_operation_proxy_impl_xh::be_visitor_operation_proxy_impl_xh (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
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

  *os << "static void" << be_nl;

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "_set_";
        }
      else
        {
          *os << "_get_";
        }
    }

  *os << node->local_name () << " (" << be_idt << be_idt_nl
      << "TAO_Abstract_ServantBase *servant," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args" << env_decl << be_uidt_nl
      << ")";

  if (this->gen_throw_spec (node) != 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_operation_proxy_impl_xh::"
          "visit_operation - "
          "throw spec generation failed\n"
        ),
        -1
      );
    }

  *os << ";";

  return 0;
}
