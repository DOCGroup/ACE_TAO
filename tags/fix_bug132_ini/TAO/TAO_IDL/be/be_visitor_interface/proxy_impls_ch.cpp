#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, base_proxy_broker_impl_ch, "$Id$")

be_visitor_interface_proxy_impls_ch::be_visitor_interface_proxy_impls_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_proxy_impls_ch::~be_visitor_interface_proxy_impls_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_proxy_impls_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->indent ();
  // Generate Guards.
  //  os->gen_ifdef_macro (node->local_name (), "PROXY_IMPLS");

  *os << "\n// The Proxy Implementations are used by each interface to\n"
      << "// perform a call. Each different implementation encapsulate\n"
      << "// an invocation logics.\n\n";

  // Code Generation for the proxy imlpementations base class.
  be_visitor *visitor = 0;
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for Base Proxy Impl. class failed\n"),
                        -1);
    }
  delete visitor;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH);
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
