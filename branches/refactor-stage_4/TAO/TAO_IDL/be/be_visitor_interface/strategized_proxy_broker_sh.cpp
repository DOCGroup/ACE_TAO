//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           base_proxy_broker_sh, 
           "$Id$")

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

  *os << be_nl << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//               Strategized Proxy Broker Declaration " << be_nl
      << "//" << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "class " << be_global->skel_export_macro () << " "
      << node->strategized_proxy_broker_name () << " : public virtual "
      << "::" << node->full_base_proxy_broker_name () << be_nl <<  "{" 
      << be_nl
      << "public: " << be_idt;

  // Constructor
  *os << be_nl
      << node->strategized_proxy_broker_name () << " (void);";

  // Destructor
  *os << be_nl << be_nl
      << "virtual ~" << node->strategized_proxy_broker_name () << " (void);";

  *os << be_nl << be_nl
      << "TAO::Collocation_Strategy" << be_nl
      << "get_strategy (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));" << be_uidt;

  *os << be_nl << be_nl
      << "void" << be_nl
      << "dispatch (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Object_out forward_obj," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args," << be_nl
      << "const char * op," << be_nl
      << "int op_len," << be_nl
      << "TAO::Collocation_Strategy strategy" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));" << be_uidt;

  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "static" << be_nl
      << node->strategized_proxy_broker_name () << " *" << be_nl 
      << "the" << node->strategized_proxy_broker_name ()
      << " (void);" << be_uidt_nl;

  *os << "};";

  *os << be_nl << be_nl
      << "//" << be_nl
      << "//            End Strategized Proxy Broker Declaration " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}

int be_visitor_interface_strategized_proxy_broker_sh::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

