
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
  this->ctx_->interface (this->node_);
  be_visitor_attribute v (this->ctx_);
  v.op_scope (op_scope_);
  return v.visit_attribute (node);
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
      << comment_border_;

  os_ << be_nl << be_nl
      << lname << "_exec_i::" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << lname << "_exec_i::~" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << "// Supported operations and attributes.";

  op_scope_ = node;

  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (node_);

  Component_Exec_Op_Attr_Generator op_attr_gen (this);

  int status =
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
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << be_nl << be_nl
      << "// Component attributes and port operations.";

  if (this->visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("scope traversal failed\n")),
                        -1);
    }

  os_ << be_nl << be_nl
      << "// Operations from Components::"
      << be_global->ciao_container_type () << "Component.";

  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");
  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl << be_nl
      << "void" << be_nl
      << lname << "_exec_i::set_"
      << tao_cg->downcase (container_type)
      << "_context (" << be_idt_nl
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->ciao_context_ =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context::_narrow (ctx);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (this->ciao_context_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << lname << "_exec_i::configuration_complete (void)"
      << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << lname << "_exec_i::ccm_activate (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
      << "void" << be_nl
      << lname << "_exec_i::ccm_passivate (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";

  os_ << be_nl << be_nl
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

