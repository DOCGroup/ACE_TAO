//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    context_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for a context class in the
//    servant sourece.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_context_svs::be_visitor_context_svs (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_context_svs::~be_visitor_context_svs (void)
{
}

int
be_visitor_context_svs::visit_component (be_component *node)
{
  node_ = node;
  
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl
      << lname <<"_Context::" << lname << "_Context ("
      << be_idt << be_idt_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << lname << "_Servant * sv)" << be_uidt_nl
      << ": ::CIAO::Context_Impl_Base (h, c)," << be_idt_nl
      << "::CIAO::Context_Impl<" << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_Context," << be_nl
      << lname << "_Servant," << be_nl
      << "::" << node->full_name () << "> (h, c, sv)";

  if (swapping_)
    {
      os_ << "," << be_uidt_nl
          << "::CIAO::Upgradeable_Context_Impl<" << be_idt_nl
          << global << sname << "::CCM_" << lname
          << "_Context," << be_nl
          << lname << "_Servant," << be_nl
          << "::" << node_->full_name ()
          << "> (h, c, sv)";
    }

  os_ << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << lname << "_Context::~"
      << lname << "_Context (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << "/// CIAO-specific." << be_nl
      << lname << "_Context *" << be_nl
      << lname
      <<  "_Context::_narrow ( ::Components::SessionContext_ptr p)"
      << be_nl
      << "{" << be_idt_nl
      << "return dynamic_cast<" << lname
      << "_Context *> (p);" << be_uidt_nl
      << "}";
      
  if (this->visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_context_svs")
                         ACE_TEXT ("::visit_component - ")
                         ACE_TEXT ("visit_component_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  if (swapping_)
    {
      os_ << be_nl << be_nl
          << "/// Operations defined in " << sname
          << "::CCM_" << lname << "_Context" << be_nl
          << "/// that enable component swapping in the container"
          << be_nl << be_nl
          << "::Components::ConsumerDescriptions *" << be_nl
          << lname << "_Context::get_registered_consumers ("
          << be_idt_nl
          << "const char * publisher_name)" << be_uidt_nl
          << "{" << be_idt_nl
          << "if (publisher_name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "::Components::ConsumerDescriptions * tmp = 0;"
          << be_nl
          << "::Components::ConsumerDescriptions_var retval;"
          << be_nl
          << "::CORBA::ULong _ciao_index = 0UL;" << be_nl
          << "::CORBA::ULong _ciao_size = 0UL;" << be_nl
          << "ACE_UNUSED_ARG (tmp);" << be_nl
          << "ACE_UNUSED_ARG (retval);" << be_nl
          << "ACE_UNUSED_ARG (_ciao_index);" << be_nl
          << "ACE_UNUSED_ARG (_ciao_size);";

      be_visitor_swapping_get_consumer v (this->ctx_);
      
      if (v.visit_component_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_context_svs")
                             ACE_TEXT ("::visit_component - ")
                             ACE_TEXT ("swapping_get_consumer ")
                             ACE_TEXT ("visitor failed\n")),
                            -1);
        }

      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";
    }

  return 0;
}

int
be_visitor_context_svs::visit_uses (be_uses *node)
{
  AST_Type *obj = node->uses_type ();
  const char *port_name = node->local_name ()->get_string ();
  bool is_multiple = node->is_multiple ();

  if (is_multiple)
    {
      this->gen_uses_multiplex (obj, port_name);
    }
  else
    {
      this->gen_uses_simplex (obj, port_name);
    }

  return 0;
}

