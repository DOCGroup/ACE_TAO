//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           strategized_proxy_broker_ss, 
           "$Id$")

be_visitor_interface_strategized_proxy_broker_ss::
be_visitor_interface_strategized_proxy_broker_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_strategized_proxy_broker_ss::
~be_visitor_interface_strategized_proxy_broker_ss (void)
{
  // No-Op.
}

int
be_visitor_interface_strategized_proxy_broker_ss::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the class declaration.
  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//            Strategized Proxy Broker Implementation" << be_nl
      << "//" << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "// Factory function Implementation." << be_nl
      << node->full_strategized_proxy_broker_name ()
      << " *" << be_nl
      << node->full_strategized_proxy_broker_name () << "::the"
      << node->strategized_proxy_broker_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl
      << "static " << node->full_strategized_proxy_broker_name ()
      << " strategized_proxy_broker;" << be_nl
      << "return &strategized_proxy_broker;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Constructor Implementation.
  *os << node->full_strategized_proxy_broker_name () << "::"
      << node->strategized_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // Destructor Implementation.
  *os << node->full_strategized_proxy_broker_name () << "::~"
      << node->strategized_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // get_strategy() impementation.
  *os << "TAO::Collocation_Strategy" << be_nl
      <<node->full_strategized_proxy_broker_name () << "::"
      << "get_strategy (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Collocation_Strategy strategy =" << be_idt_nl
      << "TAO_ORB_Core::collocation_strategy_new (obj ACE_ENV_ARG_PARAMETER);"
      << be_uidt_nl
      << "ACE_CHECK_RETURN (TAO::TAO_CS_REMOTE_STRATEGY);" << be_nl << be_nl
      << "return strategy;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // create_proxy implementation
  *os << "void" << be_nl
      << node->full_strategized_proxy_broker_name () << "::"
      << "dispatch (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Object_out forward_obj," << be_nl
      << "TAO::Argument ** args," << be_nl
      << "int num_args," << be_nl
      << "const char * op," << be_nl
      << "size_t op_len," << be_nl
      << "TAO::Collocation_Strategy strategy" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_TRY" << be_idt_nl
      << "{" << be_idt_nl
      << "switch (strategy)" << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->gen_thru_poa_collocation ())
    {
      *os << "case TAO::TAO_CS_THRU_POA_STRATEGY:" << be_idt_nl
          << "// Here is where we need table lookups." << be_nl;

      this->gen_thru_poa_operations (node, os);

      *os << "break;" << be_uidt_nl;
    }

  if (be_global->gen_direct_collocation ())
    {
      *os << "case TAO::TAO_CS_DIRECT_STRATEGY:" << be_idt_nl;

      this->gen_direct_operations (node, os);

      *os << "break;" << be_uidt_nl;
    }

  *os << "default:" << be_idt_nl
      << "ACE_THROW (CORBA::INTERNAL ());" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt
      << "\n#if (TAO_HAS_MINIMUM_CORBA == 0)" << be_nl
      << "ACE_CATCH (PortableServer::ForwardRequest, forward_request)" 
      << be_idt_nl
      << "{" << be_idt_nl
      << "forward_obj =" << be_idt_nl
      << "CORBA::Object::_duplicate (forward_request.forward_reference.in ());"
      << be_uidt_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt
      << "\n#else" << be_nl
      << "ACE_CATCHANY" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_UNUSED_ARG (forward_obj);" << be_nl
      << "ACE_RE_THROW;" << be_uidt_nl
      << "}" << be_uidt
      << "\n#endif /* TAO_HAS_MINIMUM_CORBA */" << be_nl
      << "ACE_ENDTRY;" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "//" << be_nl
      << "//        End Strategized Proxy Broker Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////";

  return 0;
}

int 
be_visitor_interface_strategized_proxy_broker_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

void
be_visitor_interface_strategized_proxy_broker_ss::gen_thru_poa_operations (
    be_interface *node,
    TAO_OutStream *os
  )
{
  int index = 0;
  AST_Decl *d = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d->node_type () != AST_Decl::NT_op)
        {
          continue;
        }

      if (index != 0)
        {
          *os << "else ";
        }

      ++index;

      *os << "if (ACE_OS::strcmp (op, \"" << d->local_name () << "\") == 0)"
          << be_idt_nl
          << "{" << be_idt_nl
          << node->full_thru_poa_proxy_impl_name () << "::" 
          << d->local_name () << " (" << be_idt << be_idt_nl
          << "obj," << be_nl
          << "forward_obj," << be_nl
          << "args," << be_nl
          << "num_args" << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt_nl
          << "ACE_TRY_CHECK;" << be_uidt_nl
          << "}" << be_uidt_nl;
    }
}

void
be_visitor_interface_strategized_proxy_broker_ss::gen_direct_operations (
    be_interface *,
    TAO_OutStream *
  )
{
}

