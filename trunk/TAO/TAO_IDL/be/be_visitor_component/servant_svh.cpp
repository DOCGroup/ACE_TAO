
//=============================================================================
/**
 *  @file    servant_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a servant class in the
 *  servant header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_servant_svh::be_visitor_servant_svh (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_servant_svh::~be_visitor_servant_svh (void)
{
}

int
be_visitor_servant_svh::visit_component (be_component *node)
{
  // This visitor is spawned by be_visitor_component_svh,
  // which already does a check for imported node, so none
  // is needed here.

  node_ = node;

  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "class " << export_macro_.c_str () << " " << lname
      << "_Servant" << be_idt_nl
      << ": public virtual" << be_idt << be_idt_nl
      << "::CIAO::Session_Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name () << "," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << lname << "_Context>"
      << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  os_ << "typedef " << global << sname << "::CCM_" << lname
      << " _exec_type;" << be_nl;

  os_ << be_nl
      << lname << "_Servant (" << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_ptr executor," << be_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Home_Servant_Impl_Base *hs," << be_nl
      << "::CIAO::Session_Container_ptr c);" << be_uidt_nl;

  os_ << be_nl
      << "virtual ~" << lname << "_Servant"
      << " (void);" << be_nl;

  if (this->node_->has_rw_attributes ())
    {
      os_ << be_nl
          << "virtual void" << be_nl
          << "set_attributes (const "
          << "::Components::ConfigValues & descr);"
          << be_nl;
    }

  os_ << be_nl
      << "/// Supported operations and attributes.";

  int status =
    node_->traverse_inheritance_graph (
      be_interface::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_servant_svh::"
                         "visit_component - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  status = this->visit_component_scope (node);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_servant_svh::"
                         "visit_component - "
                         "visit_component_scope() failed\n"),
                        -1);
    }

  this->gen_non_type_specific ();

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_servant_svh::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_servant_svh::visit_attribute (be_attribute *node)
{
  this->ctx_->interface (this->node_);
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_servant_svh::visit_provides (be_provides *node)
{
  if (node->provides_type ()->is_local ()) // @TODO || be_global->gen_lwccm ())
    {
      return 0;
    }

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();
  const char *obj_name = node->provides_type ()->full_name ();

  if (!be_global->gen_lwccm ())
    {
      os_ << be_uidt_nl << be_nl
          << "public:" << be_idt_nl
          << "virtual ::" << obj_name << "_ptr" << be_nl
          << "provide_" << port_name << " (void);";
    }

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "void" << be_nl
      << "setup_" << port_name << "_i (void);"
      << be_uidt_nl << be_nl;

  os_ << "private:" << be_idt_nl
      << "::" << obj_name << "_var" << be_nl
      << "provide_" << port_name << "_;";

  return 0;
}

int
be_visitor_servant_svh::visit_uses (be_uses *node)
{
  if (node->uses_type ()->is_local ())
    {
      return 0;
    }

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix +=node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();
  const char *obj_name = node->uses_type ()->full_name ();
  bool const is_multiple = node->is_multiple ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "virtual ";

  if (is_multiple)
    {
      os_ << "::" << node_->full_name () << "::"
          << port_name << "Connections *" << be_nl
          << "get_connections_" << port_name << " (void);";
    }
  else
    {
      os_ << "::" << obj_name << "_ptr" << be_nl
          << "get_connection_" << port_name << " (void);";
    }

  os_ << be_nl << be_nl
      << "virtual "
      << (is_multiple ? "::Components::Cookie *" : "void")
      << be_nl
      << "connect_" << port_name << " ("
      << "::" << obj_name << "_ptr);"
      << be_nl << be_nl;

  os_ << "virtual ::" << obj_name << "_ptr" << be_nl
      << "disconnect_" << port_name << " (";

  if (is_multiple)
    {
      os_ << "::Components::Cookie * ck);";
    }
  else
    {
      os_ << "void);";
    }

  return 0;
}

int
be_visitor_servant_svh::visit_publishes (be_publishes *node)
{
  const char *obj_name = node->publishes_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  os_ << "virtual ::Components::Cookie *" << be_nl
      << "subscribe_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c);" << be_uidt_nl;

  os_ << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "unsubscribe_" << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck);" << be_uidt;

  return 0;
}

int
be_visitor_servant_svh::visit_emits (be_emits *node)
{
  const char *obj_name = node->emits_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "connect_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c);" << be_uidt;

  os_ << be_nl << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "disconnect_" << port_name << " (void);";

  return 0;
}

int
be_visitor_servant_svh::visit_consumes (be_consumes *node)
{
  const char *obj_name = node->consumes_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  ACE_CString holder (obj_name);
  ACE_CString::size_type pos = holder.rfind (':');
  const char *ev_lname = 0;

  if (pos == ACE_CString::npos)
    {
      ev_lname = obj_name;
    }
  else
    {
      holder = holder.substr (pos + 1);
      ev_lname = holder.c_str ();
    }

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  os_ << "// Servant class for the " << port_name
      << " consumer." << be_nl
      << "class " << export_macro_.c_str () << " " << ev_lname
      << "Consumer_" << port_name << "_Servant" << be_idt_nl
      << ": public virtual ::POA_" << obj_name << "Consumer"
      << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << ev_lname << "Consumer_" << port_name
      << "_Servant (" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_ptr executor," << be_nl
      << global << sname << "::CCM_" << lname
      << "_Context_ptr c);" << be_uidt_nl << be_nl;

  os_ << "virtual ~" << ev_lname << "Consumer_" << port_name
      << "_Servant (void);";

  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "push_" << ev_lname << " (" << be_idt_nl
      << "::" << obj_name << " * evt);" << be_uidt;

  os_ << be_nl << be_nl
      << "// Inherited from ::Components::EventConsumerBase." << be_nl
      << "virtual void" << be_nl
      << "push_event ( ::Components::EventBase * ev);";

  os_ << be_nl << be_nl
      << "// Get component implementation." << be_nl
      << "virtual ::CORBA::Object_ptr" << be_nl
      << "_get_component (void);";

  os_ << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl;

  os_ << global << sname << "::CCM_" << lname << "_var" << be_nl
      << "executor_;";

  os_ << be_nl << be_nl
      << global << sname << "::CCM_"
      << lname << "_Context_var" << be_nl
      << "ctx_;";

  os_ << be_uidt_nl
      << "};";

  os_ << be_nl << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "get_consumer_" << port_name << " (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  os_ << "void" << be_nl
      << "setup_consumer_" << port_name << "_i (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  os_ << "::" << obj_name << "Consumer_var" << be_nl
      << "consumes_" << port_name << "_;";

  return 0;
}

void
be_visitor_servant_svh::gen_non_type_specific (void)
{
  os_ << be_nl << be_nl
      << "// Base class overrides."
      << be_uidt_nl << be_nl
      << "public:" << be_idt;

  if (this->node_->n_uses () > 0UL)
    {
      os_ << be_nl
          << "virtual ::Components::Cookie *" << be_nl
          << "connect (const char * name, ::CORBA::Object_ptr connection);";

      os_ << be_nl << be_nl
          << "virtual ::CORBA::Object_ptr " << be_nl
          << "disconnect (const char * name, ::Components::Cookie * ck);";
    }

  if (!be_global->gen_lwccm ())
    {
      os_ << be_nl << be_nl
          << "virtual ::Components::ReceptacleDescriptions *"
          << be_nl
          << "get_all_receptacles (void);";
    }

  AST_Decl::NodeType nt = this->node_->node_type ();
  bool is_connector = (nt == AST_Decl::NT_connector);

  if (!be_global->gen_lwccm () && !is_connector)
    {
      os_ << be_nl << be_nl
          << "virtual ::Components::PublisherDescriptions *"
          << be_nl
          << "get_all_publishers (void);";

      os_ << be_nl << be_nl
          << "virtual ::Components::EmitterDescriptions *"
          << be_nl
          << "get_all_emitters (void);";
    }

  /// If the node is a connector, event sources and sinks cannot
  /// be declared.
  if (!is_connector)
    {
      if (this->node_->n_publishes () > 0UL)
        {
          os_ << be_nl << be_nl
              << "virtual ::Components::Cookie *" << be_nl
              << "subscribe (const char * publisher_name,"
              << be_nl
              << "           ::Components::"
              << "EventConsumerBase_ptr subscriber);";

          os_ << be_nl << be_nl
              << "virtual ::Components::EventConsumerBase_ptr"
              << be_nl
              << "unsubscribe (const char * publisher_name,"
              << be_nl
              << "             ::Components::Cookie * ck);";
        }

      if (this->node_->n_emits () > 0UL)
        {
          os_ << be_nl << be_nl
              << "virtual void" << be_nl
              << "connect_consumer (const char * emitter_name,"
              << be_nl
              << "                  ::Components::"
              << "EventConsumerBase_ptr consumer);";

          os_ << be_nl << be_nl
              << "virtual ::Components::EventConsumerBase_ptr"
              << be_nl
              << "disconnect_consumer (const char * source_name);";
        }
    }

  if (this->node_->n_provides () > 0UL)
    {
      os_ << be_nl << be_nl
          << "/// CIAO-specific." << be_nl
          << "::CORBA::Object_ptr" << be_nl
          << "get_facet_executor (const char * name);";
    }
}

