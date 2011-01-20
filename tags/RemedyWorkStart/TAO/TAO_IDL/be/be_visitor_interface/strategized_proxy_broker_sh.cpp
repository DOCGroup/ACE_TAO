//
// $Id$
//

be_visitor_interface_strategized_proxy_broker_sh::
be_visitor_interface_strategized_proxy_broker_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}

be_visitor_interface_strategized_proxy_broker_sh::
~be_visitor_interface_strategized_proxy_broker_sh (void)
{
  // No-Op.
}

int
be_visitor_interface_strategized_proxy_broker_sh::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//               Strategized Proxy Broker Declaration " << be_nl
      << "//" << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "class " << be_global->skel_export_macro () << " "
      << node->strategized_proxy_broker_name () << be_idt_nl
      << ": public virtual "
      << "TAO::Collocation_Proxy_Broker" << be_uidt_nl <<  "{"
      << be_nl
      << "public: " << be_idt;

  // Destructor
  *os << be_nl
      << "virtual ~" << node->strategized_proxy_broker_name () << " (void);";

  *os << be_nl_2
      << "TAO::Collocation_Strategy "
      << "get_strategy (::CORBA::Object_ptr obj);" << be_nl;

  *os << be_nl
      << "void" << be_nl
      << "dispatch (" << be_idt << be_idt_nl
      << "::CORBA::Object_ptr obj," << be_nl
      << "::CORBA::Object_out forward_obj," << be_nl
      << "bool &is_forwarded," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args," << be_nl
      << "const char * op," << be_nl
      << "size_t op_len," << be_nl
      << "TAO::Collocation_Strategy strategy" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_nl
      << "static " << node->strategized_proxy_broker_name ()
      << " *" << be_nl
      << "the" << node->strategized_proxy_broker_name ()
      << " (void);" << be_uidt_nl;

  *os << "};";

  *os << be_nl_2
      << "//" << be_nl
      << "//            End Strategized Proxy Broker Declaration " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl_2;

  return 0;
}

int be_visitor_interface_strategized_proxy_broker_sh::visit_component (
    be_component *node)
{
  return this->visit_interface (node);
}

int be_visitor_interface_strategized_proxy_broker_sh::visit_connector (
    be_connector *node)
{
  return this->visit_component (node);
}
