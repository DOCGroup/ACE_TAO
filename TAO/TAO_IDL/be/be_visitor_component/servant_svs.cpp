
//=============================================================================
/**
 *  @file    servant_svs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a servant class in the
 *  servant source file.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_servant_svs::be_visitor_servant_svs (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    op_scope_ (0)
{
}

be_visitor_servant_svs::~be_visitor_servant_svs (void)
{
}

int
be_visitor_servant_svs::visit_component (be_component *node)
{
  // This visitor is spawned by be_visitor_component_svh,
  // which already does a check for imported node, so none
  // is needed here.

  node_ = node;

  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
  AST_Decl::NodeType nt = this->node_->node_type ();

  bool const is_connector = (nt == AST_Decl::NT_connector);
  bool no_events = false;

  if (!is_connector)
    {
      no_events =
        (node->n_consumes () == 0UL
         && node->n_emits () == 0UL
         && node->n_publishes () == 0UL);
    }

  bool const de_facto = (is_connector || no_events);

  const char *opt_conn =
    (de_facto ? "Connector_" : "");

  os_ << be_nl << be_nl
      << lname << "_Servant::"
      << lname << "_Servant (" << be_idt << be_idt << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_ptr exe," << be_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Home_Servant_Impl_Base * hs," << be_nl
      << "::CIAO::" << be_global->ciao_container_type ()
      << "_Container_ptr c)" << be_uidt << be_uidt_nl
      << ": ::CIAO::" << opt_conn
      << "Servant_Impl_Base (h, hs, c)," << be_idt_nl
      << "::CIAO::" << be_global->ciao_container_type ()
      << "_Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name ()
      << "," << be_nl
      << global << sname << "::CCM_"
      << lname << "," << be_nl
      << lname << "_Context> (exe, h, ins_name, hs, c)"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_idt_nl;

  be_visitor_obv_factory_reg ofr_visitor (this->ctx_);

  if (ofr_visitor.visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svs::"
                         "visit_component - "
                         "OBV factory registration failed\n"),
                        -1);
    }

  /// If a component has neither facets nor event sinks, the
  /// setup methods aren't generated.
  if (this->node_->n_remote_provides () > 0UL
      || this->node_->n_consumes () > 0UL)
    {
      be_visitor_populate_port_tables ppt_visitor (this->ctx_);

      if (ppt_visitor.visit_component_scope (node) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "be_visitor_component_svs::"
                           "visit_component - "
                           "populate port tables visitor failed\n"),
                          -1);
      }
    }

  os_ << be_uidt_nl << "}";

  os_ << be_nl << be_nl
      << lname << "_Servant::~"
      << lname << "_Servant (void)" << be_nl
      << "{" << be_nl
      << "}";

  if (this->node_->has_rw_attributes ())
    {
      os_ << be_nl << be_nl
          << "void" << be_nl
          << lname << "_Servant::set_attributes (" << be_idt_nl
          << "const ::Components::ConfigValues & descr)"
          << be_uidt_nl
          << "{" << be_idt_nl;

      os_ << "for ( ::CORBA::ULong i = 0; i < descr.length (); ++i)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "const char * descr_name = descr[i]->name ();"
          << be_nl
          << "::CORBA::Any & descr_value = descr[i]->value ();";

      be_visitor_attr_set as_visitor (this->ctx_);

      /// This will get assigned to the visitor's interface_
      /// member later so it can catch and skip porttype
      /// attributes in a component.
      as_visitor.node (this->node_);

      if (as_visitor.visit_component_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "servant_svs::"
                             "visit_component - "
                             "attr init visitor failed\n"),
                            -1);
        }

      os_ << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "/// Supported operations and attributes.";

  this->op_scope_ = node;

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->node_->get_insert_queue ().reset ();
  this->node_->get_del_queue ().reset ();
  this->node_->get_insert_queue ().enqueue_tail (node);

  Component_Op_Attr_Generator op_attr_gen (this);

  int status =
    this->node_->traverse_inheritance_graph (op_attr_gen,
                                             &os_,
                                             false,
                                             false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svs::"
                         "visit_component - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  os_ << be_nl << be_nl
      << "/// All ports and component attributes.";

  /// Port operations that require scope traversal to get all the
  /// possible string name matches.

  this->gen_provides_top ();
  this->gen_uses_top ();

  /// If we are visiting a connector we can skip these.
  if (this->node_->node_type () == AST_Decl::NT_component)
    {
      this->gen_publishes_top ();
      this->gen_emits_top ();
    }

  /// This call will generate all other operations and attributes,
  /// including inherited ones.
  if (this->visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svs::"
                         "visit_component - "
                         "visit_component_scope() failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_servant_svs::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_servant_svs::visit_operation (be_operation *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  // Components have implied IDL operations added to the AST, but
  // we are interested only in supported interface operations.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }

  be_visitor_operation_svs v (this->ctx_);
  v.scope (this->op_scope_);

  return v.visit_operation (node);
}

int
be_visitor_servant_svs::visit_attribute (be_attribute *node)
{
  this->ctx_->interface (this->node_);
  be_visitor_attribute v (this->ctx_);

  v.for_facets (false);
  v.op_scope (this->op_scope_);

  return v.visit_attribute (node);
}

int
be_visitor_servant_svs::visit_provides (be_provides *node)
{
  if (node->provides_type ()->is_local ()) // @TODO || be_global->gen_lwccm ())
    {
      return 0;
    }

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  AST_Type *obj = node->provides_type ();
  const char *obj_name = obj->full_name ();
  AST_Decl *scope = ScopeAsDecl (obj->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // Avoid '_cxx_' prefix.
  const char *lname =
    obj->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");
  const char *prefix_connector = (sname_str == "" ? "" : "_");

  if (!be_global->gen_lwccm ())
    {
      os_ << be_nl << be_nl
          << "::" << obj_name << "_ptr" << be_nl
          << node_->local_name () << "_Servant::provide_"
          << port_name << " (void)" << be_nl
          << "{" << be_idt_nl;

      os_ << "return" << be_idt_nl
          <<  "::" << obj_name << "::_duplicate (this->provide_"
          << port_name << "_.in ());" << be_uidt << be_uidt_nl
          << "}";
    }

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::setup_"
      << port_name << "_i (void)" << be_nl
      << "{" << be_idt_nl
      << "typedef" << be_idt_nl
      << "::CIAO::Port_Activator_T<" << be_idt_nl
      << "::CIAO_FACET" << prefix_connector
      << scope->flat_name () << "::" << lname
      << "_Servant," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << "::Components::CCMContext," << be_nl
      << node_->local_name () << "_Servant>" << be_uidt_nl
      << "MACRO_MADNESS_TYPEDEF;" << be_uidt_nl << be_nl
      << "MACRO_MADNESS_TYPEDEF * tmp = 0;" << be_nl
      << "ACE_CString obj_id (this->ins_name_);" << be_nl
      << "obj_id += \"_" << port_name << "\";" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (" << be_idt_nl
      << "tmp," << be_nl
      << "MACRO_MADNESS_TYPEDEF (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << port_name << "\"," << be_nl
      << "::CIAO::Port_Activator_Types::FACET," << be_nl
      << "0," << be_nl
      << "this->context_," << be_nl
      << "this)," << be_uidt_nl
      << "::CORBA::NO_MEMORY ());" << be_uidt_nl << be_nl
      << "::CIAO::Port_Activator_var pa = tmp;" << be_nl << be_nl
      << "::CIAO::Servant_Activator_var sa =" << be_idt_nl
      << "this->container_->ports_servant_activator ();"
      << be_uidt_nl << be_nl
      << "if (sa->register_port_activator (pa.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "::CORBA::Object_var obj =" << be_idt_nl
      << "this->container_->generate_reference (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "::CIAO::Container_Types::FACET_CONSUMER_t);"
      << be_uidt_nl << be_uidt_nl
      << "this->add_facet (\"" << port_name << "\", obj.in ());"
      << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_servant_svs::visit_uses (be_uses *node)
{
  if (node->uses_type ()->is_local ())
    {
      return 0;
    }

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  const char *obj_name = node->uses_type ()->full_name ();
  bool const is_multiple = node->is_multiple ();

  os_ << be_nl << be_nl
      << (is_multiple ? "::Components::Cookie *" : "void")
      << be_nl
      << node_->local_name () << "_Servant::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl;

  if (is_multiple)
    {
      os_ << "return ";
    }

  os_ << "this->context_->connect_" << port_name
      << " (c);" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << obj_name << "_ptr" << be_nl
      << node_->local_name () << "_Servant::disconnect_"
      << port_name << " (";

  if (is_multiple)
    {
      os_ << be_idt_nl
          << "::Components::Cookie * ck" << be_uidt;
    }
  else
    {
      os_ << "void";
    }

  os_ << ")" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->disconnect_"
      << port_name << " (" << (is_multiple ? "ck" : "")
      << ");" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::";

  if (is_multiple)
    {
      os_ << node_->full_name () << "::" << port_name
          << "Connections *";
    }
  else
    {
      os_ << obj_name << "_ptr";
    }

  os_ << be_nl
      << node_->local_name () << "_Servant::get_connection"
      << (is_multiple ? "s" : "") << "_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->get_connection"
      << (is_multiple ? "s" : "") << "_"
      << port_name << " ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_servant_svs::visit_publishes (be_publishes *node)
{
  const char *obj_name = node->publishes_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Servant::subscribe_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl;

 os_ << "return this->context_->subscribe_" << port_name
      << " (c);" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << obj_name << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::unsubscribe_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl;

  os_ << "return this->context_->unsubscribe_" << port_name
      << " (ck);" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_servant_svs::visit_emits (be_emits *node)
{
  const char *obj_name = node->emits_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_->connect_" << port_name
      << " (c);" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << obj_name << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->disconnect_"
      << port_name << " ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_servant_svs::visit_consumes (be_consumes *node)
{
  AST_Type  *obj = node->consumes_type ();
  const char *port_name = node->local_name ()->get_string ();

  const char *comp_lname = node_->local_name ();
  ACE_CString comp_sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *comp_sname = comp_sname_str.c_str ();
  const char *global = (comp_sname_str == "" ? "" : "::");

  const char *lname = obj->local_name ()->get_string ();
  const char *fname = obj->full_name ();

  os_ << be_nl << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant (" << be_idt << be_idt_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_ptr executor," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_Context_ptr c)" << be_uidt_nl
      << ": executor_ ( " << global << comp_sname << "::CCM_"
      << comp_lname << "::_duplicate (executor))," << be_idt_nl
      << "ctx_ ( " << global << comp_sname
      << "::CCM_" << comp_lname
      << "_Context::_duplicate (c))" << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::~" << lname << "Consumer_"
      << port_name << "_Servant (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::_get_component (void)" << be_nl
      << "{" << be_idt_nl;

  if (ACE_OS::strcmp (be_global->ciao_container_type (), "Session") == 0)
    {
      os_ << "return this->ctx_->get_CCM_object ();";
    }
  else
    {
      os_ << "return ::CORBA::Object::_nil ();";
    }

  os_ << be_uidt_nl << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::push_" << lname
      << " (" << be_idt_nl
      << "::" << fname << " * evt)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->executor_->push_" << port_name
      << " (evt);" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "/// Inherited from ::Components::EventConsumerBase."
      << be_nl
      << "void" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::push_event (" << be_idt_nl
      << "::Components::EventBase * ev)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << fname << " * ev_type =" << be_idt_nl
      << "::" << fname << "::_downcast (ev);"
      << be_uidt_nl << be_nl
      << "if (ev_type != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->push_" << lname << " (ev_type);" << be_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "throw ::Components::BadEventType ();" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::get_consumer_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "::" << fname << "Consumer::_duplicate (" << be_idt_nl
      << "this->consumes_" << port_name << "_.in ());"
      << be_uidt << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::setup_consumer_"
      << port_name << "_i (void)" << be_nl
      << "{" << be_idt_nl
      << "typedef" << be_idt_nl
      << "::CIAO::Port_Activator_T<" << be_idt_nl
      << node_->local_name () << "_Servant::" << lname
      << "Consumer_" << port_name << "_Servant," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_Context," << be_nl
      << node_->local_name () << "_Servant>" << be_uidt_nl
      << "MACRO_MADNESS_TYPEDEF;" << be_uidt_nl << be_nl
      << "MACRO_MADNESS_TYPEDEF * tmp = 0;" << be_nl
      << "ACE_CString obj_id (this->ins_name_);" << be_nl
      << "obj_id += \"_" << port_name << "\";" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (" << be_idt_nl
      << "tmp," << be_nl
      << "MACRO_MADNESS_TYPEDEF (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << port_name << "\"," << be_nl
      << "::CIAO::Port_Activator_Types::SINK," << be_nl
      << "this->executor_.in ()," << be_nl
      << "this->context_," << be_nl
      << "this)," << be_uidt_nl
      << "::CORBA::NO_MEMORY ());" << be_uidt_nl << be_nl
      << "::CIAO::Port_Activator_var pa = tmp;" << be_nl << be_nl
      << "::CIAO::Servant_Activator_var sa =" << be_idt_nl
      << "this->container_->ports_servant_activator ();"
      << be_uidt_nl << be_nl
      << "if (sa->register_port_activator (pa.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "::CORBA::Object_var obj =" << be_idt_nl
      << "this->container_->generate_reference (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"";

  ACE_CString work (obj->repoID ());
  ACE_CString result (work.substr (0, work.rfind (':')));
  result += "Consumer:1.0";

  os_ << result.c_str ();

  os_ << "\"," << be_nl
      << "::CIAO::Container_Types::FACET_CONSUMER_t);"
      << be_uidt_nl << be_uidt_nl
      << "::Components::EventConsumerBase_var ecb =" << be_idt_nl
      << "::Components::EventConsumerBase::_narrow (obj.in ());"
      << be_uidt_nl << be_nl
      << "this->add_consumer (\"" << port_name << "\", ecb.in ());"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_uidt_nl
      << "}";

  return 0;
}

void
be_visitor_servant_svs::gen_provides_top (void)
{
  if (this->node_->n_provides () == 0UL)
    {
      return;
    }

  os_ << be_nl << be_nl
      << "/// CIAO-specific." << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << node_->local_name ()
      << "_Servant::get_facet_executor (" << be_idt_nl
      << "const char * name)" << be_uidt_nl
      << "{" << be_idt_nl;

  os_ << "if (name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;

  be_visitor_facet_executor_block feb_visitor (this->ctx_);

  if (feb_visitor.visit_component_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_visitor_component_svs::"
                  "gen_provides_top - "
                  "facet executor block visitor failed\n"));

      return;
    }

  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
}

void
be_visitor_servant_svs::gen_publishes_top (void)
{
  ACE_CDR::ULong npubs = this->node_->n_publishes ();

  if (npubs > 0UL)
    {
      os_ << be_nl << be_nl
          << "::Components::Cookie *" << be_nl
          << node_->local_name () << "_Servant::subscribe ("
          << be_idt_nl
          << "const char * publisher_name," << be_nl
          << "::Components::EventConsumerBase_ptr subscribe)"
          << be_uidt_nl
          << "{" << be_idt_nl;

      os_ << "if (publisher_name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
          << "}" << be_uidt;

      be_visitor_subscribe_block sb_visitor (this->ctx_);

      if (sb_visitor.visit_component_scope (node_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_visitor_component_svs::"
                      "gen_publishes_top - "
                      "subscribe block visitor failed\n"));

          return;
        }

      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";

      os_ << be_nl << be_nl
          << "::Components::EventConsumerBase_ptr" << be_nl
          << node_->local_name () << "_Servant::unsubscribe ("
          << be_idt_nl
          << "const char * publisher_name," << be_nl
          << "::Components::Cookie * ck)" << be_uidt_nl
          << "{" << be_idt_nl;

      os_ << "if (publisher_name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
          << "}" << be_uidt;

      be_visitor_unsubscribe_block ub_visitor (this->ctx_);

      if (ub_visitor.visit_component_scope (node_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_visitor_component_svs::"
                      "gen_publishes_top - "
                      "unsubscribe block visitor failed\n"));

          return;
        }

      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";
    }

  if (!be_global->gen_lwccm ())
    {
      os_ << be_nl << be_nl
          << "::Components::PublisherDescriptions *" << be_nl
          << node_->local_name ()
          << "_Servant::get_all_publishers (void)" << be_nl
          << "{" << be_idt_nl
          << "::Components::PublisherDescriptions *retval = 0;"
          << be_nl
          << "ACE_NEW_RETURN (retval," << be_nl
          << "                ::Components::PublisherDescriptions,"
          << be_nl
          << "                0);" << be_nl << be_nl
          << "::Components::PublisherDescriptions_var "
          << "safe_retval = retval;" << be_nl
          << "safe_retval->length (" << this->node_->n_publishes ()
          << "UL);";

      if (npubs > 0UL)
        {
          be_visitor_event_source_desc esd_visitor (this->ctx_);

          if (esd_visitor.visit_component_scope (node_) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("be_visitor_component_svs::")
                          ACE_TEXT ("gen_publishes_top - ")
                          ACE_TEXT ("event source description ")
                          ACE_TEXT ("visitor failed\n")));

              return;
            }
        }

      os_ << be_nl << be_nl
          << "return safe_retval._retn ();" << be_uidt_nl
          << "}";
    }
}

void
be_visitor_servant_svs::gen_uses_top (void)
{
  ACE_CDR::ULong nuses = this->node_->n_uses ();

  if (nuses > 0UL)
    {
      os_ << be_nl << be_nl
          << "::Components::Cookie *" << be_nl
          << node_->local_name () << "_Servant::connect ("
          << be_idt_nl
          << "const char * name," << be_nl
          << "::CORBA::Object_ptr connection)" << be_uidt_nl
          << "{" << be_idt_nl;

      os_ << "if (name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
          << "}" << be_uidt;

      be_visitor_connect_block cb_visitor (this->ctx_);

      if (cb_visitor.visit_component_scope (node_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("be_visitor_component_svs::")
                      ACE_TEXT ("gen_uses_top - ")
                      ACE_TEXT ("connect block visitor failed\n")));

          return;
        }

      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";

      os_ << be_nl << be_nl
          << "::CORBA::Object_ptr" << be_nl
          << node_->local_name () << "_Servant::disconnect ("
          << be_idt_nl
          << "const char * name," << be_nl
          << "::Components::Cookie * "
          << (this->node_->has_uses_multiple () ? "ck" : "/* ck */")
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "if (name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
          << "}" << be_uidt;

      be_visitor_disconnect_block db_visitor (this->ctx_);

      if (db_visitor.visit_component_scope (node_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_visitor_component_svs::"
                      "gen_uses_top - "
                      "disconnect block visitor failed\n"));

          return;
        }

      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";
    } // nuses > 0UL

  if (!be_global->gen_lwccm ())
    {
      os_ << be_nl << be_nl
          << "::Components::ReceptacleDescriptions *" << be_nl
          << node_->local_name ()
          << "_Servant::get_all_receptacles (void)" << be_nl
          << "{" << be_idt_nl
          << "::Components::ReceptacleDescriptions * retval = 0;"
          << be_nl
          << "ACE_NEW_RETURN (retval," << be_nl
          << "                ::Components::ReceptacleDescriptions,"
          << be_nl
          << "                0);" << be_nl
          << "::Components::ReceptacleDescriptions_var "
          << "safe_retval = retval;" << be_nl
          << "safe_retval->length (" << nuses
          << "UL);";

      if (nuses > 0UL)
        {
          be_visitor_receptacle_desc rd_visitor (this->ctx_);

          if (rd_visitor.visit_component_scope (node_) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("be_visitor_component_svs::")
                          ACE_TEXT ("gen_uses_top - ")
                          ACE_TEXT ("receptacle description ")
                          ACE_TEXT ("visitor failed\n")));

              return;
            }
        }

      os_ << be_nl << be_nl
          << "return safe_retval._retn ();" << be_uidt_nl
          << "}";
    }
}

