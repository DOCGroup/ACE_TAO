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
  // Generate the class declaration.
  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//               Strategized Proxy Broker Declaration " << be_nl
      << "//" << be_nl << be_nl;

  *os << "class " << be_global->skel_export_macro () << " "
      << node->strategized_proxy_broker_name () << " : public virtual "
      << "::" << node->full_base_proxy_broker_name () << be_nl <<  "{" 
      << be_nl
      << "public: " << be_idt_nl;

  // Constructor
  *os << node->strategized_proxy_broker_name () << " (void);" << be_nl << be_nl;

  // Destructor
  *os << "virtual ~" << node->strategized_proxy_broker_name () << " (void);" 
      << be_nl << be_nl;

  // Accessor Method
  *os << "virtual " << "::" << node->full_base_proxy_impl_name () << " &" 
      << "select_proxy (" << be_idt_nl;

  *os << "::" << node->full_name () << " *object" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;


  *os << "private:" << be_idt_nl
      << "// Helper methods that takes care to create the proxy" << be_nl
      << "// as soon as their use is necessary." << be_nl
      << "void create_proxy (" << be_idt_nl << "int collocation_strategy"
      << be_nl << "ACE_ENV_ARG_DECL"
      << be_uidt_nl << ");"
      << be_nl << be_nl
      << "// Caches the proxy implementations. The proxy implementation" 
      << be_nl
      << "// are totally stateless, and those can be shared by all the" 
      << be_nl
      << "// instances of a given IDL interface type." << be_nl
      << "::" << node->full_base_proxy_impl_name () << be_nl
      << "*proxy_cache_[TAO_Collocation_Strategies::CS_LAST];"
      << be_nl << be_nl
      << "TAO_SYNCH_MUTEX mutex_;" << be_nl;

  // Factory Function declaration.
  *os << "// This funxtion is used to get an handle to the unique instance" 
      << be_nl
      << "// of the Strategized Proxy Broker that is available for a given" 
      << be_nl
      << "// interface."
      << be_uidt_nl << be_nl;

  *os << "public:" << be_idt_nl
      << "static " << node->strategized_proxy_broker_name ()
      << " *the" << node->strategized_proxy_broker_name ()
      << " (void);" << be_uidt_nl;

  *os << "};" << be_nl << be_nl;

  *os << be_nl
      << "//" << be_nl
      << "//            End Strategized Proxy Broker Declaration " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
