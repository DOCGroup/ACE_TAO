#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, base_proxy_broker_cs, "$Id$")

be_visitor_interface_remote_proxy_broker_cs::be_visitor_interface_remote_proxy_broker_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_broker_cs::~be_visitor_interface_remote_proxy_broker_cs (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_broker_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the class declaration.
  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//           Remote & Base  Proxy Broker Implementation" << be_nl
      << "//" << be_nl << be_nl;

  *os << node->full_base_proxy_broker_name () << "::~"
      << node->base_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  *os << "// Factory function Implementation." << be_nl
      << node->full_remote_proxy_broker_name ()
      << " *" << node->client_enclosing_scope () << "the"
      << node->remote_proxy_broker_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl
      << "static ::" << node->full_remote_proxy_broker_name ()
      << " remote_proxy_broker;" << be_nl
      << "return &remote_proxy_broker;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_remote_proxy_broker_name () << "::~"
      << node->remote_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  *os << node->full_base_proxy_impl_name () << "&" << be_nl
      <<node->full_remote_proxy_broker_name () << "::"
      << "select_proxy (" << be_idt_nl
      << "::" << node->full_name () << " *object," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl << ")"
      << be_uidt_nl << "{" << be_idt_nl // idt = 1
      << "ACE_UNUSED_ARG (object);" << be_nl
      << "ACE_UNUSED_ARG (ACE_TRY_ENV);" << be_nl
      << "return remote_proxy_impl_;"
      << be_uidt_nl << "}" << be_nl << be_nl;


   *os << be_nl
      << "//" << be_nl
      << "//           End Remote & Base Proxy Broker Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;
  return 0;
}
