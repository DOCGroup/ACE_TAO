
//=============================================================================
/**
 *  @file    facet_ami_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the AMI connector
 *  impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_ami_exh::be_visitor_facet_ami_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
}

be_visitor_facet_ami_exh::~be_visitor_facet_ami_exh (void)
{
}

int
be_visitor_facet_ami_exh::visit_component (be_component *node)
{
  this->node_ = node;

  return this->visit_scope (node);
}

int
be_visitor_facet_ami_exh::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  if (this->gen_reply_handler_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_reply_handler_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  if (this->gen_facet_executor_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_executor_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_facet_ami_exh::visit_operation (be_operation *node)
{
  AST_Decl *d =
    ScopeAsDecl (node->defined_in ());

  /// We end up here also from the visit_scope() call on the
  /// connector. We want to skip the CCM-related operations
  /// that were added to the connector since it's a component.
  /// We want only the facet interface operations.
  if (d->node_type () != AST_Decl::NT_interface)
    {
      return  0;
    }

  /// We're generating implementation operation declarations,
  /// so we can just use this visitor.
  be_visitor_operation_ih v (this->ctx_);

  if (v.visit_operation (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("be_visitor_operation_ih ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_facet_ami_exh::gen_reply_handler_class (void)
{
  const char *suffix = "_reply_handler";
  UTL_Scope *s = this->iface_->defined_in ();
  AST_Decl *scope = ScopeAsDecl (s);
  const char *scope_name = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");
  const char *iface_name = this->iface_->local_name ();

  /// The reply handler class we are generating inherits from the
  /// CORBA AMI skeleton class, not the AMI_xxxCallback class
  /// generated from the corresponding interface in this IDL file.
  /// So to get the correct *_excep operation signatures, we
  /// visit the scope of the AMI_xxxHandler interface generated
  /// by -GC, which must be applied to this IDL file.
  ACE_CString handler_str (scope_name);
  handler_str += smart_scope;
  handler_str += "AMI_";
  ACE_CString tmp (iface_name);
  handler_str += tmp.substr (ACE_OS::strlen ("AMI4CCM_"));
  handler_str += "Handler";

  os_ << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << iface_name << suffix << be_idt_nl
      << ": public ::POA_" << handler_str.c_str () << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << iface_name << suffix << " (" << be_idt_nl
      << "::" << scope_name << "::" << iface_name
      << "ReplyHandler_ptr callback);" << be_uidt_nl << be_nl
      << "virtual ~" << iface_name << suffix << " (void);";

  UTL_ScopedName *sn =
    idl_global->string_to_scoped_name (handler_str.c_str ());
  AST_Decl *d = s->lookup_by_name (sn, true);

  sn->destroy ();
  delete sn;
  sn = 0;

  be_interface *callback_iface =
    be_interface::narrow_from_decl (d);

  int const status =
    callback_iface->traverse_inheritance_graph (
      be_interface::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_facet_ami_exh")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("traverse_inheritance_graph() on ")
                  ACE_TEXT ("callback interface failed\n")));
    }

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << scope_name << "::" << iface_name
      << "ReplyHandler_var callback_;" << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_facet_ami_exh::gen_facet_executor_class (void)
{
  const char *suffix = "_exec_i";
  const char *scope_name =
    ScopeAsDecl (this->iface_->defined_in ())->full_name ();
  const char *iface_name = this->iface_->local_name ();

  os_ << be_nl << be_nl
      << "class " << export_macro_.c_str () << " "
      << iface_name << suffix << be_idt_nl
      << ": public virtual ::" << scope_name << "::CCM_"
      << iface_name << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << iface_name << suffix << " (void);" << be_nl << be_nl
      << "virtual ~" << iface_name << suffix
      << " (void);";

  if (this->visit_scope (this->iface_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exh")
                         ACE_TEXT ("::gen_facet_executor_class - ")
                         ACE_TEXT ("visit_scope() on facet ")
                         ACE_TEXT ("interface failed\n")),
                        -1);
    }

  os_ << be_nl << be_nl
      << "void" << be_nl
      << "set_session_context (" << be_idt_nl
      << "::Components::SessionContext_ptr ctx);" << be_uidt;

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << ScopeAsDecl (this->node_->defined_in ())->name ()
      << "::CCM_" << this->node_->local_name ()
      << "_Context_var context_;" << be_uidt_nl
      << "};";

  return 0;
}

