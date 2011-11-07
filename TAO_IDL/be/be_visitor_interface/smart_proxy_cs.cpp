
//=============================================================================
/**
 *  @file    smart_proxy_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for smart_proxy classes for an Interface in the
 *  client proxy file.
 *
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


// ************************************************************
//  be_visitor_interface_smart_proxy_cs
// ************************************************************

be_visitor_interface_smart_proxy_cs::be_visitor_interface_smart_proxy_cs (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_smart_proxy_cs::~be_visitor_interface_smart_proxy_cs (
    void
  )
{
}

int be_visitor_interface_smart_proxy_cs::visit_interface (be_interface *node)
{

  if (be_global->gen_smart_proxies ())
    {
      TAO_OutStream *os = this->ctx_->stream ();
      this->ctx_->node (node);

      os->indent ();

      // Its necessary to take care of the nested case. The smart proxy classes
      // are in the same scope as the proxy.
      be_decl* scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << be_nl_2
          << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        {
          *os << "::";
        }

      *os <<"TAO_"<< node->flat_name () << "_Default_Proxy_Factory::";
      *os << "TAO_"
          << node->flat_name () << "_Default_Proxy_Factory (int permanent)"
          << be_nl
          << "{" << be_idt_nl
          << "TAO_" << node->flat_name ()
          << "_PROXY_FACTORY_ADAPTER::instance ()->register_proxy_factory (this, permanent);"<< be_uidt_nl
          << "}\n\n";

      os->indent ();

      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os << "TAO_"<< node->flat_name ()
          << "_Default_Proxy_Factory::~";
      *os <<"TAO_"
          << node->flat_name () << "_Default_Proxy_Factory (void)"
          << be_nl << be_uidt << be_uidt
          << "{" << be_nl
          << "}";

      *os << be_nl_2
          << node->full_name () << "_ptr" << be_nl << be_uidt << be_uidt;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os << "TAO_" << node->flat_name ()
          << "_Default_Proxy_Factory::create_proxy (" << be_idt << be_idt_nl
          << "::" << node->full_name ()
          << "_ptr proxy" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "return proxy;" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        {
          *os << "::";
        }

      *os << "TAO_" << node->flat_name () << "_Proxy_Factory_Adapter::";
      *os << "TAO_"
          << node->flat_name () << "_Proxy_Factory_Adapter (void)" << be_idt_nl
          << ": proxy_factory_ (0)," << be_idt_nl
          << "one_shot_factory_ (false)," << be_nl
          << "disable_factory_ (false)" << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "}\n\n";

      os->indent ();
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        {
          *os << "::";
        }

      *os << "TAO_" << node->flat_name () << "_Proxy_Factory_Adapter::";
      *os << "~TAO_"
          << node->flat_name () << "_Proxy_Factory_Adapter (void)" << be_nl
          << "{" << be_idt_nl
          << "// Making sure the factory which the adapter"
          << " has is destroyed with it."
          << be_nl
          << "if (this->proxy_factory_ != 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "delete this->proxy_factory_;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}\n\n";

      os->indent ();
      *os << "void" << be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        {
          *os << "::";
        }

      *os << "TAO_" << node->flat_name ()
          << "_Proxy_Factory_Adapter::register_proxy_factory (" << be_idt << be_idt_nl
          << "TAO_" << node->flat_name ()
          << "_Default_Proxy_Factory *df," << be_nl
          << "bool one_shot_factory" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "ACE_MT (" << be_idt << be_idt_nl
          << "ACE_GUARD (" << be_idt << be_idt_nl
          << "TAO_SYNCH_RECURSIVE_MUTEX," << be_nl
          << "ace_mon," << be_nl
          << "this->lock_" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" <<be_uidt_nl << be_nl
          << "// Remove any existing <proxy_factory_> and "
          << "replace with the new one." << be_nl
          << "this->unregister_proxy_factory ();" << be_nl
          << "this->proxy_factory_ = df;" << be_nl
          << "this->one_shot_factory_ = one_shot_factory;" << be_uidt_nl
          << "}\n\n";

      os->indent ();
      *os << "void" << be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os << "TAO_"<< node->flat_name ()
          << "_Proxy_Factory_Adapter::unregister_proxy_factory (void)" << be_nl
          << "{" << be_idt_nl
          << "ACE_MT (" << be_idt << be_idt_nl
          << "ACE_GUARD (" << be_idt << be_idt_nl
          << "TAO_SYNCH_RECURSIVE_MUTEX," << be_nl
          << "ace_mon," << be_nl
          << "this->lock_" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" <<be_uidt_nl << be_nl
          << "if (this->one_shot_factory_)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->disable_factory_ = true;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "if ("
          << "this->one_shot_factory_ == false && this->proxy_factory_ != 0)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "delete "
          << "this->proxy_factory_;" << be_nl
          << "this->proxy_factory_ = 0;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}\n\n";

      os->indent ();
      *os << node->full_name () << "_ptr" << be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os << "TAO_"<< node->flat_name ()
          << "_Proxy_Factory_Adapter::create_proxy (" << be_idt << be_idt_nl
          << "::" << node->full_name ()
          << "_ptr proxy" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "ACE_MT (ACE_GUARD_RETURN ("
          << "TAO_SYNCH_RECURSIVE_MUTEX, ace_mon," << be_idt_nl
          << "this->lock_, 0));" << be_uidt_nl << be_nl
          << "// To take care of those <unchecked_narrow> methods where we" << be_nl
          << "// want to override the smart proxy factory if there exists one." << be_nl
          << "if (this->disable_factory_)" << be_idt_nl
          << "{" << be_idt_nl
          << "this->disable_factory_ = false;" << be_nl
          << "return proxy;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "// Verify that an <proxy_factory_> is available else make one." << be_nl
          << "if ("
          <<"this->proxy_factory_ == 0)" << be_idt_nl
          << "ACE_NEW_RETURN ("
          << "this->proxy_factory_," << be_idt << be_idt_nl
          << "TAO_" << node->flat_name ()
          << "_Default_Proxy_Factory (0)," << be_nl
          << " 0);" << be_uidt_nl << be_uidt_nl << be_uidt_nl
          << "return "
          << "this->proxy_factory_->create_proxy (proxy);"
          << be_uidt << be_uidt_nl
          << "}";

      *os << be_nl_2
          << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
      *os << "TAO_"
          <<  node->flat_name () << "_Smart_Proxy_Base (void)" << be_uidt_nl
          << "{" << be_nl
          << "}" << be_nl_2;

      os->indent ();
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
      *os << "~TAO_"
          <<  node->flat_name () << "_Smart_Proxy_Base (void)" <<be_nl
          << "{" << be_nl
          << "}\n\n";

      // Implement the <stubobj> method of the base class
      os->indent ();
      *os << "TAO_Stub *"<<be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
      *os << "_stubobj (void) const"<<be_nl
          << "{" << be_idt_nl
          << "return this->base_proxy_->_stubobj ();"<< be_uidt_nl
          << "}"<< be_nl_2;

      // Implement the <stubobj> method of the base class
      os->indent ();
      *os << "TAO_Stub *"<<be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";
      *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
      *os << "_stubobj (void)"<<be_nl
          << "{" << be_idt_nl
          << "return this->base_proxy_->_stubobj ();"<< be_uidt_nl
          << "}" << be_nl_2;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_smart_proxy_cs::"
                             "visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }


      os->indent ();

      *os << node->full_name() << "_ptr" << be_nl;
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (node->is_nested ())
        *os << "::";

      *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::"
          << "get_proxy (void)" << be_idt_nl
          << be_uidt_nl;

      *os << "{" << be_idt_nl
          << "// Obtain the real proxy stored in <base_proxy_>" << be_nl
          << "if (CORBA::is_nil (this->proxy_.in ()))" << be_idt_nl
          << "{" << be_idt_nl
          << " // Verify whether factory is one-shot, if so disable"<<be_nl
          << " // factory temporarily or not else remove."<<be_nl
          << "TAO_"<< node->flat_name ()
          <<"_PROXY_FACTORY_ADAPTER::instance ()->unregister_proxy_factory ();"
          << be_nl << "this->proxy_ = " << "::" << node->full_name ()
          << "::_narrow (this->base_proxy_.in ());"
          << be_uidt_nl
          << "}" << be_uidt_nl
          << "return this->proxy_.in ();" << be_uidt_nl
          << "}" << be_nl_2;
    }
  else
    {
      ACE_UNUSED_ARG (node);
    }

  return 0;
}

int be_visitor_interface_smart_proxy_cs::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}
