#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, be_visitor_interface_remote_proxy_broker_ch, "$Id$")

be_visitor_interface_remote_proxy_broker_ch::be_visitor_interface_remote_proxy_broker_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_broker_ch::~be_visitor_interface_remote_proxy_broker_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_broker_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the class declaration.
  os->indent ();
  
  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//                 Remote Proxy Broker Declaration " << be_nl 
      << "//" << be_nl << be_nl;

  *os << "class " << be_global->stub_export_macro () << " "
      << node->remote_proxy_broker_name () << " : public virtual "
      << node->base_proxy_broker_name () << be_nl <<  "{" << be_nl
      << "public: " << be_idt_nl;

  // Destructor
  *os << "virtual ~" << node->remote_proxy_broker_name () << " (void);\n" << be_nl;

  // Accessor Method
  *os << "virtual " << node->base_proxy_impl_name () << " &" << "select_proxy (" << be_idt_nl;

  *os << node->local_name () << " *object," << be_nl
      << "CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "private:" << be_idt_nl
      << node->remote_proxy_impl_name () << " remote_proxy_impl_;"
      << be_uidt_nl;

  *os << "};" << be_nl << be_nl;

  // Factory Function declaration.
  *os << "// This funxtion is used to get an handle to the unique instance" << be_nl
      << "// of the Remote Proxy Broker that is available for a given" << be_nl
      << "// interface."
      << be_nl << be_nl;

  *os << node->remote_proxy_broker_name () << " *the" 
      << node->remote_proxy_broker_name ()
      << " (void);" << be_nl;

  *os << be_nl
      << "//" << be_nl
      << "//              End Remote Proxy Broker Declaration " << be_nl 
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