void
be_visitor_servant_svs::gen_emits_top (void)
{
  /// Generated whether the component has emits ports
  /// or not, except if we are generating a LwCCM
  /// profile, in which case this call returns
  /// immediately, and the base class method
  /// (returning a null pointer to a sequence of
  /// emitter descriptions) is seen instead.
  this->gen_get_all_emitters ();

  if (this->node_->n_emits () == 0UL)
    {
      return;
    }

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::connect_consumer ("
      << be_idt_nl
      << "const char * emitter_name," << be_nl
      << "::Components::EventConsumerBase_ptr consumer)"
      << be_uidt_nl
      << "{" << be_idt_nl;

  os_ << "if (emitter_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;

  be_visitor_connect_consumer_block ccb_visitor (this->ctx_);

  if (ccb_visitor.visit_component_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_visitor_component_svs::"
                  "gen_emits_top - "
                  "connect consumer block visitor failed\n"));

      return;
    }

  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "::Components::EventConsumerBase_ptr" << be_nl
      << node_->local_name ()
      << "_Servant::disconnect_consumer (" << be_idt_nl
      << "const char * source_name)" << be_uidt_nl
      << "{" << be_idt_nl;

  os_ << "if (source_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;

  be_visitor_disconnect_consumer_block dcb_visitor (this->ctx_);

  if (dcb_visitor.visit_component_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_visitor_component_svs::"
                  "gen_emits_top - "
                  "disconnect consumer block visitor failed\n"));

      return;
    }

  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
}

