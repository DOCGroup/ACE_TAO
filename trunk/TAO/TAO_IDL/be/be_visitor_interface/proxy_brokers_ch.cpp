//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           proxy_brokers_ch, 
           "$Id$")

be_visitor_interface_proxy_brokers_ch::be_visitor_interface_proxy_brokers_ch (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_proxy_brokers_ch::~be_visitor_interface_proxy_brokers_ch (
    void
  )
{
  // No-Op.
}

int
be_visitor_interface_proxy_brokers_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate Guards.

  // Generate the class declaration.
  *os << "// The Proxy Brokers are used by each interface to get" << be_nl
      << "// the right proxy for performing a call. In the new " << be_nl
      << "// collocation scheme, the proxy to be used can vary on" << be_nl
      << "// a call by call basis." << be_nl << be_nl;

  // Code Generation for the proxy brokers base class.
  be_visitor *visitor = 0;
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH);
  be_visitor_interface_base_proxy_broker_ch bpb_visitor (&ctx);

  if (node->accept (&bpb_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH);
  be_visitor_interface_remote_proxy_broker_ch rpb_visitor (&ctx);

  if (node->accept (&rpb_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for Remote Proxy Broker class failed\n"),
                        -1);
    }

  return 0;
}
