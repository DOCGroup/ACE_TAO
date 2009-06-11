//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    component_exs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the exec impl source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_exs,
           "$Id$")

// ******************************************************
// Component visitor for CIAO exec impl source
// ******************************************************

be_visitor_component_exs::be_visitor_component_exs (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    node_ (0),
    op_scope_ (0),
    os_ (*ctx->stream ()),
    comment_border_ ("//=============================="
                     "=============================="),
    your_code_here_ ("/* Your code here. */"),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_component_exs::~be_visitor_component_exs (void)
{
}

int
be_visitor_component_exs::visit_component (be_component *node)
{
  node_ = node;
  TAO_OutStream &os_  = *this->ctx_->stream ();
  
  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node_->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;
    
  if (this->gen_facets () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("gen_facets() failed\n")),
                        -1);
    }
 
  if (this->gen_exec_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("gen_servant_class() failed\n")),
                        -1);
    }
    
  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";
     
  return 0;
}

int
be_visitor_component_exs::visit_operation (be_operation *node)
{
  be_visitor_operation_exs v (this->ctx_);
  v.scope (op_scope_);
  return v.visit_operation (node);
}

int
be_visitor_component_exs::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  v.op_scope (op_scope_);
  return v.visit_attribute (node);
}

int
be_visitor_component_exs::gen_facets (void)
{
  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node_->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      be_interface *intf =
        be_interface::narrow_from_decl (pd->impl);
      const char *lname = intf->local_name ();
      AST_Decl *s = ScopeAsDecl (intf->defined_in ());
      ACE_CString sname_str (s->full_name ());
      const char *sname = sname_str.c_str ();
      const char *global = (sname_str == "" ? "" : "::");
      
      os_ << be_nl
          << comment_border_ << be_nl
          << "// Facet Executor Implementation Class: "
          << lname << "_exec_i" << be_nl
          << comment_border_;
          
      os_ << be_nl << be_nl
          << lname << "_exec_i::" << lname
          << "_exec_i (void)" << be_nl
          << "{" << be_nl
          << "}";
          
      os_ << be_nl << be_nl
          << lname << "_exec_i::~" << lname
          << "_exec_i (void)" << be_nl
          << "{" << be_nl
          << "}";
          
      op_scope_ = intf;
          
      if (this->gen_facet_ops_attrs (intf) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_component_exs::gen_facet - "
                             "gen_facet_ops_attrs() failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_component_exs::gen_facet_ops_attrs (be_interface *node)
{
  os_ << be_nl << be_nl
      << "// Operations from ::" << node->full_name ();
      
  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues. 
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();
  node->get_insert_queue ().enqueue_tail (node);
      
  Component_Exec_Op_Attr_Generator op_attr_gen (this);
  
  int status =
    node->traverse_inheritance_graph (op_attr_gen,
                                      &os_,
                                      false,
                                      false);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("gen_facet_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component_exs::gen_exec_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
  
  // In the interest of pretty formatting....
  if (node_->provides ().size () > 0)
    {
      os_ << be_nl;
    }
  
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
      
  op_scope_ = node_;
      
  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues. 
  node_->get_insert_queue ().reset ();
  node_->get_del_queue ().reset ();
  node_->get_insert_queue ().enqueue_tail (node_);
      
  Component_Exec_Op_Attr_Generator op_attr_gen (this);
  
  int status =
    node_->traverse_inheritance_graph (op_attr_gen,
                                       &os_,
                                       false,
                                       false);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("gen_exec_class - ")
                         ACE_TEXT ("traverse_inheritance_graph() failed\n")),
                        -1);
    }
    
  os_ << be_nl << be_nl
      << "// Component attributes.";
    
  status = this->gen_component_attrs_r (node_);
    
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exs::")
                         ACE_TEXT ("gen_exec_class - ")
                         ACE_TEXT ("gen_component_attrs_r() failed\n")),
                        -1);
    }
    
  os_ << be_nl << be_nl
      << "// Port operations.";
    
  this->gen_provides_r (node_);
  this->gen_consumes_r (node_);
    
  this->gen_non_type_specific ();
  
  return 0;
}

int
be_visitor_component_exs::gen_component_attrs_r (AST_Component *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  /// Traverse the scope and ignore everything but attributes.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      be_attribute *attr = be_attribute::narrow_from_decl (d);
      
      if (attr == 0)
        {
          continue;
        }

      if (attr->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_exs::")
                             ACE_TEXT ("gen_component_attrs_r - ")
                             ACE_TEXT ("be_attribute::accept() failed\n")),
                            -1);
        }
    }
    
 node = node->base_component ();
 return this->gen_component_attrs_r (node);
}

void
be_visitor_component_exs::gen_provides_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  AST_Component::port_description *pd = 0;
    
  for (AST_Component::PORTS::ITERATOR i = node->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_provides (pd->impl,
                          pd->id->get_string ());
    }  
  
  node = node->base_component ();
  this->gen_provides_r (node);
}

void
be_visitor_component_exs::gen_provides (AST_Type *obj,
                                        const char *port_name)
{
  AST_Decl *scope = ScopeAsDecl (obj->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = obj->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl << be_nl
      << global << sname << "::CCM_" << lname
      << "_ptr" << be_nl
      << node_->local_name () << "_exec_i::get_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_nl
      << "return " << global << sname << "::CCM_"
      << lname << "::_nil ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_exs::gen_consumes_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  AST_Component::port_description *pd = 0;
    
  for (AST_Component::PORTS::ITERATOR i = node->consumes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_consumes (pd->impl->full_name (),
                          pd->id->get_string ());
    }
  
  node = node->base_component ();
  this->gen_consumes_r (node);
}

void
be_visitor_component_exs::gen_consumes (const char *obj_name,
                                        const char *port_name)
{
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_exec_i::push_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << " * /* ev */)" << be_uidt_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";
}

void
be_visitor_component_exs::gen_non_type_specific (void)
{
  os_ << be_nl << be_nl
      << "// Operations from Components::SessionComponent.";
      
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_exec_i::set_session_context (" << be_idt_nl
      << "::Components::SessionContext_ptr ctx)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_ =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context::_narrow (ctx);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (this->context_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_exec_i::configuration_complete (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_exec_i::ccm_activate (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_exec_i::ccm_passivate (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_exec_i::ccm_remove (void)" << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_uidt_nl
      << "}";
}

void
be_visitor_component_exs::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::EnterpriseComponent_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr retval ="
      << be_idt_nl
      << "::Components::EnterpriseComponent::_nil ();"
      << be_uidt_nl << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "retval," << be_nl
      << node_->local_name () << "_exec_i," << be_nl
      << "::Components::EnterpriseComponent::_nil ());"
      << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

Component_Exec_Op_Attr_Generator::Component_Exec_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Component_Exec_Op_Attr_Generator::emit (be_interface * /*derived_interface */,
                                        TAO_OutStream * /* os */,
                                        be_interface * base_interface)
{
  if (base_interface->node_type () == AST_Decl::NT_component)
    {
      return 0;
    }

  return visitor_->visit_scope (base_interface);
}

