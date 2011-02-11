
//=============================================================================
/**
 *  @file    context_svs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a context class in the
 *  servant sourece.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

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
  // This visitor is spawned by be_visitor_component_svh,
  // which already does a check for imported node, so none
  // is needed here.

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
      << "::CIAO::" << be_global->ciao_container_type ()
      << "_Container_ptr c," << be_nl
      << "PortableServer::Servant sv," << be_uidt_nl
      << "const char *id)" << be_uidt_nl
      << ": ::CIAO::Context_Impl_Base_T < ::CIAO::"
      << be_global->ciao_container_type () << "_Container> (h, c, id),"
      << be_idt_nl
      << "::CIAO::" << be_global->ciao_container_type ()
      << "_Context_Impl<" << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_Context," << be_nl
      << "::" << node->full_name () << "> (h, c, sv, id)";

  os_ << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << lname << "_Context::~"
      << lname << "_Context (void)" << be_nl
      << "{" << be_nl
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

  return 0;
}

int
be_visitor_context_svs::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_context_svs::visit_uses (be_uses *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  AST_Type *obj = node->uses_type ();
  bool const is_multiple = node->is_multiple ();

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

  os_ << be_nl_2
      << "void" << be_nl
      << node_->local_name () << "_Context::push_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << " * ev)" << be_uidt_nl
      << "{" << be_idt_nl;

  os_ << "ACE_GUARD (TAO_SYNCH_MUTEX," << be_nl
      << "           mon," << be_nl
      << "           this->" << port_name
      << "_lock_);" << be_nl_2;

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
      << "}" << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::subscribe_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  os_ << "ptrdiff_t const ptr = reinterpret_cast<ptrdiff_t> (c);"
      << be_nl_2;

  os_ << "{" << be_idt_nl
      << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->" << port_name
      << "_lock_," << be_nl
      << "                  0);";

  os_ << be_nl_2
      << "::" << fname << "Consumer_var ciao_var = " << be_idt_nl
      << "::" << fname << "Consumer::_duplicate (c);" << be_uidt << be_nl_2
      << "std::pair<" << tao_cg->upcase (port_name) <<"_TABLE::iterator, bool> ret =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_.insert (" << be_idt_nl << tao_cg->upcase (port_name)
      << "_TABLE::value_type (ptr, ciao_var.in ()));"
      << be_uidt << be_uidt_nl
      << "if (!ret.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ciao_var._retn ();" << be_uidt_nl
      << "}";

  os_ << be_nl
      << be_nl
      << "::Components::Cookie * retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (retv," << be_nl
      << "                  ::CIAO::Cookie_Impl (ptr),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl_2
      << "return retv;" << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::unsubscribe_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "ptrdiff_t key = 0UL;" << be_nl_2
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  os_ << "{" << be_idt_nl
      << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->" << port_name
      << "_lock_," << be_nl
      << "                  ::" << fname
      << "Consumer::_nil ());" << be_nl_2;

  os_ << tao_cg->upcase (port_name) << "_TABLE::iterator iter ="
      << be_idt_nl
      << "this->ciao_publishes_" << port_name << "_.find (key);"
      << be_uidt_nl << be_nl
      << "if (iter != this->ciao_publishes_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << fname
      << "Consumer_var retv = iter->second;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_.erase (key);" << be_uidt_nl << be_nl
      << "if (n == 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl;

  os_ << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
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

  os_ << be_nl_2
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

  os_ << be_nl_2
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_" << port_name
      << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
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

  os_ << be_nl_2
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "::" << fname << "Consumer_var ciao_emits_" << port_name << " =" << be_idt_nl
      << "this->ciao_emits_" << port_name << "_consumer_._retn ();" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (ciao_emits_"
      << port_name << ".in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return ciao_emits_" << port_name
      << "._retn ();" << be_uidt_nl
      << "}";

  return 0;
}

void
be_visitor_context_svs::gen_uses_simplex (AST_Type *obj,
                                          const char *port_name)
{
  const char *fname = obj->full_name ();

  os_ << be_nl_2
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::get_connection_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return ::" << fname << "::_duplicate (" << be_idt_nl
      << "this->ciao_uses_" << port_name << "_.in ());"
      << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (this->ciao_uses_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ciao_uses_" << port_name << "_ =" << be_idt_nl
      << "::" << fname << "::_duplicate (c);"
      << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "::" << fname << "_var ciao_uses_" << port_name << " =" << be_idt_nl
      << "this->ciao_uses_" << port_name << "_._retn ();" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (ciao_uses_"
      << port_name << ".in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return ciao_uses_" << port_name
      << "._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_context_svs::gen_uses_multiplex (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();

  os_ << be_nl_2
      << "::" << node_->full_name () << "::" << port_name
      << "Connections *" << be_nl
      << node_->local_name () << "_Context::get_connections_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl;

  os_ << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->" << port_name
      << "_lock_," << be_nl
      << "                  0);" << be_nl_2;

  os_ << "::" << node_->full_name () << "::" << port_name
      << "Connections * tmp_retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (tmp_retv," << be_nl
      << "                  ::" << node_->full_name ()
      << "::" << port_name << "Connections (" << be_nl
      << "                    this->ciao_uses_"
      << port_name << "_.size ())," << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl_2
      << "::" << node_->full_name () << "::" << port_name
      << "Connections_var retv = tmp_retv;" << be_nl
      << "retv->length (this->ciao_uses_" << port_name
      << "_.size ());" << be_nl
      << "::CORBA::ULong i = 0UL;" << be_nl_2
      << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_nl
      << "       this->ciao_uses_" << port_name << "_.begin ();"
      << be_nl
      << "     iter != this->ciao_uses_"
      << port_name << "_.end ();" << be_nl
      << "     ++iter, ++i)" << be_idt_nl
      << "{" << be_idt_nl
      << "retv[i].objref = iter->second;" << be_nl_2
      << "ACE_NEW_THROW_EX (retv[i].ck.inout ()," << be_nl
      << "                  ::CIAO::Cookie_Impl (iter->first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn (); " << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  os_ << "ptrdiff_t const ptr = reinterpret_cast<ptrdiff_t> (c);";

  os_ << be_nl_2
      << "{" << be_idt_nl
      << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->" << port_name
      << "_lock_," << be_nl
      << "                  0);";

  os_ << be_nl_2
      << "::" << fname << "_var ciao_var = " << be_idt_nl
      << "::" << fname << "::_duplicate (c);" << be_uidt << be_nl_2
      << "std::pair<" << tao_cg->upcase (port_name) <<"_TABLE::iterator, bool> ret =" << be_idt_nl
      << "this->ciao_uses_" << port_name
      << "_.insert (" << be_idt_nl << tao_cg->upcase (port_name)
      << "_TABLE::value_type (ptr, ciao_var.in ()));"
      << be_uidt << be_uidt_nl
      << "if (!ret.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ciao_var._retn ();" << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::Components::Cookie * ck = 0;" << be_nl
      << "ACE_NEW_THROW_EX (ck," << be_nl
      << "                  ::CIAO::Cookie_Impl (ptr),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl_2
      << "return ck;" << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name  () << "_Context::disconnect_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << fname << "_var retv;" << be_nl
      << "ptrdiff_t key = 0UL;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n = 0UL;" << be_nl_2
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt;

  os_ << be_nl_2
      << "{" << be_idt_nl
      << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->" << port_name
      << "_lock_," << be_nl
      << "                  ::" << fname
      << "::_nil ());";

  os_ << be_nl_2
      << tao_cg->upcase (port_name)
      << "_TABLE::iterator iter =" << be_idt_nl
      << "this->ciao_uses_" << port_name
      << "_.find (key);" << be_uidt_nl << be_nl
      << "if (iter != this->ciao_uses_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "retv = iter->second;" << be_nl
      << "n = this->ciao_uses_" << port_name
      << "_.erase (key);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_uidt;

  os_ << be_nl_2
      << "if (n != 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}";
}