void
be_visitor_servant_svs::gen_get_all_emitters (void)
{
  if (be_global->gen_lwccm ())
    {
      return;
    }

  os_ << be_nl << be_nl
      << "::Components::EmitterDescriptions *" << be_nl
      << node_->local_name ()
      << "_Servant::get_all_emitters (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EmitterDescriptions *retval = 0;"
      << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                ::Components::EmitterDescriptions,"
      << be_nl
      << "                0);" << be_nl << be_nl
      << "::Components::EmitterDescriptions_var "
      << "safe_retval = retval;" << be_nl
      << "safe_retval->length (" << this->node_->n_emits ()
      << "UL);";

  be_visitor_emitter_desc ed_visitor (this->ctx_);

  if (ed_visitor.visit_component_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_visitor_component_svs::"
                  "gen_emits_top - "
                  "emitter description visitor failed\n"));

      return;
    }

  os_ << be_nl << be_nl
      << "return safe_retval._retn ();" << be_uidt_nl
      << "}";
}

// ==========================================================

be_visitor_obv_factory_reg::be_visitor_obv_factory_reg (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_obv_factory_reg::~be_visitor_obv_factory_reg (void)
{
}

int
be_visitor_obv_factory_reg::visit_publishes (be_publishes *node)
{
  this->gen_obv_factory_registration (node->field_type ());

  return 0;
}

int
be_visitor_obv_factory_reg::visit_emits (be_emits *node)
{
  this->gen_obv_factory_registration (node->field_type ());

  return 0;
}

int
be_visitor_obv_factory_reg::visit_consumes (be_consumes *node)
{
  this->gen_obv_factory_registration (node->field_type ());

  return 0;
}

void
be_visitor_obv_factory_reg::gen_obv_factory_registration (AST_Type *t)
{
  if (be_global->gen_ciao_valuefactory_reg ())
    {
      const char *fname = t->full_name ();

      os_ << "TAO_OBV_REGISTER_FACTORY (" << be_idt_nl
          << "::" << fname << "_init," << be_nl
          << "::" << fname << ");" << be_uidt_nl;
    }
}

// ==========================================================

be_visitor_attr_set::be_visitor_attr_set (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_attr_set::~be_visitor_attr_set (void)
{
}

int
be_visitor_attr_set::visit_attribute (be_attribute *node)
{
  be_visitor_attribute_ccm_init v (this->ctx_);

  if (v.visit_attribute (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_attr_set")
                         ACE_TEXT ("::visit_attribute - ")
                         ACE_TEXT ("ccm attr init visitor ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

// ==========================================================

be_visitor_facet_executor_block::be_visitor_facet_executor_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_executor_block::~be_visitor_facet_executor_block (
  void)
{
}

int
be_visitor_facet_executor_block::visit_provides (
  be_provides *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->executor_->get_" << port_name
      << " ();" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ======================================================

be_visitor_connect_block::be_visitor_connect_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_connect_block::~be_visitor_connect_block (void)
{
}

int
be_visitor_connect_block::visit_uses (be_uses *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  const char *obj_name = node->uses_type ()->full_name ();
  bool const is_multiple = node->is_multiple ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name << "_var _ciao_conn =" << be_idt_nl
      << "::" << obj_name << "::_narrow (connection);"
      << be_uidt_nl << be_nl;

  if (node->uses_type ()->is_local ())
    {
      // @@todo: placeholder for connection logic
      os_ << "/// " << (is_multiple ? "Multiplex" : "Simplex")
          << " connect." << be_nl
          << (is_multiple ? "return " : "") << "this->context_->connect_"
          << port_name << " (_ciao_conn.in ());";
    }
  else
    {
      os_ << "if ( ::CORBA::is_nil (_ciao_conn.in ()))" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::Components::InvalidConnection ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
      os_ << "/// " << (is_multiple ? "Multiplex" : "Simplex")
          << " connect." << be_nl
          << (is_multiple ? "return " : "") << "this->connect_"
          << port_name << " (_ciao_conn.in ());";
    }

  if (! is_multiple)
    {
      os_ << be_nl
          << "return 0;";
    }

  os_ << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ======================================================

be_visitor_disconnect_block::be_visitor_disconnect_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_disconnect_block::~be_visitor_disconnect_block (
  void)
{
}

int
be_visitor_disconnect_block::visit_uses (be_uses *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  bool const is_multiple = node->is_multiple ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "/// " << (is_multiple ? "Multiplex" : "Simplex")
      << " disconnect." << be_nl;

  if (is_multiple)
    {
      os_ << "if (ck == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::Components::CookieRequired ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
    }

  os_ << "return this->";

  if (node->uses_type ()->is_local ())
    {
      os_ << "context_->";
    }

  os_ << "disconnect_" << port_name
      << " (" << (is_multiple ? "ck" : "") << ");" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ======================================================

be_visitor_receptacle_desc::be_visitor_receptacle_desc (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    slot_ (0UL)
{
}

be_visitor_receptacle_desc::~be_visitor_receptacle_desc (
  void)
{
}

int
be_visitor_receptacle_desc::visit_uses (be_uses *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  AST_Type *obj = node->uses_type ();
  bool const is_multiple = node->is_multiple ();

  os_ << be_nl << be_nl;

  bool gen_guard = is_multiple;

  if (gen_guard)
    {
      os_ << "{" << be_idt_nl
          << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                  mon," << be_nl
          << "                  this->context_->"
          << port_name << "_lock_," << be_nl
          << "                  0);" << be_nl << be_nl;
    }

  os_ << "::CIAO::Servant::describe_"
      << (is_multiple ? "multiplex" : "simplex")
      << "_receptacle<" << be_idt_nl
      << "::" << obj->full_name () << "_var> (" << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_uses_"
      << port_name << "_," << be_nl
      << "safe_retval," << be_nl
      << slot_++ << "UL);" << be_uidt << be_uidt;

  if (gen_guard)
    {
      os_ << be_uidt_nl
          << "}";
    }

  return 0;
}

// ======================================================

be_visitor_subscribe_block::be_visitor_subscribe_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_subscribe_block::~be_visitor_subscribe_block (
  void)
{
}

int
be_visitor_subscribe_block::visit_publishes (
  be_publishes *node)
{
  const char *obj_name =
    node->publishes_type ()->full_name ();
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name << "Consumer_var sub =" << be_idt_nl
      << "::" << obj_name << "Consumer::_narrow (subscribe);"
      << be_uidt_nl << be_nl
      << "return this->subscribe_" << port_name
      << " (sub.in ());" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ==========================================================

be_visitor_unsubscribe_block::be_visitor_unsubscribe_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_unsubscribe_block::~be_visitor_unsubscribe_block (
  void)
{
}

int
be_visitor_unsubscribe_block::visit_publishes (
  be_publishes *node)
{
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->unsubscribe_" << port_name
      << " (ck);" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ==========================================================

be_visitor_event_source_desc::be_visitor_event_source_desc (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    slot_ (0UL)
{
}

be_visitor_event_source_desc::~be_visitor_event_source_desc (
  void)
{
}

int
be_visitor_event_source_desc::visit_publishes (
  be_publishes *node)
{
  AST_Type *obj = node->publishes_type ();
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl;

  os_ << "{" << be_idt_nl
      << "ACE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
      << "                  mon," << be_nl
      << "                  this->context_->"
      << port_name << "_lock_," << be_nl
      << "                  0);" << be_nl << be_nl;

  os_ << "::CIAO::Servant::describe_pub_event_source<"
      << be_idt_nl
      << "::" << obj->full_name () << "Consumer_var> ("
      << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_publishes_"
      << port_name << "_," << be_nl
      << "safe_retval," << be_nl
      << slot_++ << "UL);" << be_uidt << be_uidt;

  os_ << be_uidt_nl
      << "}";

  return 0;
}

// ======================================================

be_visitor_connect_consumer_block::be_visitor_connect_consumer_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_connect_consumer_block::~be_visitor_connect_consumer_block (void)
{
}

int
be_visitor_connect_consumer_block::visit_emits (
  be_emits *node)
{
  const char *obj_name =
    node->emits_type ()->full_name ();
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (emitter_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name
      << "Consumer_var _ciao_consumer =" << be_idt_nl
      << "::" << obj_name
      << "Consumer::_narrow (consumer);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (_ciao_consumer.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->connect_" << port_name
      << " (_ciao_consumer.in ());" << be_nl << be_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ======================================================

be_visitor_disconnect_consumer_block::be_visitor_disconnect_consumer_block (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_disconnect_consumer_block::~be_visitor_disconnect_consumer_block (
  void)
{
}

int
be_visitor_disconnect_consumer_block::visit_emits (
  be_emits *node)
{
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (source_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->disconnect_" << port_name
      << " ();" << be_uidt_nl
      << "}" << be_uidt;

  return 0;
}

// ======================================================

be_visitor_emitter_desc::be_visitor_emitter_desc (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    slot_ (0UL)
{
}

be_visitor_emitter_desc::~be_visitor_emitter_desc (
  void)
{
}

int
be_visitor_emitter_desc::visit_emits (be_emits *node)
{
  AST_Type *obj = node->emits_type ();
  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "::CIAO::Servant::describe_emit_event_source<"
      << be_idt_nl
      << "::" << obj->full_name () << "Consumer_var> ("
      << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_emits_" << port_name
      << "_consumer_," << be_nl
      << "safe_retval," << be_nl
      << slot_++ << "UL);" << be_uidt << be_uidt;

  return 0;
}

// ======================================================

be_visitor_populate_port_tables::be_visitor_populate_port_tables (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_populate_port_tables::~be_visitor_populate_port_tables (
  void)
{
}

int
be_visitor_populate_port_tables::visit_provides (
  be_provides *node)
{
  if (node->provides_type ()->is_local ())
    {
      return 0;
    }

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  os_ << be_nl
      << "this->setup_"
      << port_name << "_i ();";

  return 0;
}

int
be_visitor_populate_port_tables::visit_consumes (
  be_consumes *node)
{
  os_ << be_nl
      << "this->setup_consumer_"
      << node->local_name ()->get_string () << "_i ();";

  return 0;
}

// ======================================================

Component_Op_Attr_Generator::Component_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Component_Op_Attr_Generator::emit (be_interface * /* derived_interface */,
                                   TAO_OutStream * /* os */,
                                   be_interface * base_interface)
{
  AST_Decl::NodeType nt = base_interface->node_type ();

  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }

  return visitor_->visit_scope (base_interface);
}

