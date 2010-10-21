
//=============================================================================
/**
 *  @file    executor_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================
#include <TAO_IDL/be_include/be_helper.h>

be_visitor_executor_exs::be_visitor_executor_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    op_scope_ (0),
    comment_border_ ("//=============================="
                     "=============================="),
    your_code_here_ ("/* Your code here. */")
{
}

be_visitor_executor_exs::~be_visitor_executor_exs (void)
{
}

int
be_visitor_executor_exs::visit_operation (be_operation *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  // Components have implied IDL operations added to the AST, but
  // we are interested only in supported interface operations.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }

  be_visitor_operation_exs v (this->ctx_);
  v.scope (op_scope_);
  return v.visit_operation (node);
}

int
be_visitor_executor_exs::visit_attribute (be_attribute *node)
{
  AST_Decl::NodeType nt = this->node_->node_type ();

  // Executor attribute code generated for porttype attributes
  // always in connectors and only for mirrorports in components.
  if (this->in_ext_port_ && nt == AST_Decl::NT_component)
    {
      return 0;
    }

  os_ << be_uidt_nl << be_idt_nl;

  be_type *ft = node->field_type ();
  be_visitor_operation_rettype rt_visitor (this->ctx_);

  if (ft->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept on return type failed\n")),
                        -1);
    }

  os_ << be_nl
      << this->node_->original_local_name () << "_exec_i::"
      << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << " (void)" << be_nl
      << "{" << be_idt;

  be_visitor_attr_return ar_visitor (this->ctx_);
  ar_visitor.attr_name (node->original_local_name ()->get_string ());

  if (ft->accept (&ar_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept on get visitor failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "}" << be_uidt_nl;

  os_ << be_idt_nl
      << "void" << be_nl
      << this->node_->original_local_name () << "_exec_i::"
      << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << " (" << be_idt_nl;

  be_visitor_attr_setarg_type at_visitor (this->ctx_);

  if (ft->accept (&at_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept on set arg type failed\n")),
                        -1);
    }

  os_ << node->local_name () << ")" << be_uidt_nl
      << "{" << be_idt;

  be_visitor_attr_assign as_visitor (this->ctx_);
  as_visitor.attr_name (node->original_local_name ()->get_string ());

  if (ft->accept (&as_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept on set func body failed\n")),
                        -1);
    }
  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_executor_exs::visit_component (be_component *node)
{
  // In the interest of pretty formatting, if we have generated
  // at least one facet executor impl class, add a linebreak.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->node_type () == AST_Decl::NT_provides)
        {
          os_ << be_nl;
          break;
        }
    }

  node_ = node;
  const char *lname = node->local_name ();

  os_ << be_nl
      << comment_border_ << be_nl
      << "// Component Executor Implementation Class: "
      << lname << "_exec_i" << be_nl
      << comment_border_ << be_uidt_nl;

  os_ << be_idt_nl
      << lname << "_exec_i::" << lname
      << "_exec_i (void)";

  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (this->node_);
  
  be_visitor_executor_exs_attr_init ai_visitor (this->ctx_);
  ai_visitor.node (node);

  Component_Exec_Attr_Init_Generator attr_init_gen (&ai_visitor);

  int status =
    node->traverse_inheritance_graph (attr_init_gen,
                                      &os_,
                                      false,
                                      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("for attr init failed\n")),
                        -1);
    }

  if (ai_visitor.attr_generated ())
    {
      os_ << be_uidt << be_uidt_nl;
    }

  os_ << "{" << be_nl
      << "}" << be_uidt_nl;

  os_ << be_idt_nl
      << lname << "_exec_i::~" << lname
      << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}" << be_uidt_nl;

  os_ << be_idt_nl
      << "// Supported operations and attributes."
      << be_uidt_nl << be_idt;

  if (be_global->gen_ciao_exec_reactor_impl ())
    {
      os_ << be_nl
          << "ACE_Reactor*" << be_nl
          << lname << "_exec_i::reactor (void)" << be_nl
          << "{" << be_idt_nl
          << "ACE_Reactor* reactor = 0;" << be_nl
          << "::CORBA::Object_var ccm_object = " << be_idt_nl
          << "this->ciao_context_->get_CCM_object();" << be_uidt_nl
          << "if (! ::CORBA::is_nil (ccm_object.in ())) " << be_idt_nl
          << "{" << be_idt_nl
          << "::CORBA::ORB_var orb = ccm_object->_get_orb ();" << be_nl
          << "if (! ::CORBA::is_nil (orb.in ()))" << be_idt_nl
          << "{" << be_idt_nl
          << "reactor = orb->orb_core ()->reactor ();"
          << be_uidt_nl << "}"
          << be_uidt << be_uidt_nl << "}"
          << be_uidt_nl << "if (reactor == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::INTERNAL ();"
          << be_uidt_nl << "}"
          << be_uidt_nl << "return reactor;"
          << be_uidt_nl << "}" << be_uidt << be_uidt_nl;
    }

  op_scope_ = node;

  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (node_);

  Component_Exec_Op_Attr_Generator op_attr_gen (this);

  status =
    node->traverse_inheritance_graph (op_attr_gen,
                                      &os_,
                                      false,
                                      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("for operations failed\n")),
                        -1);
    }

  os_ << be_idt_nl
      << "// Component attributes and port operations."
      << be_uidt << be_idt;

  if (this->visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("scope traversal failed\n")),
                        -1);
    }

  os_ << be_uidt_nl << be_idt_nl
      << "// Operations from Components::"
      << be_global->ciao_container_type () << "Component."
      << be_uidt_nl;

  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");
  const char *container_type = be_global->ciao_container_type ();

  os_ << be_idt_nl
      << "void" << be_nl
      << lname << "_exec_i::set_"
      << tao_cg->downcase (container_type)
      << "_context (" << be_idt_nl
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->ciao_context_ =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context::_narrow (ctx);" << be_uidt << be_uidt
      << be_uidt_nl << be_idt << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_context_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  if (ACE_OS::strcmp (be_global->ciao_container_type (), "Session") == 0)
    {
      os_ << be_uidt_nl << be_idt_nl
          << "void" << be_nl
          << lname << "_exec_i::configuration_complete (void)"
          << be_nl
          << "{" << be_idt_nl
          << your_code_here_ << be_uidt_nl
          << "}";

      os_ << be_uidt_nl << be_idt_nl
          << "void" << be_nl
          << lname << "_exec_i::ccm_activate (void)" << be_nl
          << "{" << be_idt_nl
          << your_code_here_ << be_uidt_nl
          << "}";

      os_ << be_uidt_nl << be_idt_nl
          << "void" << be_nl
          << lname << "_exec_i::ccm_passivate (void)" << be_nl
          << "{" << be_idt_nl
          << your_code_here_ << be_uidt_nl
          << "}";
    }

  os_ << be_uidt_nl << be_idt_nl
      << "void" << be_nl
      << lname << "_exec_i::ccm_remove (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_executor_exs::visit_provides (be_provides *node)
{
  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  be_type *obj = node->provides_type ();
  const char *iname =
    obj->original_local_name ()->get_string ();

  AST_Decl *scope = ScopeAsDecl (obj->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");

  ACE_CString lname_str (this->ctx_->port_prefix ());
  lname_str += node->original_local_name ()->get_string ();
  const char *lname = lname_str.c_str ();

  os_ << be_nl << be_nl
      << global << sname << "::CCM_"
      << iname << "_ptr" << be_nl
      << node_->local_name () << "_exec_i::get_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_" << port_name
      << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << lname << "_exec_i *tmp = 0;" << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "tmp," << be_nl
      << lname << "_exec_i (" << be_idt_nl
      << "this->ciao_context_.in ())," << be_uidt_nl
      << global << sname << "::CCM_" << iname << "::_nil ());"
      << be_uidt_nl << be_nl
      << "this->ciao_" << port_name << "_ = tmp;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return" << be_idt_nl
      << global << sname << "::CCM_" << iname
      << "::_duplicate (" << be_idt_nl
      << "this->ciao_" << port_name << "_.in ());"
      << be_uidt << be_uidt << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_executor_exs::visit_consumes (be_consumes *node)
{
  be_eventtype *obj = node->consumes_type ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->original_local_name () << "_exec_i::push_"
      << port_name << " (" << be_idt_nl
      << "::" << IdentifierHelper::orig_sn (obj->name (), false).c_str ()
      << " * /* ev */)" << be_uidt_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";

  return 0;
}

// ==================================================

Component_Exec_Attr_Init_Generator::Component_Exec_Attr_Init_Generator (
      be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Component_Exec_Attr_Init_Generator::emit (be_interface * /*derived_interface */,
                                        TAO_OutStream * /* os */,
                                        be_interface * base_interface)
{
  // Even though this call seems unaware of CCM types, the
  // visitor must inherit from be_visitor_component_scope so
  // it will pick up attributes via porttypes.
  return visitor_->visit_scope (base_interface);
}

