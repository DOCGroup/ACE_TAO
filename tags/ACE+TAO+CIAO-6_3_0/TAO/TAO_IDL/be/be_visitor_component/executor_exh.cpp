
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

#include "component.h"

be_visitor_executor_exh::be_visitor_executor_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
    // comment_start_border_ ("/**"),
    // comment_end_border_ (" */")
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
  if (node->imported ())
    {
      return 0;
    }
  this->node_ = node;
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No _cxx_ prefix.
  const char *lname =
    node->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "/// Component Executor Implementation Class: "
      << lname <<  "_exec_i";

  os_ << be_nl
      << "class ";

  os_ << lname
      << "_exec_i" << be_idt_nl
      << ": public virtual " << lname << "_Exec," << be_idt_nl;

  os_ << "public virtual ::CORBA::LocalObject";

  os_ << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  os_ << "/// Constructor" << be_nl
      << lname << "_exec_i (void);";

  os_ << be_nl
      << "/// Destructor" << be_nl
      << "virtual ~" << lname << "_exec_i (void);";

  os_ << be_nl_2
      << "/** @name Supported operations and attributes. */" << be_nl
      << "//@{";

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

  os_ << "/** @name Component attributes and port operations. */" << be_nl
      << "//@{";

  status = this->visit_component_scope (node);

  os_<< be_nl << "//@}" << be_nl_2;

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("visit_component_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << "/** @name Session component operations */" << be_nl
  //    Operations from Components::" << be_global->ciao_container_type ()
  //    << "Component. */" << be_nl
      << "//@{";

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl_2
      << "/// Setter for container context for this component" << be_nl
      << "/// @param[in] ctx - Container context" << be_nl
      << "virtual void set_"
      << tao_cg->downcase (container_type)
      << "_context ("
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx);";

  if (ACE_OS::strcmp (be_global->ciao_container_type (), "Session") == 0)
    {
      os_ << be_nl_2
          << "/// Component state change method to configuration_complete state" << be_nl
          << "virtual void configuration_complete (void);";

      os_ << be_nl_2
          << "/// Component state change method to activated state" << be_nl
          << "virtual void ccm_activate (void);" << be_nl_2
          << "/// Component state change method to passivated state" << be_nl
          << "virtual void ccm_passivate (void);";
    }

  os_ << be_nl_2
      << "/// Component state change method to removed state" << be_nl
      << "virtual void ccm_remove (void);";

  os_ << be_nl
      << "//@}";

  os_ << be_nl_2
      << "/** @name User defined public operations. */" << be_nl
      << "//@{";

  os_ << be_nl_2 << "//@}";

  os_ << be_uidt << be_nl_2
      << "private:" << be_idt_nl
      << "/// Context for component instance. Used for all middleware communication" << be_nl
      << global << sname << "::CCM_" << lname
      << "_Context_var ciao_context_;" << be_nl_2;


  // Traverse inheritance graph twice to group
  // component attributes and component facets for DOxygen documentation

  // Traverse inheritance grapp for component facets:

  // The overload of traverse_inheritance_graph() used here
  // doesn't automatically prime the queues.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (node_);

  be_visitor_executor_private_exh v (this->ctx_);
  v.node (node);

  v.set_flags (false, true);

 os_ << "/** @name Component attributes. */" << be_nl
      << "//@{";

  Exec_Attr_Decl_Generator attr_decl (&v);

  status =
    node->traverse_inheritance_graph (attr_decl,
                                      &os_,
                                      false,
                                      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("for attr decls failed\n")),
                        -1);
    }

  os_<< be_nl << "//@}" << be_nl_2;

  // Traverse inheritance grapp for component facets:

  // The overload of traverse_inheritance_graph() used here
  // doesn't automatically prime the queues.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (node_);

  be_visitor_executor_private_exh v_f (this->ctx_);
  v_f.node (node);

  v_f.set_flags (true, false);

  os_ << "/** @name Component facets. */" << be_nl
      << "//@{";

  Exec_Attr_Decl_Generator facet_decl (&v_f);

  status =
    node->traverse_inheritance_graph (facet_decl,
                                      &os_,
                                      false,
                                      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("for facet decls failed\n")),
                        -1);
    }

  os_<< be_nl << "//@}" << be_nl_2;


  os_ << "/** @name User defined members. */" << be_nl
      << "//@{";

  os_ << be_nl_2 << "//@}";

  os_ << be_nl_2
      << "/** @name User defined private operations. */" << be_nl
      << "//@{";

  os_ << be_nl_2 << "//@}";

  if (be_global->gen_ciao_exec_reactor_impl ())
    {
      os_ << be_nl_2
          << "/// Get the ACE_Reactor" << be_nl
          << "/// @return non-owning pointer to reactor" << be_nl
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
      << "/// Factory method and getter for " << port_name << " facet" << be_nl
      << "/// @return existing instance of facet if one exists, else creates one" << be_nl
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

// ==================================================

Exec_Attr_Decl_Generator::Exec_Attr_Decl_Generator (
      be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Exec_Attr_Decl_Generator::emit (
  be_interface * /*derived_interface */,
  TAO_OutStream *  /*os*/,
  be_interface * base_interface)
{

  // Even though this call seems unaware of CCM types, the
  // visitor must inherit from be_visitor_component_scope so
  // it will pick up attributes via porttypes.
  return visitor_->visit_scope (base_interface);
}
