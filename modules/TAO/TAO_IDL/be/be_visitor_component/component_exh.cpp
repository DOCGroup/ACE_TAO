//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    component_exh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the exec impl header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_exh,
           "$Id$")

// ******************************************************
// Component visitor for CIAO exec impl header
// ******************************************************

be_visitor_component_exh::be_visitor_component_exh (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    node_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_component_exh::~be_visitor_component_exh (void)
{
}

int
be_visitor_component_exh::visit_component (be_component *node)
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
                         ACE_TEXT ("be_visitor_component_exh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("gen_facets() failed\n")),
                        -1);
    }
  
  if (this->gen_exec_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exh::")
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
be_visitor_component_exh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_component_exh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_component_exh::gen_facets (void)
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
          << "class " << export_macro_.c_str () << " "
          << lname << "_exec_i" << be_idt_nl
          << ": public virtual " << global << sname << "::CCM_"
          << lname << "," << be_idt_nl
          << "public virtual ::CORBA::LocalOBject"
          << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "public:" << be_idt_nl
          << lname << "_exec_i (void);" << be_nl
          << "virtual ~" << lname << "_exec_i (void);";
          
      os_ << be_nl << be_nl
          << "// Operations and attributes from ::"
          << intf->full_name ();
         
      if (this->gen_facet_ops_attrs (intf) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_component_exh::gen_facet - "
                             "gen_facet_ops_attrs() failed\n"),
                            -1);
        }
        
      os_ << be_uidt_nl
          << "};" << be_nl;
    }

  return 0;
}

int
be_visitor_component_exh::gen_facet_ops_attrs (be_interface *node)
{
  int status =
    node->traverse_inheritance_graph (
      be_visitor_component_exh::op_attr_decl_helper,
      &os_);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exh::")
                         ACE_TEXT ("gen_facet_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() failed\n")),
                        -1);
    }
    
  return 0;
}

int
be_visitor_component_exh::gen_exec_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
     
  os_ << be_nl
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
  
  os_ << be_nl << be_nl
      << "// Supported operations and attributes.";
      
  int status =
    node_->traverse_inheritance_graph (
      be_visitor_component_exh::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_exh::")
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
                         ACE_TEXT ("be_visitor_component_exh::")
                         ACE_TEXT ("gen_exec_class - ")
                         ACE_TEXT ("gen_component_attrs_r() failed\n")),
                        -1);
    }
    
  os_ << be_nl << be_nl
      << "// Port operations.";
    
  this->gen_provides_r (node_);
  this->gen_consumes_r (node_);
    
  this->gen_non_type_specific ();
  
  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context_var context_;"
      << be_uidt_nl
      << "};";
     
  return 0;
}

int
be_visitor_component_exh::gen_component_attrs_r (AST_Component *node)
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
                             ACE_TEXT ("be_visitor_component_exh::")
                             ACE_TEXT ("gen_component_attrs_r - ")
                             ACE_TEXT ("be_attribute::accept() failed\n")),
                            -1);
        }
    }
    
 node = node->base_component ();
 return this->gen_component_attrs_r (node);
}

void
be_visitor_component_exh::gen_ports (AST_Component *node,
                                     port_kind kind)
{
  AST_Component::port_description *pd = 0;
  
  switch (kind)
    {
      case PROVIDES:
        for (AST_Component::PORTS::ITERATOR i = node->provides ().begin ();
             !i.done ();
             i.advance ())
          {
            i.next (pd);
            this->gen_provides (pd->impl,
                                pd->id->get_string ());
          }
          
        break;
      case CONSUMES:
        for (AST_Component::PORTS::ITERATOR i = node->consumes ().begin ();
             !i.done ();
             i.advance ())
          {
            i.next (pd);
            this->gen_consumes (pd->impl->full_name (),
                                pd->id->get_string ());
          }
          
        break;
      default:
        break;
    }
}

void
be_visitor_component_exh::gen_provides_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  this->gen_ports (node, PROVIDES);
  
  node = node->base_component ();
  this->gen_provides_r (node);
}

void
be_visitor_component_exh::gen_provides (AST_Type *obj,
                                        const char *port_name)
{
  AST_Decl *scope = ScopeAsDecl (obj->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = obj->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");
     
  os_ << be_nl << be_nl
      << "virtual " << global << sname << "::CCM_"
      << lname << "_ptr" << be_nl
      << "get_" << port_name << " (void);";
}

void
be_visitor_component_exh::gen_consumes_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  this->gen_ports (node, CONSUMES);
  
  node = node->base_component ();
  this->gen_consumes_r (node);
}

void
be_visitor_component_exh::gen_consumes (const char *obj_name,
                                        const char *port_name)
{
  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "push_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << " * ev);" << be_uidt;
}

void
be_visitor_component_exh::gen_non_type_specific (void)
{
  os_ << be_nl << be_nl
      << "// Operations from Components::SessionComponent.";
    
  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "set_session_context (" << be_idt_nl
      << "::Components::SessionContext_ptr ctx);" << be_uidt;
      
  os_ << be_nl << be_nl
      << "virtual void configuration_complete (void);";
      
  os_ << be_nl << be_nl
      << "virtual void ccm_activate (void);" << be_nl
      << "virtual void ccm_passivate (void);" << be_nl
      << "virtual void ccm_remove (void);";    
}

void
be_visitor_component_exh::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::EnterpriseComponent_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void);"; 
}

int
be_visitor_component_exh::op_attr_decl_helper (be_interface *derived,
                                               be_interface *ancestor,
                                               TAO_OutStream *os)
{
  if (be_component::narrow_from_decl (ancestor) != 0)
    {
      return 0;
    }

  /// We're in a static method, so we have to instantiate a temporary
  /// visitor and context.
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_EXH);
  ctx.stream (os);
  be_visitor_component_exh visitor (&ctx);
  
  /// Since this visitor overriddes only visit_operation() and 
  /// visit_attribute(), we can get away with this for the declarations.
  return visitor.visit_scope (ancestor);
}

