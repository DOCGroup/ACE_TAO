#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, proxy_brokers_ch, "$Id$")

  be_visitor_interface_proxy_brokers_ch::be_visitor_interface_proxy_brokers_ch (be_visitor_context *ctx)
    : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_proxy_brokers_ch::~be_visitor_interface_proxy_brokers_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_proxy_brokers_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate Guards.
  //  os->gen_ifdef_macro (node->local_name (), "PROXY_BROKERS");

  // Generate the class declaration.
  os->indent ();
  *os << "\n// The Proxy Brokers are used by each interface to get\n"
      << "// the right proxy for performing a call. In the new \n"
      << "// collocation scheme, the proxy to be used can vary on\n"
      << "// a call by call basis. \n\n\n";

  // Code Generation for the proxy brokers base class.
  be_visitor *visitor = 0;
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }
  delete visitor;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH);
  visitor = tao_cg->make_visitor (&ctx);

    if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for Remote Proxy Broker class failed\n"),
                        -1);
    }
  delete visitor;

  // Close #ifdef Guard.
  //  os->gen_endif ();

  return 0;
}