int
be_visitor_context_svs::visit_publishes (be_publishes *node)
{
  AST_Type *obj = node->publishes_type ();
  const char *port_name =
    node->local_name ()->get_string ();
  const char *fname = obj->full_name ();
  const char *lname = obj->local_name ()->get_string ();
  ACE_CString sname_str (
    ScopeAsDecl (obj->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::push_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << " * ev)" << be_uidt_nl
      << "{" << be_idt_nl;

  if (! static_config_)
    {
      os_ << "ACE_READ_GUARD (TAO_SYNCH_MUTEX," << be_nl
          << "                mon," << be_nl
          << "                this->" << port_name
          << "_lock_);" << be_nl << be_nl;
    }

  os_ << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_nl
      << "       this->ciao_publishes_" << port_name
      << "_.begin ();" << be_nl
      << "     iter != this->ciao_publishes_" << port_name
      << "_.end ();" << be_nl
      << "     ++iter)" << be_idt_nl
      << "{" << be_idt_nl
      << "iter->second->push_" << lname << " (ev);"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "ACE_CString source_id (this->_ciao_instance_id ());"
      << be_nl
      << "source_id += \"_" << port_name << "\";"
      << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::const_iterator giter =" << be_nl
      << "       this->ciao_publishes_" << port_name
      << "_generic_.begin ();" << be_nl
      << "     giter != this->ciao_publishes_"
      << port_name << "_generic_.end ();" << be_nl
      << "     ++giter)" << be_idt_nl
      << "{" << be_idt_nl
      << "giter->second->ciao_push_event (ev," << be_nl
      << "                                source_id.c_str (),"
      << be_nl
      << "                                " << global
      << sname << "::_tc_" << lname << ");" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::subscribe_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);"
      << be_nl
      << "entry.second = ::" << fname
      << "Consumer::_duplicate (c);" << be_nl << be_nl;

  if (! static_config_)
    {
    os_ << "{" << be_idt_nl
        << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
        << "                        mon," << be_nl
        << "                        this->" << port_name
        << "_lock_," << be_nl
        << "                        0);" << be_nl << be_nl;
    }

  os_ << "result = this->ciao_publishes_" << port_name
      << "_.insert (entry);";

  if (! static_config_)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_ERROR_RETURN ((LM_ERROR," << be_nl
      << "                   ACE_TEXT (\"subscribe on %s failed\\n\"),"
      << be_nl
      << "                   ACE_TEXT (\"" << port_name
      << "\"))," << be_nl
      << "                  0);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (retv," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::subscribe_"
      << port_name << "_generic (" << be_idt_nl
      << "::Components::EventConsumerBase_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);"
      << be_nl
      << "entry.second =" << be_idt_nl
      << "::Components::EventConsumerBase::_duplicate (c);"
      << be_uidt_nl << be_nl;

  if (! static_config_)
    {
      os_ << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        0);" << be_nl << be_nl;
    }

  os_ << "result =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_generic_.insert (entry);" << be_uidt;

  if (! static_config_)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_ERROR_RETURN ((LM_ERROR," << be_nl
      << "                   ACE_TEXT (\"generic subscribe \")"
      << be_nl
      << "                   ACE_TEXT (\"on %s failed\\n\"),"
      << be_nl
      << "                   ACE_TEXT (\"" << port_name
      << "\"))," << be_nl
      << "                  0);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (retv," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::unsubscribe_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "ptrdiff_t key = 0UL;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n = 0UL;" << be_nl << be_nl
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  if (! static_config_)
    {
      os_ << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        ::" << fname
          << "Consumer::_nil ());" << be_nl << be_nl;
    }

  os_ << tao_cg->upcase (port_name) << "_TABLE::iterator iter ="
      << be_idt_nl
      << "this->ciao_publishes_" << port_name << "_.find (key);"
      << be_uidt_nl << be_nl
      << "if (iter != this->ciao_publishes_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << fname
      << "Consumer_var retv = iter->second;" << be_nl
      << "n = this->ciao_publishes_" << port_name
      << "_.erase (key);" << be_nl << be_nl
      << "if (n == 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::iterator giter =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_generic_.find (key);" << be_uidt_nl << be_nl
      << "if (giter == this->ciao_publishes_" << port_name
      << "_generic_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "n = this->ciao_publishes_" << port_name
      << "_generic_.erase (key);";

  if (! static_config_)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "if (n != 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return ::" << fname << "Consumer::_nil ();"
      << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_context_svs::visit_emits (be_emits *node)
{
  AST_Type *obj = node->emits_type ();
  const char *port_name =
    node->local_name ()->get_string ();

  const char *fname = obj->full_name ();
  const char *lname = obj->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::push_" << port_name
      << " (" << be_idt_nl
      << "::" << fname << " * ev)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "this->ciao_emits_" << port_name << "_consumer_->push_"
      << lname << " (ev);" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_" << port_name
      << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ciao_emits_" << port_name
      << "_consumer_ =" << be_idt_nl
      << "::" << fname << "Consumer::_duplicate (c);"
      << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return this->ciao_emits_" << port_name
      << "_consumer_._retn ();" << be_uidt_nl
      << "}";

  return 0;
}

void
be_visitor_context_svs::gen_uses_simplex (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();

  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::get_connection_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return ::" << fname << "::_duplicate (" << be_idt_nl
      << "this->ciao_uses_" << port_name << "_.in ());"
      << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->ciao_uses_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ciao_uses_" << port_name << "_ =" << be_idt_nl
      << "::" << fname << "::_duplicate (c);"
      << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_uses_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return this->ciao_uses_" << port_name
      << "_._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_context_svs::gen_uses_multiplex (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();
  bool static_config = be_global->gen_ciao_static_config ();

  os_ << be_nl << be_nl
      << "::" << node_->full_name () << "::" << port_name
      << "Connections *" << be_nl
      << node_->local_name () << "_Context::get_connections_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl;

  if (! static_config)
    {
      os_ << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->" << port_name
          << "_lock_," << be_nl
          << "                       0);" << be_nl << be_nl;
    }

  os_ << "::" << node_->full_name () << "::" << port_name
      << "Connections * tmp_retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (tmp_retv," << be_nl
      << "                  ::" << node_->full_name ()
      << "::" << port_name << "Connections (" << be_nl
      << "                    this->ciao_uses_"
      << port_name << "_.size ())," << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "::" << node_->full_name () << "::" << port_name
      << "Connections_var retv = tmp_retv;" << be_nl
      << "retv->length (this->ciao_uses_" << port_name
      << "_.size ());" << be_nl
      << "::CORBA::ULong i = 0UL;" << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_nl
      << "       this->ciao_uses_" << port_name << "_.begin ();"
      << be_nl
      << "     iter != this->ciao_uses_"
      << port_name << "_.end ();" << be_nl
      << "     ++iter, ++i)" << be_idt_nl
      << "{" << be_idt_nl
      << "retv[i].objref = iter->second;" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (retv[i].ck.inout ()," << be_nl
      << "                  ::CIAO::Cookie_Impl (iter->first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn (); " << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);" << be_nl
      << "entry.second = ::" << fname
      << "::_duplicate (c);";

  if (! static_config)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        0);";
    }

  os_ << be_nl << be_nl
      << "result = this->ciao_uses_" << port_name
      << "_.insert (entry);";

  if (! static_config)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * ck = 0;" << be_nl
      << "ACE_NEW_THROW_EX (ck," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return ck;" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name  () << "_Context::disconnect_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << fname << "_var retv;" << be_nl
      << "ptrdiff_t key = 0UL;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n = 0UL;" << be_nl << be_nl
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt;

  if (! static_config)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        ::" << fname
          << "::_nil ());";
    }

  os_ << be_nl << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::iterator iter =" << be_idt_nl
      << "this->ciao_uses_" << port_name
      << "_.find (key);" << be_uidt_nl << be_nl
      << "if (iter == this->ciao_uses_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "retv = iter->second;" << be_nl
      << "n = this->ciao_uses_" << port_name
      << "_.erase (key);";

  if (! static_config)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "if (n != 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}";
}

// ===============================================

be_visitor_swapping_get_consumer::be_visitor_swapping_get_consumer (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_swapping_get_consumer::~be_visitor_swapping_get_consumer (
  void)
{
}

int
be_visitor_swapping_get_consumer::visit_publishes (
  be_publishes *node)
{
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "_ciao_size = this->_ciao_publishes_"
      << port_name << "_.size ();" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (tmp," << be_nl
      << "                  ::Components::"
      << "ConsumerDescriptions (_ciao_size)," << be_nl
      << "                  ::CORBA::NO_MEMORY ());";

  if (! static_config_)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->" << port_name
          << "_lock_," << be_nl
          << "                       0);";
    }

  os_ << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_idt_nl
      << "     this->ciao_publishes_" << port_name
      << "_.begin ();" << be_uidt_nl
      << "     iter != this->ciao_publishes_" << port_name
      << ".end ();" << be_nl
      << "     ++iter, ++_ciao_index)" << be_idt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (iter->second.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::ConsumerDescription * cd = 0;" << be_nl
      << "ACE_NEW_THROW_EX (cd," << be_nl
      << "                  OBV_Components::ConsumerDescription,"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "::Components::ConsumerDescription_var safe = cd;"
      << be_nl
      << "safe->name (\"\");" << be_nl
      << "safe->type_id (\"\");" << be_nl
      << "safe->consumer (iter->second.in ());"
      << be_nl << be_nl
      << "retval[_ciao_index] = safe;" << be_uidt_nl
      << "}" << be_uidt;

  if (! static_config_)
    {
      os_ << be_uidt_nl
          << "}";
    }

  os_ << be_uidt_nl << be_nl
      << "return retval._retn ();" << be_uidt_nl
      << "}";
      
  return 0;
}