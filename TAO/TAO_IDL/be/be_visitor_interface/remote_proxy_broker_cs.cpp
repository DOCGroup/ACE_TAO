//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           remote_proxy_broker_cs, 
           "$Id$")

be_visitor_interface_remote_proxy_broker_cs::
be_visitor_interface_remote_proxy_broker_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_broker_cs::
~be_visitor_interface_remote_proxy_broker_cs (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_broker_cs::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the class declaration.
  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//           Remote & Base  Proxy Broker Implementation" << be_nl
      << "//" << be_nl << be_nl;

  *os << node->full_base_proxy_broker_name () << "::"
      << node->base_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  *os << node->full_base_proxy_broker_name () << "::~"
      << node->base_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // Generate the proxy broker factory function pointer definition.
  *os << node->full_base_proxy_broker_name () << " * (*"
      << node->flat_client_enclosing_scope ()
      << node->base_proxy_broker_name ()
            << "_Factory_function_pointer) ("
      << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_uidt_nl
      << ") = 0;" << be_uidt_nl << be_nl;

  *os << "// Factory Member function Implementation." << be_nl
      << node->full_remote_proxy_broker_name () << " *" << be_nl
      << node->full_remote_proxy_broker_name () << "::the"
      << node->remote_proxy_broker_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl
      << "static ::" << node->full_remote_proxy_broker_name ()
      << " remote_proxy_broker;" << be_nl
      << "return &remote_proxy_broker;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Ctor
  *os << node->full_remote_proxy_broker_name () << "::"
      << node->remote_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // Dtor
  *os << node->full_remote_proxy_broker_name () << "::~"
      << node->remote_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  *os << node->full_base_proxy_impl_name () << "&" << be_nl
      <<node->full_remote_proxy_broker_name () << "::"
      << "select_proxy (" << be_idt_nl
      << "::" << node->full_name () << " *" << be_nl
      << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl << ")"
      << be_uidt_nl << "{" << be_idt_nl // idt = 1
      << "return this->remote_proxy_impl_;"
      << be_uidt_nl << "}" << be_nl << be_nl;

   *os << be_nl
      << "//" << be_nl
      << "//           End Remote & Base Proxy Broker Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
