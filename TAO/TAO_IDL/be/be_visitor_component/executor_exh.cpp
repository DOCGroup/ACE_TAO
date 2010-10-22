
//=============================================================================
/**
 *  @file    executor_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_executor_exh::be_visitor_executor_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->exec_export_macro ();
}

be_visitor_executor_exh::~be_visitor_executor_exh (void)
{
}

int
be_visitor_executor_exh::visit_attribute (be_attribute *node)
{
  AST_Decl::NodeType nt = this->node_->node_type ();

  // Executor attribute code generated for porttype attributes
  // always in connectors and only for mirrorports in components.
  if (this->in_ext_port_ && nt == AST_Decl::NT_component)
    {
      return 0;
    }

  this->ctx_->interface (this->node_);
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_executor_exh::visit_component (be_component *node)
{
  this->node_ = node;
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No _cxx_ prefix.
  const char *lname =
    node->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "class " << export_macro_.c_str () << " " << lname
      << "_exec_i" << be_idt_nl
      << ": public virtual " << lname << "_Exec," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  os_ << lname << "_exec_i (void);";

  os_ << be_nl
      << "virtual ~" << lname << "_exec_i (void);";

  os_ << be_nl_2
      << "//@{" << be_nl
      << "/** Supported operations and attributes. */";

  int status =
    node->traverse_inheritance_graph (
      be_interface::op_attr_decl_helper,
      &os_,
      false,
      false);

  os_ << be_nl_2 << "//@}" << be_nl_2;

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << "//@{" << be_nl
      << "/** Component attributes and port operations. */";

  status = this->visit_component_scope (node);

  os_<< "//@}" << be_nl_2;

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("visit_component_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << "//@{" << be_nl
      << "/** Operations from Components::" << be_global->ciao_container_type ()
      << "Component. */";

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl_2
      << "virtual void set_"
      << tao_cg->downcase (container_type)
      << "_context ("
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx);";

  if (ACE_OS::strcmp (be_global->ciao_container_type (), "Session") == 0)
    {
      os_ << be_nl_2
          << "virtual void configuration_complete (void);";

      os_ << be_nl_2
          << "virtual void ccm_activate (void);" << be_nl
          << "virtual void ccm_passivate (void);";
    }

  os_ << be_nl << "virtual void ccm_remove (void);";

  os_ << be_nl
      << "//@}";

  os_ << be_nl_2
      << "//@{" << be_nl
      << "/** User defined operations */";

  os_ << be_nl_2 << "//@}";

  os_ << be_uidt << be_nl_2
      << "private:" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context_var ciao_context_;";

  be_visitor_executor_private_exh v (this->ctx_);
  v.node (node);

  if (v.visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("private member ")
                         ACE_TEXT ("visitor failed\n")),
                        -1);
    }

  if (be_global->gen_ciao_exec_reactor_impl ())
    {
      os_ << be_nl_2
          << "ACE_Reactor* reactor (void);";
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_executor_exh::visit_provides (be_provides *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  be_type *impl = node->provides_type ();

  AST_Decl *scope = ScopeAsDecl (impl->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    impl->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "virtual " << global << sname << "::CCM_"
      << lname << "_ptr" << be_nl
      << "get_" << port_name << " (void);";

  return 0;
}

int
be_visitor_executor_exh::visit_consumes (be_consumes *node)
{
  const char *obj_name =
    node->consumes_type ()->full_name ();

  const char *port_name =
    node->local_name ()->get_string ();

  os_ << be_nl_2
      << "virtual void" << be_nl
      << "push_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << " * ev);" << be_uidt;

  return 0;
}
