//$Id$

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, base_proxy_broker_ch, "$Id$")

be_visitor_interface_base_proxy_broker_ch::be_visitor_interface_base_proxy_broker_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_base_proxy_broker_ch::~be_visitor_interface_base_proxy_broker_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_base_proxy_broker_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//                 Base Proxy Broker Declaration " << be_nl
      << "//" << be_nl << be_nl;

  // Generate the class declaration.
  os->indent ();
  *os << "class " << be_global->stub_export_macro () << " "
      << node->base_proxy_broker_name () << be_nl <<  "{" << be_nl
      << "public:"
      << be_idt_nl; // idt = 1



  // Destructor
  *os << "virtual ~" << node->base_proxy_broker_name () << " (void);" << be_nl;

  // Accessor Method
  *os << "virtual " << node->base_proxy_impl_name () << " &"
      << "select_proxy ("
      << be_idt << be_idt_nl;

  *os << node->local_name () << " *object" << be_nl
      << "TAO_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ") = 0;"
      << be_uidt_nl  // idt = 1
      << be_uidt_nl; // idt = 0

  // Constructor
  *os  << "protected:" << be_idt_nl
       << node->base_proxy_broker_name () << " (void);" << be_nl
       << be_uidt_nl;

  *os << "};" << be_nl << be_nl;

  be_global->non_local_interfaces.enqueue_tail (node);

  *os << "//" << be_nl
      << "//              End Base Proxy Broker Declaration " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
